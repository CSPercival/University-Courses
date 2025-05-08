// Cyryl Szatan 340510
// jestem jedynym autorem
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

#include "mm.h"
#include "memlib.h"

/* If you want debugging output, use the following macro.
 * When you hand in, remove the #define DEBUG line. */
// #define DEBUG
#ifdef DEBUG
#define debug(fmt, ...) printf("%s: " fmt "\n", __func__, __VA_ARGS__)
#define msg(...) printf(__VA_ARGS__)
#else
#define debug(fmt, ...)
#define msg(...)
#endif

#define __unused __attribute__((unused))

/* do not change the following! */
#ifdef DRIVER
/* create aliases for driver tests */
#define malloc mm_malloc
#define free mm_free
#define realloc mm_realloc
#define calloc mm_calloc
#endif /* !DRIVER */

typedef int32_t word_t; /* Heap is bascially an array of 4-byte words. */

typedef enum {
  FREE = 0,     /* Block is free */
  USED = 1,     /* Block is used */
  PREVFREE = 2, /* Previous block is free (optimized boundary tags) */
} bt_flags;

static word_t *heap_low_ptr; /* Address of the first byte of the whole heap*/
static long long
  heap_low; /* Address of the first byte of the whole heap casted to long long*/
static word_t *heap_start; /* Address of the first block */
// static word_t *heap_end;   /* Address past last byte of last block */
static word_t *last;        /* Points at last block */
static word_t *lists_start; /* Points at header of first list */

static word_t number_of_small_lists =
  32; // lists that stores respectively blocks of sizes 1 * 16B, 2 * 16B, ... ,
      // 32 * 16B
static word_t number_of_big_lists =
  6; // lists that stores blocks of sizes from range [2^k, 2^(k + 1) ) there
     // will be one additional list at the end with size = inf
static word_t number_of_lists; // sum of small lists and big lists

/* --=[ boundary tag handling ]=-------------------------------------------- */

static inline size_t bt_size(word_t *bt) {
  return *bt & ~(USED | PREVFREE);
}

static inline int bt_used(word_t *bt) {
  return *bt & USED;
}

static inline int bt_free(word_t *bt) {
  return !(*bt & USED);
}

/* Previous block free flag handling for optimized boundary tags. */
static inline bt_flags bt_get_prevfree(word_t *bt) {
  return *bt & PREVFREE;
}

static inline void bt_clr_prevfree(word_t *bt) {
  if (bt)
    *bt &= ~PREVFREE;
}

static inline void bt_set_prevfree(word_t *bt) {
  *bt |= PREVFREE;
}

/* Given boundary tag address calculate it's buddy address. */
static inline word_t *bt_footer(word_t *bt) {
  return (void *)bt + bt_size(bt) - sizeof(word_t);
}

/* Given payload pointer returns an address of boundary tag. */
static inline word_t *bt_fromptr(void *ptr) {
  return (word_t *)ptr - 1;
}

/* Returns address of payload. */
static inline void *bt_payload(word_t *bt) {
  return bt + 1;
}

/* Returns address of next block or NULL. */
static inline word_t *bt_next(word_t *bt) {
  return (void *)bt + bt_size(bt);
}

/* Returns address of previous block or NULL. */
static inline word_t *bt_prev(word_t *bt) {
  return (void *)bt - bt_size(bt - 1);
}

/* Creates boundary tag(s) for given block.
    For USED block, creates only header
    For Free block, creates header and footer
    Clears or Sets prevfree bit in the next block respectively
*/
static inline void bt_make(word_t *bt, size_t size, bt_flags flags) {
  (*bt) = size | flags;
  if (size == 0)
    return;
  if (flags & USED) {
    bt_clr_prevfree(bt_next(bt));
    return;
  }
  bt_set_prevfree(bt_next(bt));
  bt = bt_footer(bt);
  (*bt) = size | flags;
}

/* --=[ explicit list links handling
 * ]=------------------------------------------ */

// Converts address from heap to 32-bit offset
static inline word_t link_a2l(word_t *address) {
  return (long long)address;
}

// Converts 32-bit offset to address
static inline word_t *link_l2a(word_t link) {
  return (word_t *)(heap_low | link);
}

// Sets what is the previous block on the explicit list
static inline void link_set_pred(word_t *curr, word_t *pred) {
  (*(curr + 1)) = link_a2l(pred);
}
// Returns previous block on the explicit list
static inline word_t *link_get_pred(word_t *curr) {
  return link_l2a((*(curr + 1)));
}

