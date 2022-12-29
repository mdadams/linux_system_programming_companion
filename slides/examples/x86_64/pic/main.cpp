#include <sys/syscall.h>

int answer = 42;

extern "C" void exit(int status) {
	/* invoke exit system call (SYS_exit) */
}

extern "C" int main() {
	return answer;
}

extern "C" void _start() {
	exit(main());
}
