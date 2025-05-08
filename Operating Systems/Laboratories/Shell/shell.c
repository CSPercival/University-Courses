#ifdef READLINE
#include <readline/readline.h>
#include <readline/history.h>
#endif

#define DEBUG 0
#include "shell.h"

sigset_t sigchld_mask;

static void sigint_handler(int sig) {
  /* No-op handler, we just need break read() call with EINTR. */
  (void)sig;
}

/* Rewrite closed file descriptors to -1,
 * to make sure we don't attempt do close them twice. */
static void MaybeClose(int *fdp) {
  if (*fdp < 0)
    return;
  Close(*fdp);
  *fdp = -1;
}

/* Consume all tokens related to redirection operators.
 * Put opened file descriptors into inputp & output respectively. */
static int do_redir(token_t *token, int ntokens, int *inputp, int *outputp) {
  token_t mode = NULL; /* T_INPUT, T_OUTPUT or NULL */
  int n = 0;           /* number of tokens after redirections are removed */

  for (int i = 0; i < ntokens; i++) {
    /* TODO: Handle tokens and open files as requested. */
#ifdef STUDENT
    // Search for INPUT and OUTPUT tokens
    if (token[i] == T_INPUT) {
      i++;
      // After INPUT token there is token
      // containing file name that can be open with Read Only flag
      (*inputp) = Open(token[i], O_RDONLY, 0);
      // Continue so neither INPUT token nor token with file will be saved
      continue;
    }
    if (token[i] == T_OUTPUT) {
      i++;
      // After OUTPUT token there is token
      // containing file name that can be open with Write Only flag
      (*outputp) = Open(token[i], O_WRONLY, 0);
      // Continue so neither OUTPUT token nor token with file will be saved
      continue;
    }
    // if it is normal token save it
    token[n++] = token[i];
    (void)mode;

#endif /* !STUDENT */
  }

  token[n] = NULL;
  return n;
}

/* Execute internal command within shell's process or execute external command
 * in a subprocess. External command can be run in the background. */
static int do_job(token_t *token, int ntokens, bool bg) {
  int input = -1, output = -1;
  int exitcode = 0;

  ntokens = do_redir(token, ntokens, &input, &output);

  if (!bg) {
    if ((exitcode = builtin_command(token)) >= 0)
      return exitcode;
  }

  sigset_t mask;
  Sigprocmask(SIG_BLOCK, &sigchld_mask, &mask);

  /* TODO: Start a subprocess, create a job and monitor it. */
#ifdef STUDENT
  // starting subprocess that will execve to execute external command
  pid_t pid = Fork();
  if (pid == 0) {
    // child
    setpgid(pid, pid); // sets pgid to pid

    // If it is FG job give terminal access to child
    if (bg == FG) {
      setfgpgrp(getpid());
    }

    // If there was input/output redirection update in and out filedescriptors
    if (input != -1) {
      dup2(input, STDIN_FILENO);
      MaybeClose(&input);
    }
    if (output != -1) {
      dup2(output, STDOUT_FILENO);
      MaybeClose(&output);
    }

    // Unblock signals since shell blocked it at the beginning
    Signal(SIGINT, SIG_DFL);
    Signal(SIGTSTP, SIG_DFL);
    Signal(SIGTTIN, SIG_DFL);
    Signal(SIGTTOU, SIG_DFL);
    Signal(SIGCHLD, SIG_DFL);

    // execute external command
    external_command(token);
  } else {
    // parent
    // sets pgid of child to it's pid. It is crucial to do it in both child and
    // parent
    setpgid(pid, pid);
    // add job and process inside new allocated job
    int job_idx = addjob(pid, bg);
    addproc(job_idx, pid, token);

    // close potencial file descriptors related to redirections
    MaybeClose(&input);
    MaybeClose(&output);

    // if it is a FG job give terminal access to child, and monitor the job
    if (bg == FG) {
      setfgpgrp(pid);
      exitcode = monitorjob(&mask);
    } else {
      msg("[%d] running '%s'\n", job_idx, jobcmd(job_idx));
    }
  }

#endif /* !STUDENT */

  Sigprocmask(SIG_SETMASK, &mask, NULL);
  return exitcode;
}

/* Start internal or external command in a subprocess that belongs to pipeline.
 * All subprocesses in pipeline must belong to the same process group. */
static pid_t do_stage(pid_t pgid, sigset_t *mask, int input, int output,
                      token_t *token, int ntokens, bool bg) {
  ntokens = do_redir(token, ntokens, &input, &output);

  if (ntokens == 0)
    app_error("ERROR: Command line is not well formed!");

  /* TODO: Start a subprocess and make sure it's moved to a process group. */
  pid_t pid = Fork();
#ifdef STUDENT
  // Set pgid of pid to given pgid, so every process in pipeline has the same
  // pgid
  setpgid(pid, pgid);
  pgid = getpgid(pid);
  if (pid == 0) {
    // child
    // If it is FG job give terminal access to child
    if (bg == FG) {
      setfgpgrp(pgid);
    }

    // If there was input/output redirection update in and out filedescriptors
    if (input != -1) {
      Dup2(input, STDIN_FILENO);
      MaybeClose(&input);
    }
    if (output != -1) {
      Dup2(output, STDOUT_FILENO);
      MaybeClose(&output);
    }

    // Unblock signals since shell blocked it at the beginning
    Signal(SIGINT, SIG_DFL);
    Signal(SIGTSTP, SIG_DFL);
    Signal(SIGTTIN, SIG_DFL);
    Signal(SIGTTOU, SIG_DFL);
    Signal(SIGCHLD, SIG_DFL);

    // execute external command
    external_command(token);
  } else {
    // parent
    // If it is FG job give terminal access to child
    if (bg == FG)
      setfgpgrp(pgid);

    // close unnecessary file descriptors
    MaybeClose(&input);
    MaybeClose(&output);
  }

#endif /* !STUDENT */

  return pid;
}

