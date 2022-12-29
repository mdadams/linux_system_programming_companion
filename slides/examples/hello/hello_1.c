#include <stdio.h>

int main() {
	printf("Hello, World!\n");
	return fflush(stdout) ? 1 : 0;
}
