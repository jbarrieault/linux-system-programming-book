#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>

int main()
{
  int fd;
  int ret;
  char *filename = "pread_test_file.txt";

  fd = open(filename, O_RDONLY);
  if (fd == -1)
  {
    perror("open error");

    printf("Error code: %d\n", errno);

    return 1;
  }

  printf("reading pread_test_file.txt...");

  char buffer[256];
  ret = pread(fd, buffer, sizeof(buffer), 0);

  if (ret == -1)
  {
    perror("pread error. did you create pread_test_file.txt?\n");
  }
  else if (ret == 0)
  {
    printf("idk what this means actually\n");
  }
  else
  {
    printf("read data: %s\n", buffer);
  }

  close(fd);

  return 0;
}