// Sets what is the next block on the explicit list
static inline void link_set_succ(word_t *curr, word_t *succ) {
  (*(curr + 2)) = link_a2l(succ);
}
// Returns next block on the explicit list
static inline word_t *link_get_succ(word_t *curr) {
  return link_l2a((*(curr + 2)));
}

// Removes block from free list
static inline void link_remove(word_t *curr) {
  word_t *pred = link_get_pred(curr);
  word_t *succ = link_get_succ(curr);
  if (pred)
    link_set_succ(pred, succ);
  if (succ)
    link_set_pred(succ, pred);
}

// Adds free block on the list beetween pred and succ
static inline void link_make(word_t *curr, word_t *pred, word_t *succ) {
  if (pred)
    link_set_succ(pred, curr);
  if (succ)
    link_set_pred(succ, curr);

  link_set_pred(curr, pred);
  link_set_succ(curr, succ);
}

/* --=[ multiple lists handling ]=------------------------------------------ */

// Returns address to header of list that contains blocks with sizes at least
// equal to size
static inline word_t *find_list(size_t size) {
  word_t list = size >> 4;
  if (list <= number_of_small_lists)
    return lists_start +
           ((list - 1) << 2); // block should be in small list section
  list = 54 - __builtin_clz(size);
  if (list > number_of_lists)
    list = number_of_lists;
  return lists_start + (list << 2); // block should be in big list section
}

// Merges processes of finding right list, adding to list and creating boundary
// tags for free block
static inline void add_free_block(word_t *block, size_t size) {
  word_t *prev_block = find_list(size);
  word_t *next_block = link_get_succ(prev_block);
  link_make(block, prev_block, next_block);
  bt_make(block, size, FREE);
}

/* --=[ miscellanous procedures ]=------------------------------------------ */

static inline void dmsg(word_t *ptr, char *message) {
  if (!ptr) {
    printf("    D %s     ERROR\n", message);
  } else {
    printf("    D %s:    %p  ->  (%ld   %d)\n", message, ptr, bt_size(ptr),
           (*ptr) & (USED | FREE | PREVFREE));
  }
}

// Prints content of heap from s to e
static inline void printheap(int s, int e) {
  printf("\nHEAP\n");
  printf("Heap low: %p\n", heap_low_ptr);
  printf("Lists start: %p\n", lists_start);
  printf("Heap start: %p\n", heap_start);
  printf("Last: %p\n", last);

  void *ptr = lists_start + s * sizeof(word_t);
  void *btag = NULL;
  if (s == 0) {
    btag = ptr + 16;
  }
  for (int i = s; ptr <= (void *)last; i++) {
    if (ptr == btag) {
      dmsg(ptr, "HEAP BT");
      btag = bt_next(btag);
    } else {
      dmsg(ptr, "HEAP");
    }
    ptr += sizeof(word_t);
  }
  printf("\n");
}

/* Calculates block size incl. header, footer & payload,
 * and aligns it to block boundary (ALIGNMENT). */
static inline size_t blksz(size_t size) {
  return (sizeof(word_t) + size + ALIGNMENT - 1) & -ALIGNMENT;
}

static void *morecore(size_t size) {
  void *ptr = mem_sbrk(size);
  if (ptr == (void *)-1)
    return NULL;
  return ptr;
}

// Creates block of required size at the end of the heap
//   if last block of heap before extension is free, function uses it to create
//   new block
static inline void *extend_heap(size_t size) {
  word_t *new_block;
  size_t block_size = size;
  if (bt_get_prevfree(last)) {
    // last block of heap is free, reduce the number of new bytes that are
    // needed
    new_block = bt_prev(last);
    link_remove(new_block);
    size -= bt_size(new_block);
    if (morecore(size) == NULL)
      return NULL;
  } else {
    // last block of heap is used, need for exatcly size new bytes
    new_block = morecore(size);
    if (new_block == NULL)
      return NULL;
  }

  last = (void *)last + size;
  bt_make(last, 0, USED);
  bt_make(new_block, block_size, USED);
  return new_block;
}

/* --=[ mm_init ]=---------------------------------------------------------- */

