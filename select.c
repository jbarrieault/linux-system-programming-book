#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/select.h>

int main()
{
  fd_set read_fds;
  struct timeval timeout;
  int result;

  FD_ZERO(&read_fds);
  FD_SET(STDIN_FILENO, &read_fds);

  timeout.tv_sec = 5;
  timeout.tv_usec = 0;

  printf("Waiting for input (5 seconds timouet)...\n");

  result = select(STDIN_FILENO + 1, &read_fds, NULL, NULL, &timeout);

  if (result == -1)
  {
    perror("select error");
  }
  else if (result == 0)
  {
    printf("Timed out, no data available\n");
  }
  else
  {
    if (FD_ISSET(STDIN_FILENO, &read_fds))
    {
      printf("Data is available! Reading...\n");
      char buffer[256];
      fgets(buffer, sizeof(buffer), stdin);
      printf("we read the data: %s", buffer);
    }
  }

  return 0;
}
