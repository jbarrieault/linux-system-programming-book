#include <stdio.h>
#include <stdlib.h>

int main()
{
  FILE *file = fopen("fputc-example.txt", "w+");
  if (file == NULL)
  {
    perror("failed to open fputs-example.txt");
    return 1;
  }

  if (fputc('Y', file) == EOF)
  {
    perror("fputc error for 'Y'\n");
    fclose(file);
    return 1;
  }

  if (fputc('o', file) == EOF)
  {
    perror("fputc error for 'o'\n");
    fclose(file);
    return 1;
  }

  // move file position to start of file
  rewind(file);

  char buffer[3];
  buffer[2] = '\0';

  size_t recordsRead = fread(buffer, sizeof(char), 2, file);
  if (recordsRead < 2)
  {
    if (feof(file))
    {
      printf("End of file reached");
    }
    else if (ferror(file))
    {
      perror("fread error");
    }
    fclose(file);
    return 1;
  }

  printf("Read chars: %s\n", buffer);
  fclose(file);
  return 0;
}