// Creates headers for every type of list
//  lists are connected and forms one big lists which starts and ends in
//  heap_low address
int mm_init(void) {
  number_of_lists = number_of_small_lists + number_of_big_lists;
  heap_low_ptr = mem_heap_lo();
  heap_low = (long long)heap_low_ptr;
  void *ptr =
    morecore((number_of_lists + 1) * ALIGNMENT + ALIGNMENT - sizeof(word_t));
  if (!ptr)
    return -1;

  lists_start = ptr + 12;
  last = ptr + (number_of_lists + 1) * ALIGNMENT + ALIGNMENT - sizeof(word_t);
  heap_start = last;

  word_t *prev_block = heap_low_ptr;
  word_t *curr_block = lists_start;
  word_t *next_block = lists_start + 4;
  size_t size = 16;

  // create lists
  for (int i = 0; i < number_of_lists; i++) {
    bt_make(curr_block, size, USED);
    link_make(curr_block, prev_block, next_block);
    prev_block = curr_block;
    curr_block = next_block;
    next_block += 4;
  }
  // create last list
  bt_make(curr_block, size, USED);
  link_make(curr_block, prev_block, heap_low_ptr);

  // create last block
  bt_make(last, 0, USED);
  return 0;
}

/* --=[ malloc ]=----------------------------------------------------------- */

/* First fit startegy.
Weighted memory utilization: 79.9%
Total memory utilization: 93.54%
Instructions per operation: 392
*/
// Finds starting point via find_list and treverses through all free blocks
// until free block with enough space is found
static word_t *find_fit(size_t reqsz) {
  word_t *curr_block = find_list(reqsz);
  while (curr_block != heap_low_ptr &&
         (bt_size(curr_block) < reqsz || curr_block < heap_start)) {
    curr_block = link_get_succ(curr_block);
  }
  return curr_block;
}

void *malloc(size_t size) {
  size_t reqsz = blksz(size);
  word_t *dest = find_fit(reqsz);
  if (dest == heap_low_ptr) {
    // Couldn't find free block with at least reqsz bytes. Need to extend heap
    dest = extend_heap(reqsz);
    if (dest == NULL)
      return NULL;
    return bt_payload(dest);
  } else {
    // Found free block with at least reqsz bytes
    size_t leftover = bt_size(dest) - reqsz;
    link_remove(dest);
    if (leftover > 0) {
      // If current block is larger than reqsz, add remainings to the free list
      add_free_block((void *)dest + reqsz, leftover);
    }
    bt_make(dest, reqsz, USED);
    return bt_payload(dest);
  }
}

/* --=[ free ]=------------------------------------------------------------- */

void free(void *ptr) {
  if (ptr < (void *)heap_start)
    return;
  ptr -= sizeof(word_t);

  size_t block_size = bt_size(ptr);
  word_t *next_block = bt_next(ptr);

  // Check if next block is free. If yes -> merge
  if (bt_free(next_block)) {
    block_size += bt_size(next_block);
    link_remove(next_block);
  }
  // Check if previous block is free. If yes -> merge
  if (bt_get_prevfree(ptr)) {
    ptr = bt_prev(ptr);
    block_size += bt_size(ptr);
    link_remove(ptr);
  }
  add_free_block(ptr, block_size);
}

/* --=[ realloc ]=---------------------------------------------------------- */

void *realloc(void *old_ptr, size_t size) {
  /* If size == 0 then this is just free, and we return NULL. */
  if (size == 0) {
    free(old_ptr);
    return NULL;
  }
  /* If old_ptr is NULL, then this is just malloc. */
  if (!old_ptr)
    return malloc(size);

  old_ptr -= sizeof(word_t);
  size_t old_size = bt_size(old_ptr);
  size_t reqsz = blksz(size);
  // If new size is smaller than block size, leave it as it is
  if (reqsz <= old_size) {
    return bt_payload(old_ptr);
  }
  word_t *next_block = bt_next(old_ptr);
  // If next block is free and covers the needs, merge it to the current block
  // and return old_ptr
  if (bt_free(next_block)) { // && reqsz <= (old_size + bt_size(next_block))
    word_t leftover = (old_size + bt_size(next_block)) - reqsz;
    if (leftover >= 0) {
      link_remove(next_block);
      bt_make(old_ptr, reqsz, USED);
      if (leftover > 0) {
        // If next block is larger than it needs to be, add remainings to the
        // free list
        add_free_block(bt_next(old_ptr), leftover);
      }
      return bt_payload(old_ptr);
    }
  }

  // In other cases, just malloc the reqsz bytes, copy contents to new allocated
  // block and free old block
  void *new_ptr = malloc(reqsz);
  old_ptr += sizeof(word_t);

  memcpy(new_ptr, old_ptr, old_size - 4);
  free(old_ptr);
  return new_ptr;
}

/* --=[ calloc ]=----------------------------------------------------------- */

