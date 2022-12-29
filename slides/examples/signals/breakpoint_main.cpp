#include <atomic>
#include <format>
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include "breakpoint_lib.hpp"

std::atomic<int> sigtrap_count(0);
std::atomic<int> break_count(0);

void sigtrap_handler(int sig_no, siginfo_t* siginfo, void* context) {
	if (breakpoint_type(get_ip(context)) >= 0) {++break_count;}
	++sigtrap_count;
}

void print_stats(const char* s) {
	std::cout << std::format("{}: {}/{}\n", s, static_cast<int>(break_count),
	  static_cast<int>(sigtrap_count));
}

int main(int argc, char** argv) {
	struct sigaction sa;
	sa.sa_sigaction = &sigtrap_handler;
	sigfillset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	if (sigaction(SIGTRAP, &sa, 0)) {abort();}
	print_stats("initial values");
	raise(SIGTRAP); print_stats("after sending SIGTRAP");
	breakpoint(0); print_stats("after 1-byte breakpoint");
	breakpoint(1); print_stats("after 2-byte breakpoint");
}
