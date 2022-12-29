#include <cassert>
#include <format>
#include <iostream>
#include <sys/types.h>
#include <unistd.h>

int main() {
	pid_t parent_pid = getpid();
	std::cout << std::format("[parent] PID: {}\n", parent_pid);
	if (pid_t child_pid = fork(); child_pid > 0) {
		// parent
		assert(getpid() == parent_pid);
		std::cout << std::format("[parent] PID of child: {}\n", child_pid);
	} else if (child_pid == 0) {
		// child
		assert(getpid() != parent_pid);
		std::cout << std::format("[child] PID: {}\n", getpid());
		// note: if parent already terminated prior to getppid
		// call, this call will not return parent_pid
		pid_t ppid = getppid();
		std::cout << std::format("[child] PID of parent: {} ({})\n",
		  ppid, ppid == parent_pid ? "creator" : "reaper");
		std::exit(0);
	} else {
		std::cout << "fork failed\n";
	}
}
