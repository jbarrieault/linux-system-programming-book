#include <stdio.h>
#include <stdlib.h>

// I was only able to get coredumps on Linux.
// Different/Additional steps are required on Mac OSX.

// remove limit of coredump files (reset to 0 when done testing!)
// ulimit -c unlimited

// Tell the kernal how to name coreedump files by adding the following to /etc/sysctl.conf:
// kernel.core_pattern = /var/crash/core.%e.%p.%t

// apply that config file change:
// sudo sysctl -p

// create the /var/crash directory we told the kernel to put dumps in:
// sudo mkdir -p /var/crash
// sudo chmod 1777

// compile this program with -g flag:
// gcc -g -o segfault-coredump segfault-coredump.c
// running the following program should crash and produce a coredump.

// use gdb to review the dump:
// gdb segfault-coredump /var/crash/core.segfault-coredu.304448.1726450124
// this will stop at the line that caused the segfault.

int main()
{
  int *ptr = NULL;
  printf("Attempting to dereference a null pointer, crash imminent...\b");
  *ptr = 99;

  return 0;
}
