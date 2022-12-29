#include <chrono>
#include <format>
#include <iostream>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char** argv) {
	if (argc < 2) {std::cerr << "no program specified\n"; return 1;}
	if (pid_t child_pid = fork(); child_pid > 0) {
		struct user_regs_struct regs;
		unsigned long long count = 0;
		auto start_time = std::chrono::high_resolution_clock::now();
		for (;;) {
			int status;
			if (waitpid(child_pid, &status, 0) < 0) {return 1;}
			if (WIFEXITED(status)) {break;}
			if (ptrace(PTRACE_GETREGS, child_pid, nullptr, &regs) < 0) {return 1;}
			std::cerr << std::format("rip: {:#x}\n", regs.rip);
			if (ptrace(PTRACE_SINGLESTEP, child_pid, nullptr, nullptr) < 0) {return 1;}
			++count;
		}
		double elapsed_time = std::chrono::duration<double>(
		  std::chrono::high_resolution_clock::now() - start_time).count();
		std::cerr << std::format("instruction count: {}\n"
		  "elapsed time: {}\n"
		  "instructions/second: {}\n",
		  count, elapsed_time, count / elapsed_time);
	} else if (child_pid == 0) {
		if (ptrace(PTRACE_TRACEME, 0, nullptr, nullptr) < 0) {return 1;}
		if (execve(argv[1], &argv[1], environ)) {return 1;}
	} else {return 1;}
}
