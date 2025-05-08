import sys

def B(i,j):
    return 'B_%d_%d' % (i,j)

def domains(Vs):
    return [ q + ' in 0..1' for q in Vs ]

def single_column_sum(idx, R, E):
    #return 'sum_list([' + ', '.join([B(i,idx) for i in range(R)]) + '], ' + str(E) + ')'
    return ' + '.join([B(i,idx) for i in range(R)]) + " #= " + str(E)

def column_sum(cols, R):
    return [single_column_sum(i,R,cols[i]) for i in range(len(cols))]

def single_row_sum(idx, C, E):
    #return 'sum_list([' + ', '.join([B(idx,i) for i in range(C)]) + '], ' + str(E) + ')'
    return ' + '.join([B(idx,i) for i in range(C)]) + " #= " + str(E)

def row_sum(rows, C):
    return [single_row_sum(i,C,rows[i]) for i in range(len(rows))]
    
def print_constraints(Cs, indent, d):
    position = indent
    print (indent * ' ', end='')
    for c in Cs:
        print (c + ',', end=' ')
        position += len(c)
        if position > d:
            position = indent
            print ()
            print (indent * ' ', end='')

def good_squares(R,C):
    cs = []
    not_good = [6,7,9,11,13,14]
    for i in range(R-1):
        for j in range(C-1):
            for k in not_good:
                cs.append('1 * %s + 2 * %s + 4 * %s + 8 * %s #\= %d' % (B(i,j),B(i,j+1),B(i+1,j),B(i+1,j+1),k))
    return cs


def single_good_stripe(f,s,t):
    #return '(%s + %s #= 2) #==> (%s #= 1)' % (f,t,s)
    return '1 * %s + 2 * %s + 4 * %s #\= %d' % (f,s,t,2)

def single_good_edge_stripe(f,s):
    return '1 * %s + 2 * %s #\= %d' % (f,s,1)

def good_stripes(R,C):
    cs = []

    for i in range(R):
        for j in range(1,C-1):
            cs.append(single_good_stripe(B(i,j-1),B(i,j),B(i,j+1)))

    for i in range(1,R-1):
        for j in range(C):
            cs.append(single_good_stripe(B(i-1,j),B(i,j),B(i+1,j)))
    
    for i in range(R):
        cs.append(single_good_edge_stripe(B(i,0),B(i,1)))
        cs.append(single_good_edge_stripe(B(i,C-1),B(i,C-2)))

    for i in range(C):
        cs.append(single_good_edge_stripe(B(0,i),B(1,i)))
        cs.append(single_good_edge_stripe(B(R-1,i),B(R-2,i)))

    return cs

def storms(rows, cols, triples):
    R = len(rows)
    C = len(cols)
    
    bs = [ B(i,j) for i in range(R) for j in range(C)]
    
    print(':- use_module(library(clpfd)).')
    #print(':- use_module(library(reif)).')
    print('solve([' + ', '.join(bs) + ']) :- ')
    
    cs = []

    for i,j,val in triples:
        cs.append( '%s #= %d' % (B(i,j), val) )

    cs += domains(bs) + column_sum(cols, R) + row_sum(rows, C) + good_squares(R,C) + good_stripes(R,C)

    print_constraints(cs, 4, 70),




    print('    labeling([ff], [' +  ', '.join(bs) + ']).' )
    print('')
    #print(":- tell('zad_output.txt'), solve(X), write(X), nl, told.")
    print(':- solve(X), write(X), nl.')

txt = open('zad_input.txt').readlines()
sys.stdout = open('zad_output.txt', 'w')

rows = list(map(int, txt[0].split()))
cols = list(map(int, txt[1].split()))
triples = []

for i in range(2, len(txt)):
    if txt[i].strip():
        triples.append(list(map(int, txt[i].split())))

storms(rows, cols, triples)            
        

