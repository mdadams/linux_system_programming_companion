#include <linux/futex.h>
#include <syscall.h>
#include <sys/time.h>
#include <unistd.h>

inline int futex_wait_private(int* addr, int expected,
  struct timespec* timeout) noexcept {
	return syscall(SYS_futex, addr, FUTEX_WAIT_PRIVATE, expected, timeout,
	  nullptr, 0);
}

inline int futex_wake_private(int* addr, int count) noexcept {
	return syscall(SYS_futex, addr, FUTEX_WAKE_PRIVATE, count, nullptr,
	  nullptr, 0);
}
