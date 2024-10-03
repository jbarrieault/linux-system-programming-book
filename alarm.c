#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void sigalrm_handler(int sig)
{
  if (sig == SIGALRM)
  {
    printf("SIGALRM received!\n");
  }
}

int main()
{
  signal(SIGALRM, sigalrm_handler);

  // alarm() sends a SIGALRM to the invoking process after the given number of seconds
  printf("alarming after 3 seconds...\n");
  alarm(3);

  // wait for a signal
  pause();
}
