#include <atomic>
#include <format>
#include <iostream>
#include <signal.h>
#include <unistd.h>
#include "sigsegv_lib.hpp"

std::atomic<unsigned int> sigsegv_count(0);

void sigsegv_handler(int sig_no, siginfo_t* siginfo, void* context) {
	if (siginfo->si_code == SI_USER) {return;}
	if (!is_safe_read(context, siginfo->si_addr)) {std::abort();}
	safe_read_fail(context);
	++sigsegv_count;
}

int main() {
	long page_size = sysconf(_SC_PAGE_SIZE);
	struct sigaction sa;
	sa.sa_sigaction = &sigsegv_handler;
	sigfillset(&sa.sa_mask);
	sa.sa_flags = SA_SIGINFO;
	if (sigaction(SIGSEGV, &sa, 0)) {std::abort();}
	uintptr_t addr = 0;
	int c;
	for (;; addr += page_size) {
		if ((c = safe_read(reinterpret_cast<void*>(addr))) >= 0) {break;}
		std::cout << std::format("read failed {:#x}\n", addr);
	}
	std::cout << std::format("read success {:#x} {}\n", addr, c);
	std::cout << std::format("number of faults: {}\n",
	  static_cast<unsigned int>(sigsegv_count));
}
