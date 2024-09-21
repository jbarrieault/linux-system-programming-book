#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// This program uses sigaction() to register
// a SIGINT signal handler, then waits for that signal.
// kill -SIGINT 123456

void signal_handler(int sig)
{
  printf("Caught signal %d. Exiting.\n", sig);
  exit(0);
}

int main(int argc, char *argv[])
{
  struct sigaction sa;

  sa.sa_handler = signal_handler;
  sa.sa_flags = 0; // not using any flags

  // sa_mask is used to specify signals that are blocked whilst the sa's handler is running.
  // We aren't blocking any signals. Still, explicitly zero-ing out the sa_mask is good practice.
  sigemptyset(&sa.sa_mask);

  // the third argument, oldact, is optional,
  // and can be passed a sigaction pointer, wich will be set
  // to point to the old sa for the given signal.
  // Passing null because we don't need that.
  if (sigaction(SIGINT, &sa, NULL) == -1)
  {
    perror("sigaction error");
    return 1;
  }

  printf("PID: %d\n", getpid());
  printf("Waiting for a SIGINT...\n");

  for (;;)
  {
    // puts the process into interruptible sleep state
    // and tell the kernal it can go find other work.
    // when the kernal has a signal to deliver the process
    // it will wake it up.
    pause();
  }
}
