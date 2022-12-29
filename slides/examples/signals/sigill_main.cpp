#include <atomic>
#include <format>
#include <cassert>
#include <iostream>
#include <signal.h>
#include <sys/ucontext.h>
#include <unistd.h>
#include "sigill_lib.hpp"

std::atomic<unsigned int> sigill_count(0);

void sigill_handler(int sig_no, siginfo_t* siginfo, void* context) {
	void* ip = get_ip(context);
	int length = get_ins_length(ip);
	if (length < 0) {abort();}
	set_ip(context, static_cast<unsigned char*>(ip) + length);
	++sigill_count;
}

int main() {
	struct sigaction sa;
	sa.sa_sigaction = &sigill_handler;
	sigfillset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	if (sigaction(SIGILL, &sa, 0)) {abort();}
	for (int i = 0; i < 3; ++i) {
		illegal_instruction(i);
		std::cout << std::format("{}\n",
		  static_cast<unsigned int>(sigill_count));
	}
}
