class spinlock {
public:
	spinlock() : m_(0) {}
	spinlock(const spinlock&) = delete;
	spinlock& operator=(const spinlock&) = delete;
	void lock() noexcept {
		while (atomic_cas(&m_, 0, 1) != 0) {}
	}
	bool try_lock() noexcept {
		return atomic_cas(&m_, 0, 1) == 0;
	}
	void unlock() noexcept {
		atomic_store(&m_, 0);
	}
private:
	static int atomic_cas(int* addr, int expected, int desired)
	  noexcept {
		__atomic_compare_exchange_n(addr, &expected, desired, false,
		  __ATOMIC_ACQUIRE, __ATOMIC_ACQUIRE); // GCC/Clang
		return expected; // return initial value of *ptr
	}
	static void atomic_store(int* addr, int value) noexcept {
		__atomic_store(addr, &value, __ATOMIC_RELEASE); // GCC/Clang
	}
	int m_;
	static_assert(__atomic_always_lock_free(sizeof(int), 0));
};
