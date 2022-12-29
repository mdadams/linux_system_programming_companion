#include <format>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>

int main() {
	std::cout << std::format(
	  "PID: {}; "
	  "parent PID: {}; "
	  "process group ID: {}; "
	  "session ID: {}\n",
	  getpid(), getppid(), getpgrp(), getsid(0));
}
