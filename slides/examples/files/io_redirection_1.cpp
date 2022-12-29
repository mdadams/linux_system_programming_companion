#include <cassert>
#include <format>
#include <iostream>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char** argv) {
	if (argc < 4) {std::cerr << "invalid usage\n"; std::exit(1);}
	int stdin_fd = open(argv[1], O_RDONLY);
	if (stdin_fd < 0) {std::cerr << "cannot open input\n"; return 1;}
	int stdout_fd = open(argv[2], O_CREAT | O_TRUNC | O_WRONLY, S_IRUSR | S_IWUSR);
	if (stdin_fd < 0) {std::cerr << "cannot open output\n"; return 1;}
	if (pid_t child_pid = fork(); child_pid > 0) {
		int status;
		if (wait(&status) < 0) {std::cerr << "wait failed\n";}
		std::cout << std::format("child exit status {}\n",
		  (WIFEXITED(status) ? WEXITSTATUS(status) : -1));
	} else if (child_pid == 0) {
		close(0);
		dup2(stdin_fd, 0);
		close(1);
		dup2(stdout_fd, 1);
		if (execve(argv[3], &argv[3], environ) < 0)
		  {std::cerr << "exec failed\n"; std::exit(1);}
	} else {std::cerr << "fork failed\n"; std::exit(1);}
}