void *calloc(size_t nmemb, size_t size) {
  size_t bytes = nmemb * size;
  void *new_ptr = malloc(bytes);
  if (new_ptr)
    memset(new_ptr, 0, bytes);
  return new_ptr;
}

/* --=[ mm_checkheap ]=----------------------------------------------------- */

/*Traverse through free blocks on the heap starting from curr_block and check
  if: 1) predecessor and successor are pointing at each other via links 2) only
  free blocks are on the free list Also stores information in flag, so go_all
  can anylyze it flag&1 => special block is on the free block list and after
  curr_block flag^2 => Exist free block that is on the list and is inside
  special block
*/

static int go_free(word_t *curr_block, word_t *special_block) {
  int flag = 0;
  word_t *next_special_block = bt_next(special_block);
  while (curr_block != heap_low_ptr) {
    if (curr_block != lists_start) {
      if (link_get_succ(link_get_pred(curr_block)) != curr_block) {
        printf("CHECK HEAP ERROR\n One way connection between blocks: %p  - %p "
               "on the free block list\n",
               link_get_pred(curr_block), curr_block);
        exit(0);
      }
    }
    if (curr_block == special_block)
      flag |= 1;
    if (special_block < curr_block && curr_block < next_special_block)
      flag |= 2;
    if (heap_start <= curr_block && bt_used(curr_block)) {
      printf("CHECK HEAP ERROR\n used block: %p on the free block list\n",
             curr_block);
      exit(0);
    }
    curr_block = link_get_succ(curr_block);
  }
  return flag;
}

/*Traverse through all blocks on the heap and check if:
  1) free block is not inside of other block
  2) every free block is on the list of all free blocks
  3) two conecutive blocks are not marked as free at the same time
  4) Prevfree property is preserved
  5) Every free block eventually leads to end of a list(heap_low)
*/
static void go_all(word_t *curr_block) {
  word_t *next_block;
  int flag;
  while (curr_block != last) {
    // dmsg(curr_block, "GA while");
    next_block = bt_next(curr_block);
    flag = go_free(lists_start, curr_block);
    if (flag & 2) {
      printf("CHECK HEAP ERROR\n found free block inside other block %p\n",
             curr_block);
      exit(0);
    }
    if (bt_free(curr_block)) {
      if (!(flag & 1)) {
        printf(
          "CHECK HEAP ERROR\n free block %p is not on the free block list\n",
          curr_block);
        exit(0);
      }
      if (bt_free(next_block)) {
        printf(
          "CHECK HEAP ERROR\n two consecutive blocks marked as free %p %p\n",
          curr_block, next_block);
        exit(0);
      }
      if (bt_get_prevfree(next_block) == 0) {
        printf("CHECK HEAP ERROR\n block %p after free block %p is not marked "
               "as prevfree\n",
               next_block, curr_block);
        exit(0);
      }
      go_free(curr_block, curr_block); // succ guides eventually to heap_low
    } else {
      if (bt_get_prevfree(next_block)) {
        printf("CHECK HEAP ERROR\n block %p after used block %p is marked as "
               "prevfree\n",
               next_block, curr_block);
        exit(0);
      }
    }

    curr_block = next_block;
  }
}

// Traverses through one list specified by curr_block and checks if every block
// on that list has right size
static void check_block_size_on_list(word_t *curr_block, size_t least,
                                     size_t most) {
  curr_block = link_get_succ(curr_block);
  while (heap_start <= curr_block) {
    if (bt_size(curr_block) < least || most < bt_size(curr_block)) {
      printf("CHECK HEAP ERROR\n block %p of size %ld is on wrong list\nfound "
             "on list containing blocks [%ld - %ld]",
             curr_block, bt_size(curr_block), least, most);
      exit(0);
    }
    curr_block = link_get_succ(curr_block);
  }
}

void mm_checkheap(int verbose) {
  printf("MM CHECKHEAP\n");
  go_all(lists_start);

  void *start = lists_start;
  for (int i = 1; i <= number_of_small_lists; i++) {
    check_block_size_on_list(start, i * ALIGNMENT, i * ALIGNMENT);
    start += ALIGNMENT;
  }
  size_t size = 1;
  while (size <= number_of_small_lists * ALIGNMENT)
    size <<= 1;
  for (int i = 0; i <= number_of_big_lists; i++) {
    check_block_size_on_list(start, size >> 1, size);
    start += ALIGNMENT;
    size <<= 1;
  }

  printf("HEAP VALID!!!\n");
}
