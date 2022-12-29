#include <atomic>
#include <format>
#include <iostream>
#include <signal.h>
#include <time.h>
#include <unistd.h>

std::atomic<unsigned long> counter;

void handler(int sig_no) {++counter;}

int sleep_with_retry(int seconds) {
	timespec t = {.tv_sec = seconds, .tv_nsec = 0};
	int ret;
	while ((ret = nanosleep(&t, &t)) < 0 && errno == EINTR) {}
	if (ret < 0) {return -1;}
	return 0;
}

int main() {
	if (signal(SIGINT, handler) == SIG_ERR)
		{std::cerr << "signal failed\n"; return 1;}
	for (unsigned long i = 0;; ++i) {
		counter = 0;
		if (int ret = sleep_with_retry(5); ret < 0) {return 1;}
		std::cout << std::format("\nsignal count: {}",
		  static_cast<unsigned long>(counter));
	}
}
