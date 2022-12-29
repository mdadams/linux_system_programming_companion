#include "futex.hpp"
#include "mutex2.hpp"

inline int mutex::atomic_cas(int* addr, int expected, int desired)
  noexcept {
	__atomic_compare_exchange_n(addr, &expected, desired, false,
	  __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE); // GCC/Clang
	return expected; // return initial value of *ptr
}

inline int mutex::atomic_dec(int* ptr) noexcept {
	 return __atomic_sub_fetch (ptr, 1, __ATOMIC_RELEASE) + 1; // GCC/Clang
}

void mutex::lock() noexcept {
	if (int c = atomic_cas(&m_, 0, 1); c != 0) {
		do {
			if (c == 2 || atomic_cas(&m_, 1, 2) != 0)
				futex_wait_private(&m_, 2, nullptr);
		} while ((c = atomic_cas(&m_, 0, 2)) != 0);
	}
}

void mutex::unlock() noexcept {
	if (atomic_dec(&m_) != 1) {
		m_ = 0;
		futex_wake_private(&m_, 1);
	}
}
