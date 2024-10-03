#include <stdio.h>
#include <unistd.h>
#define _POSIX_C_SOURCE 199309
#include <time.h>
#include <errno.h>

int main()
{
  printf("sleeping 2 seconds...\n");
  int seconds_not_slept = sleep(2);
  printf("I'm awake again\n");
  printf("seconds not slept: %d\n", seconds_not_slept);

  // sleep() returns the number of seconds NOT slept
  // One scenario in which the the full sleep did not occur is when the process is woken by a signa

  // one way to ensure you sleep the full amount is to re-sleep until you have no remainder
  unsigned int to_sleep = 3;
  printf("sleeping 3 seconds, even if it takes multiple sleep() calls...\n");
  while ((to_sleep = sleep(to_sleep)))
    ;

  // second-level precision not enough for you? How about microseconds
  // (Linux has deprecated usleep)
  unsigned long micro_seconds = 250;
  printf("sleeping 250 microseconds...\n");
  printf("that was quick.\n");

  // how about nano seconds?
  // nanosleep() has a different signature!
  struct timespec req;
  struct timespec rem;

  req.tv_sec = 2;
  req.tv_nsec = 250000000;

  printf("sleeping 2s 250,000,000ns using nanosleep()\n");
  if (nanosleep(&req, &rem) == -1)
  {
    perror("nanosleep");
    if (errno == EINTR)
    {
      // we were interruped
      printf("nanosleep() was interrupted, leaving remaining sleep time: %lds %ldns\n", rem.tv_sec, rem.tv_nsec);
    }
  }

  // clock_nanosleep() (Linux only) allows you to provide a time source (clock) to measure the sleep against.
  // it can be used to sleep a relative length of time, or alternatively,
  // until an absolute time on the given clock.
  //
  struct timespec ts = {.tv_sec = 1, .tv_nsec = 250000000};
  int ret;

  // relative sleep (1.25s)
  printf("sleeping 1.25s on the monotonic clock\n");
  ret = clock_nanosleep(CLOCK_MONOTONIC, 0, &ts, NULL);
  if (ret)
  {
    perror("clock_nanosleep");
  }
  printf("done\n");

  // to sleep until an absolute time, pass the TIMER_ABSTIME flag
  // struct timespec ts;
  ret = clock_gettime(CLOCK_MONOTONIC, &ts);
  if (ret)
  {
    perror("clock_gettime");
    exit(1);
  }

  ts.tv_sec += 1;
  printf("Sleeping until %lds %ldns after epoch\n");
  ret = clock_nanosleep(CLOCK_MONOTONIC, TIMER_ABSTIME, &ts, NULL);
  if (ret)
  {
    perror("clock_nanocleep");
  }
  printf("Done.\n");
}
