#include <format>
#include <iostream>
#include <vector>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int exitstatus(int wstatus)
  {return (WIFEXITED(wstatus) ? WEXITSTATUS(wstatus) : -1);}

int main() {
	constexpr int num_children = 8;
	std::vector<pid_t> child_pids;
	for (int i = 0; i < num_children; ++i) {
		if (pid_t child_pid = fork(); child_pid > 0) {
			child_pids.push_back(child_pid);
		} else if (child_pid == 0) {
			std::cout << std::format("[child {}] sleeping {}\n", i, i);
			sleep(i);
			std::cout << std::format("[child {}] exiting\n", i);
			std::exit(i);
		} else {
			std::cerr << "fork failed\n";
		}
	}
	for (int i = 0; i < num_children; ++i) {
		int status;
		if (waitpid(child_pids[num_children - i], &status, 0) < 0)
		  {std::cerr << "wait failed\n";}
		std::cout << std::format("[parent] child {} exit status: {}\n", i,
		  exitstatus(status));
	}
	std::cout << "[parent] exiting\n";
}
