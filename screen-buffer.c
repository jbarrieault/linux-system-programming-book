#include <stdio.h>
#include <unistd.h>

int main()
{
  // this clears (creates empty lines and scrolls) the main terminal screen buffer
  // \x1b[?1049h is an ANSI escape code that switches the terminal to the alternate screen buffer.
  // \x1b[H moves the cursor to the home position (top left corner)
  // printf("\x1b[?1049h\x1b[H");

  // instead, let's use an entirely alternate screen buffer.
  // here's the escape sequence for that:
  printf("\033[?1049h\033[H");

  // i noticed in terminal (vs iterm2), you can still easily scroll up and see prior terminal output.
  // iterm2 doesn't do this.
  // but even with commands like bat, that's possible in terminal.
  // it's a little wonky.
  printf("this is an alternate screen buffer, wow!\n\n");
  printf("You can press the enter key to exit.");

  getchar(); // wait for user input

  // this exits the alternate screen buffer, restoring original terminal content
  printf("\x1b[?1049l");

  return 0;
}
