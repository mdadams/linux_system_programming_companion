bool test_and_set(bool& x) noexcept {
	return __atomic_test_and_set(&x, __ATOMIC_ACQUIRE); // GCC/Clang
}

void clear(bool& x) noexcept {
	__atomic_clear(&x, __ATOMIC_RELEASE); // GCC/Clang
}
