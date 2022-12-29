#include <cassert>
#include <format>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <unistd.h>

int main() {
	pid_t parent_pid = getpid();
	if (pid_t child_pid = fork(); child_pid > 0) {
		// parent
		sleep(5);
		kill(child_pid, SIGKILL);
		int status;
		if (waitpid(child_pid, &status, 0) > 0) {
			if (WIFSIGNALED(status)) {
				std::cout << std::format("child terminated by signal {}\n",
				  WTERMSIG(status));
			}
		}
	} else if (child_pid == 0) {
		// child
		while (true) {std::cout << '.' << std::flush; sleep(1);}
	} else {
		std::cout << "fork failed\n";
	}
}
