#include <format>
#include <iostream>
#include <signal.h>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
	long x = -1;
	long y = -1;
	if (pid_t child_pid = fork(); child_pid > 0) {
		int status;
		if (waitpid(child_pid, &status, 0) != child_pid) {return 1;}
		if (!WIFSTOPPED(status)) {return 1;}
		if (ptrace(PTRACE_SETOPTIONS, child_pid, 0, PTRACE_O_EXITKILL))
		  {return 1;}
		errno = 0;
		long w = ptrace(PTRACE_PEEKDATA, child_pid, &x, 0);
		if (w == -1 && errno) {return 1;}
		if (ptrace(PTRACE_POKEDATA, child_pid, &y, w)) {return 1;}
		if (ptrace(PTRACE_CONT, child_pid, 0, 0)) {return 1;}
	} else if (child_pid == 0) {
		x = 42;
		y = 0;
		if (ptrace(PTRACE_TRACEME, 0, 0, 0)) {return 1;}
		if (raise(SIGSTOP)) {return 1;}
		std::cout << std::format("value is {}\n", y);
	} else {return 1;}
}
