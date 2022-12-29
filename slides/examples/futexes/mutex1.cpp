#include "futex.hpp"
#include "mutex1.hpp"

inline int mutex::atomic_cas(int* addr, int expected, int desired)
  noexcept {
	__atomic_compare_exchange_n(addr, &expected, desired, false,
	  __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE); // GCC/Clang
	return expected; // return initial value of *ptr
}

inline void mutex::atomic_store(int* addr, int value) noexcept {
	__atomic_store(addr, &value, __ATOMIC_RELEASE); // GCC/Clang
}

void mutex::lock() noexcept {
	while (atomic_cas(&m_, 0, 1) != 0) {futex_wait_private(&m_, 1, nullptr);}
}

bool mutex::try_lock() noexcept {return atomic_cas(&m_, 0, 1) == 0;}

void mutex::unlock() noexcept {
	atomic_store(&m_, 0);
	futex_wake_private(&m_, 1);
}
