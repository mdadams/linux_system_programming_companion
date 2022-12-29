#include <format>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
	if (pid_t child_pid = fork(); child_pid > 0) {
		// parent
		int status;
		if (wait(&status) < 0) {std::cerr << "wait failed\n";}
		  // or equivalently, waitpid(-1, &status, 0)
		if (WIFEXITED(status)) {
			std::cout << std::format("[parent] child exit status: {}\n",
			  WEXITSTATUS(status));
		} else {std::cout << "[parent] unexpected child state change\n";}
	} else if (child_pid == 0) {
		// child
		std::cout << "[child] sleeping\n";
		sleep(2);
		std::cout << "[child] exiting\n";
		std::exit(0);
	} else {
		std::cerr << "fork failed\n";
	}
}
