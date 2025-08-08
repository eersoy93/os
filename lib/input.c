// Input handling implementation
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#include "input.h"

int ReadKey(void)
{
	struct termios oldt, newt;
	int ch;

	// Set terminal to non-canonical mode to read single characters
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);

	// Set stdin to non-blocking mode to avoid blocking on getchar
	fcntl(STDIN_FILENO, F_SETFL, O_NONBLOCK);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, 0);

	// If no character was read, return -1
	if (ch == EOF)
	{
		return -1;
	}
	return ch;
}
