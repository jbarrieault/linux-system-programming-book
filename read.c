#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
  int fd;
  int ret;

  printf("reading STDIN (blocking)\n");

  char buffer[256];
  ret = read(fd, buffer, sizeof(buffer));

  if (ret == -1)
  {
    perror("read error.\n");
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
