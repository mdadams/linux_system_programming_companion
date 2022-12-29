#include <cassert>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char** argv, char** envp) {
	if (argc < 2) {std::cerr << "invalid usage\n"; std::exit(1);}
	if (pid_t child_pid = fork(); child_pid > 0) {
		// parent
		int status;
		if (wait(&status) < 0) {std::cerr << "wait failed\n";}
	} else if (child_pid == 0) {
		// child
		if (execve(argv[1], &argv[1], envp) < 0) {
			std::cerr << "exec failed\n";
			std::exit(1);
		}
	} else {
		std::cerr << "fork failed\n";
		std::exit(1);
	}
}
