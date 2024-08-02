#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <poll.h>

int main()
{
  struct pollfd fds[1];
  int timeout_msecs = 5000;
  int ret;

  fds[0].fd = STDIN_FILENO;
  fds[0].events = POLLIN;

  printf("waiting for input (5 second timeout)\n");

  ret = poll(fds, 1, timeout_msecs);

  if (ret == -1)
  {
    perror("poll error\n");
  }
  else if (ret == 0)
  {
    printf("timed out, no data\n");
  }
  else
  {
    if (fds[0].revents & POLLIN)
    {
      printf("data available! reading...\n");
      char buffer[256];
      fgets(buffer, sizeof(buffer), stdin);
      printf("read data: %s\n", buffer);
    }
  }

  return 0;
}
