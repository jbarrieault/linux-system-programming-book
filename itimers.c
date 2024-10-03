#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
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
  // inverval timers are like alarm() in that they use signals,
  // but they can also re-arm themselves.
  // you can also specify what _kind_ of time to time (user/kernel/real),
  // which can be useful for profiling.

  struct itimerval itv;
  int ret;

  signal(SIGALRM, sigalrm_handler);

  // it_value is the interval wait when first setting the timer
  itv.it_value.tv_sec = 3;
  itv.it_value.tv_usec = 0;
  // it_interval is subsequent interval waited, after the initial interval
  // after the initial it_value interval, the timer will continue to
  // run indefinitely. it_interval specifies how long that reapating timer interval is.
  itv.it_interval.tv_sec = 1;
  itv.it_interval.tv_usec = 0;

  printf("setting timer for 3 seconds, then repeated intervals of 1 second...\n");
  ret = setitimer(ITIMER_REAL, &itv, NULL);
  if (ret)
  {
    perror("setitimer");
    return 1;
  }

  // pause to wait for the initial interval
  pause();
  // each pause waits for a subsequent interval...
  pause();
  pause();
  pause();
}
