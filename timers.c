#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// This program showcases the POSIX timer API.
// Note: MacOs does not support, use Linux!

void signal_handler(int signo)
{
  if (signo == SIGUSR1)
  {
    printf("SIGUSR1 signal received!\n");
  }
}

int main()
{
  timer_t timer;
  int ret;

  // evp is short for "Event Parameter"
  // it allows you to specify the timer's behavior.
  // timer_t can either emit a specificed signal,
  // spawn a thread and execute a given fuctnion,
  // or do nothing.
  struct sigevent evp;

  evp.sigev_value.sival_ptr = &timer;
  // tell the timer to send a signal, SIGUSR1, when it expires.
  evp.sigev_notify = SIGEV_SIGNAL;
  evp.sigev_signo = SIGUSR1;

  // create a timer, it is not armed yet
  ret = timer_create(CLOCK_REALTIME, &evp, &timer);
  if (ret)
  {
    perror("time_create");
    exit(1);
  }

  // register handler for the signal we told the timer to send
  signal(SIGUSR1, signal_handler);

  struct itimerspec ts;

  // initial timer time
  ts.it_value.tv_sec = 5;
  ts.it_value.tv_nsec = 0;

  // after the initial timer, the timer will continue
  // at this interval
  ts.it_interval.tv_sec = 3;
  ts.it_interval.tv_nsec = 0;

  // if given a pointer to an itimerspec as the final arg,
  // timer_settime() will set it to the given timer's old itimerspec
  struct itimerspec old_ts;

  int no_flags = 0;

  // arm the timer!
  ret = timer_settime(timer, 0, &ts, &old_ts);
  if (ret)
  {
    perror("timer_settime");
    exit(1);
  }
  printf("timer armed! T-minus 5 seconds...\n");

  sleep(2);

  struct itimerspec rem;
  ret = timer_gettime(timer, &rem);
  if (ret)
  {
    perror("timer_gettime");
  }
  printf("Seconds remaining: %d\n", rem.it_value.tv_sec);

  // wait for the timer's signal
  pause();
  printf("The timer will now sound at the subsequent interval of 3s\n");
  pause();
  pause();
  pause();

  // TODO: timer_getoverrun
  // block the SIGUSR1 signal,
  // sleep for multime interval lengths
  // wake, get & print the the overrun
}