static void mkpipe(int *readp, int *writep) {
  int fds[2];
  Pipe(fds);
  fcntl(fds[0], F_SETFD, FD_CLOEXEC);
  fcntl(fds[1], F_SETFD, FD_CLOEXEC);
  *readp = fds[0];
  *writep = fds[1];
}

/* Pipeline execution creates a multiprocess job. Both internal and external
 * commands are executed in subprocesses. */
static int do_pipeline(token_t *token, int ntokens, bool bg) {
  pid_t pid, pgid = 0;
  int job = -1;
  int exitcode = 0;

  int input = -1, output = -1, next_input = -1;

  mkpipe(&next_input, &output);

  sigset_t mask;
  Sigprocmask(SIG_BLOCK, &sigchld_mask, &mask);

  /* TODO: Start pipeline subprocesses, create a job and monitor it.
   * Remember to close unused pipe ends! */
#ifdef STUDENT
  // remember start of proccess command in last_token
  int last_token = 0;
  for (int i = 0; i < ntokens; i++) {
    // Search for PIPE token
    if (token[i] == T_PIPE) {
      // start subprocess that will execute command which starts at last token
      // and ends on i-th token
      pid = do_stage(pgid, &mask, input, output, &token[last_token],
                     i - last_token, bg);
      // If it was first part of pipe, then initialize pgid and job
      if (pgid == 0) {
        pgid = pid;
        job = addjob(pgid, bg);
      }
      // add process to job
      addproc(job, pid, &token[last_token]);

      last_token = i + 1;

      input = next_input;
      // Prepare for next pipe
      mkpipe(&next_input, &output);
    }
  }

  MaybeClose(&next_input);
  MaybeClose(&output);

  // need to execute last part of pipe since, there is no PIPE token at the end
  // of tokens
  pid = do_stage(pgid, &mask, input, -1, &token[last_token],
                 ntokens - last_token, bg);
  addproc(job, pid, &token[last_token]);

  // if it is a FG job, monitor it
  if (bg == FG) {
    exitcode = monitorjob(&mask);
  } else {
    msg("[%d] running '%s'\n", job, jobcmd(job));
  }
#endif /* !STUDENT */

  Sigprocmask(SIG_SETMASK, &mask, NULL);
  return exitcode;
}

static bool is_pipeline(token_t *token, int ntokens) {
  for (int i = 0; i < ntokens; i++)
    if (token[i] == T_PIPE)
      return true;
  return false;
}

static void eval(char *cmdline) {
  bool bg = false;
  int ntokens;
  token_t *token = tokenize(cmdline, &ntokens);

  if (ntokens > 0 && token[ntokens - 1] == T_BGJOB) {
    token[--ntokens] = NULL;
    bg = true;
  }

  if (ntokens > 0) {
    if (is_pipeline(token, ntokens)) {
      do_pipeline(token, ntokens, bg);
    } else {
      do_job(token, ntokens, bg);
    }
  }

  free(token);
}

#ifndef READLINE
static char *readline(const char *prompt) {
  static char line[MAXLINE]; /* `readline` is clearly not reentrant! */

  write(STDOUT_FILENO, prompt, strlen(prompt));

  line[0] = '\0';

  ssize_t nread = read(STDIN_FILENO, line, MAXLINE);
  if (nread < 0) {
    if (errno != EINTR)
      unix_error("Read error");
    msg("\n");
  } else if (nread == 0) {
    return NULL; /* EOF */
  } else {
    if (line[nread - 1] == '\n')
      line[nread - 1] = '\0';
  }

  return strdup(line);
}
#endif

int main(int argc, char *argv[]) {
  /* `stdin` should be attached to terminal running in canonical mode */
  if (!isatty(STDIN_FILENO))
    app_error("ERROR: Shell can run only in interactive mode!");

#ifdef READLINE
  rl_initialize();
#endif

  sigemptyset(&sigchld_mask);
  sigaddset(&sigchld_mask, SIGCHLD);

  if (getsid(0) != getpgid(0))
    Setpgid(0, 0);

  initjobs();

  struct sigaction act = {
    .sa_handler = sigint_handler,
    .sa_flags = 0, /* without SA_RESTART read() will return EINTR */
  };
  Sigaction(SIGINT, &act, NULL);

  Signal(SIGTSTP, SIG_IGN);
  Signal(SIGTTIN, SIG_IGN);
  Signal(SIGTTOU, SIG_IGN);

  while (true) {
    char *line = readline("# ");

    if (line == NULL)
      break;

    if (strlen(line)) {
#ifdef READLINE
      add_history(line);
#endif
      eval(line);
    }
    free(line);
    watchjobs(FINISHED);
  }

  msg("\n");
  shutdownjobs();

  return 0;
}
