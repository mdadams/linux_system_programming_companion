#include <format>
#include <iostream>
#include <map>
#include <stdexcept>
#include <sys/ptrace.h>
#include <sys/types.h>
#include <sys/user.h>
#include <sys/wait.h>
#include <syscall.h>
#include <unistd.h>
#include "syscall_names.hpp"

void parent(int child_pid) {
	std::map<long, size_t> syscall_counts;
	int status;
	if (waitpid(child_pid, &status, 0) < 0) {throw std::runtime_error("waitpid failed");}
	if (!WIFSTOPPED(status)) {throw std::runtime_error("unexpected tracee state");}
	if (ptrace(PTRACE_SETOPTIONS, child_pid, 0, PTRACE_O_EXITKILL))
	  {throw std::runtime_error("ptrace failed");}
	for (;;) {
		if (ptrace(PTRACE_SYSCALL, child_pid, 0, 0) < 0)
		  {throw std::runtime_error("ptrace failed");}
		if (waitpid(child_pid, &status, 0) < 0)
		  {throw std::runtime_error("waitpid failed");}
		if (!WIFSTOPPED(status)) {throw std::runtime_error("unexpected tracee state");}
		struct user_regs_struct regs;
		if (ptrace(PTRACE_GETREGS, child_pid, 0, &regs) < 0)
		  {throw std::runtime_error("cannot get registers");}
		long syscall = regs.orig_rax;
		syscall_counts[syscall]++;
		std::cout << std::format("entering syscall {}\n", syscall_names[syscall]);
		if (ptrace(PTRACE_SYSCALL, child_pid, 0, 0) < 0)
		  {throw std::runtime_error("ptrace failed");}
		if (waitpid(child_pid, &status, 0) < 0)
		  {throw std::runtime_error("waitpid failed");}
		if (WIFEXITED(status)) {break;}
		if (!WIFSTOPPED(status)) {throw std::runtime_error("unexpected tracee state");}
	}
	std::cout << "total system call counts:\n";
	for (auto [k, v] : syscall_counts)
	  {std::cout << std::format("{:9d} {:s}\n", v, syscall_names[k]);}
}

void child(int argc, char** argv) {
	ptrace(PTRACE_TRACEME, 0, 0, 0);
	if (execve(argv[1], &argv[1], environ) < 0)
	  {throw std::runtime_error("execve failed");}
}

int main(int argc, char** argv) try {
	if (argc <= 1) {throw std::runtime_error("invalid usage");}
	if (pid_t pid = fork(); pid > 0) {parent(pid);}
	else if (pid == 0) {child(argc, argv);}
	else {throw std::runtime_error("fork failed");}
} catch(const std::exception& e) {
	std::cerr << "fatal error: " << e.what() << '\n';
}
