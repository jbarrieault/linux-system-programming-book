#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// LINUX ONLY.
#include <sys/inotify.h>

#define BUF_LEN 1024

// This program watches the ./bin/ dir for create/update/destroy file events.
// Once it reads an event, it prints some info, then exits the program.
// After starting, create a file in ./bin !
// $ echo "hello, world\n" >> ./bin/test.txt

int main()
{
  printf("Watching files in ./bin for create, update, an destroy events...\n");

  int fd;
  // initialize inotify, receive fd for it
  fd = inotify_init1(0);
  if (fd == -1)
  {
    perror("inotify_init1");
    exit(EXIT_FAILURE);
  }

  int wd;

  // initialize a watch, watching the relative ./bin dir for create/modify/delete, receive watch descriptor
  wd = inotify_add_watch(fd, "./bin", IN_CREATE | IN_MODIFY | IN_DELETE);
  if (wd == -1)
  {
    perror("inotify_add_watch");
    exit(EXIT_FAILURE);
  }

  // Declares a char array of size BUF_LEN
  // and ensures that the starting memory address of buf is aligned to a 4-byte boundary.
  char buf[BUF_LEN] __attribute__((aligned(4)));

  ssize_t len, i = 0;

  // read BUF_LEN bytes worth of events.
  // this blocks, waiting to receive any of the watched events.
  len = read(fd, buf, BUF_LEN);
  if (len == -1)
  {
    perror("read");
    exit(EXIT_FAILURE);
  }

  while (i < len)
  {
    struct inotify_event *event = (struct inotify_event *)&buf[i];
    printf("wd=%d mask=%d cookie=%d len=%d dir=%s\n",
           event->wd, event->mask, event->cookie, event->len,
           (event->mask & IN_ISDIR) ? "yes" : "no");

    if (event->len)
      printf("name=%s\n", event->name);

    i += sizeof(struct inotify_event) + event->len;
  }
}
