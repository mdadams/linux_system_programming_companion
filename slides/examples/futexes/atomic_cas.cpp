int atomic_cas(int& x, int expected, int desired) noexcept {
	__atomic_compare_exchange_n(&x, &expected, desired, false,
	  __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE); // GCC/Clang
	return expected; // return initial value of *ptr
}

void atomic_store(int& x, int value) noexcept {
	__atomic_store(&x, &value, __ATOMIC_RELEASE); // GCC/Clang
}
