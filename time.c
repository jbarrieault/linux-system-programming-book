#include <stdio.h>
#include <time.h>
#include <sys/time.h>

int main()
{
  // Real arithmetic type capable of representing times.
  // Although not defined by the C standard,
  // this is almost always an integral value holding the number of seconds
  // (not counting leap seconds) since 00:00, Jan 1 1970 UTC,
  time_t time_t_time;

  // time() both returns a t_time respresenting the current time,
  // as well as stores it in the provided time_t pointer, if one is given.
  time_t_time = time(NULL);

  printf("%jd seconds since midnight Jan 1 1970 UTC (excluding leap seconds)\n", time_t_time);

  char *ctime_str = ctime(&time_t_time);
  printf("Current time ctime(): %s", ctime_str); // Automatically formatted as "Wed Sep 27 21:40:00 2023\n"

  // tm is a structure holding a calendar date and time broken down into its components.
  // ie tm_yday, tm_year, tm_mon, tm_mday, tm_hour, etc.
  struct tm *tm_time;

  // localtime() converts a time_t to a tm (using pointers for arg and return value)
  // it is NOT reentrant, because it uses a static local buffer for its result.
  // So multiple calling threads can race.
  tm_time = localtime(&time_t_time);

  // localtime_r() is the reentrant version of localtime.
  // is is thread-safe because the result is placed in a buffer the caller provides.
  localtime_r(&time_t_time, tm_time);

  if (tm_time == NULL)
  {
    perror("localtime_r");
  }
  // print individual broken down time parts
  printf("tm_time, years since 1900: %d\n", tm_time->tm_year);
  printf("tm_time, months since jan: %d\n", tm_time->tm_mon);
  printf("tm_time, day of month: %d\n", tm_time->tm_mday);

  // format & print the time using strftime()
  char tm_buff[80];
  strftime(tm_buff, sizeof(tm_buff), "%Y-%m-%d %H:%M:%S", tm_time);
  printf("tm_time formatted: %s\n", tm_buff);

  // timespec is a struct used to represent an amount of time using seconds & nanoseconds
  struct timespec ts_nice_time;
  ts_nice_time.tv_sec = 420;
  ts_nice_time.tv_nsec = 69;
  printf("nice timespec: %ld seconds and %ld nanoseconds\n", ts_nice_time.tv_sec, ts_nice_time.tv_nsec);

  // clock_gettime() retrieves the time from a given clock id,
  // placing it in the provided timespec.
  struct timespec ts_realtime;
  clock_gettime(CLOCK_REALTIME, &ts_realtime);
  printf("CLOCK_REALTIME timespec: %ld seconds and %ld nanoseconds since epoch\n", ts_realtime.tv_sec, ts_realtime.tv_nsec);

  // There are other clocks! Let's check the monotonic clock.
  struct timespec ts_monotonic;
  clock_gettime(CLOCK_MONOTONIC, &ts_monotonic);
  printf("CLOCK_MONOTONIC timespec (1st): %ld seconds and %ld nanoseconds since...boot? /shug\n", ts_monotonic.tv_sec, ts_monotonic.tv_nsec);
  clock_gettime(CLOCK_MONOTONIC, &ts_monotonic);
  printf("CLOCK_MONOTONIC timespec (2nd): %ld seconds and %ld nanoseconds since...boot? /shug\n", ts_monotonic.tv_sec, ts_monotonic.tv_nsec);

  // clock_getres() retrieves the resolution of a given clock id.
  // resolution refers to the smallest interval the clock can accurately measure.
  struct timespec clock_res;
  if (clock_getres(CLOCK_MONOTONIC, &clock_res) == 0)
  {
    printf("CLOCK_MONOTONIC's resolution: %ld seconds, %ld nanoseconds\n", clock_res.tv_sec, clock_res.tv_nsec);
  }
  else
  {
    perror("clock_getres");
  }

  // sys/time.h's timval struct is similar to timespec, but with microsecond precision.
  struct timeval tv;

  // get time of day (seconds + microseconds since the unix epoch)
  gettimeofday(&tv, NULL);
  printf("gettimeofday: %ld seconds and %d microseconds since epoch\n", tv.tv_sec, tv.tv_usec);
}
