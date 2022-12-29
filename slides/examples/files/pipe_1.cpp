#include <iostream>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

template <class... Ts> void panic(const Ts&... args)
  {(std::cerr << ... << args) << '\n'; std::exit(255);}

int main(int argc, char** argv) {
	if (argc != 3) {panic("invalid usage");}
	pid_t pids[2];
	int pipe_fds[2];
	if (pipe(pipe_fds)) {panic("pipe failed");}
	pids[0] = fork();
	if (pids[0] == 0) {
		if (close(pipe_fds[0])) {panic("close failed");}
		if (pipe_fds[1] != 1) {
			if (dup2(pipe_fds[1], 1) != 1) {panic("dup2 failed");}
			if (close(pipe_fds[1])) {panic("close failed");}
		}
		char *args[] = {argv[1], nullptr};
		if (execve(argv[1], args, environ)) {panic("exec failed");}
	} else if (pids[0] < 0) {panic("fork failed");}
	if (close(pipe_fds[1])) {panic("close failed");}
	pids[1] = fork();
	if (pids[1] == 0) {
		if (pipe_fds[0] != 0) {
			if (dup2(pipe_fds[0], 0) != 0) {panic("dup2 failed");}
			if (close(pipe_fds[0])) {panic("close failed");}
		}
		char *args[] = {argv[2], nullptr};
		if (execve(argv[2], args, environ)) {panic("exec failed");}
		return 1;
	} else if (pids[1] < 0) {panic("fork failed");}
	if (close(pipe_fds[0])) {panic("closed failed");}
	int status;
	if (waitpid(pids[0], &status, 0) < 0) {std::cerr << "wait failed\n";}
	if (waitpid(pids[1], &status, 0) < 0) {std::cerr << "wait failed\n";}
	return WIFEXITED(status) ? WEXITSTATUS(status) : -1;
}
