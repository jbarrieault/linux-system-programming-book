#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
int main()
{
  int fd;
  int ret;
  char *filename = "pwrite_test.txt";

  fd = open(filename, O_RDWR | O_CREAT | O_TRUNC, 0644);
  if (fd == -1)
  {
    perror("open error\n");
    return 1;
  }

  printf("let's write to pwrite_test.txt\n");

  const char *data = "hello worldy\n";
  ret = pwrite(fd, data, strlen(data), 0);
  if (ret == -1)
  {
    perror("pwrite error\n");
    close(fd);
    return 1;
  }

  printf("I think we read it. But go check for yourself...\n");

  close(fd);
  return 0;
}
