#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

// Send a signal to another process using sigqueue()
// Note, sigqueue() is not available on MacOS.
int main(int argc, char *argv[])
{
  if (argc < 3)
  {
    printf("usage: sigaction pid signalno\n");
    return 1;
  }

  int pid = atoi(argv[1]);
  int signalno = atoi(argv[2]);
  union sigval sival;
  sival.sival_int = 0;

  printf("sending PID %d signo %d\n", pid, signalno);

  sigqueue(pid, signalno, sival);
}
