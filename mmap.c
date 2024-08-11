#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

int main(int argc, char *argv[])
{
  if (argc < 2)
  {
    fprintf(stderr, "Usage: %s <file>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  const char *filepath = argv[1];
  int fd = open(filepath, O_RDONLY);
  if (fd == -1)
  {
    perror("Error opening file");
    exit(EXIT_FAILURE);
  }

  struct stat sb;
  if (fstat(fd, &sb) == -1)
  {
    perror("Error getting file size");
    close(fd);
    exit(EXIT_FAILURE);
  }

  printf("File descriptor: %d\n", fd);
  printf("File size: %ld bytes\n", (long)sb.st_size);

  if (sb.st_size == 0)
  {
    fprintf(stderr, "Cannot map an empty file.\n");
    close(fd);
    exit(EXIT_FAILURE);
  }

  // Get the system's page size, so we can align reads to pages
  long page_size = sysconf(_SC_PAGESIZE);
  if (page_size == -1)
  {
    perror("Error getting page size");
    close(fd);
    exit(EXIT_FAILURE);
  }

  printf("Page size: %ld bytes\n", page_size);

  char *mapped = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
  if (mapped == MAP_FAILED)
  {
    fprintf(stderr, "Error mapping file: %s\n", strerror(errno));
    close(fd);
    exit(EXIT_FAILURE);
  }

  // write(STDOUT_FILENO, mapped, sb.st_size);

  // read from the map in page size chunks.
  // i think this is pointless with mmap? not sure.
  off_t offset;
  for (offset = 0; offset < sb.st_size; offset += page_size)
  {
    size_t chunk_size = page_size;
    if (offset + page_size > sb.st_size)
    {
      chunk_size = sb.st_size - offset; // adjust final chunk
    }

    write(STDERR_FILENO, mapped + offset, chunk_size);
  }

  if (munmap(mapped, sb.st_size) == -1)
  {
    perror("Error unmapping memory");
    // why no exit(EXIT_FAILURE) here?
  }

  close(fd);
  return 0;
}
