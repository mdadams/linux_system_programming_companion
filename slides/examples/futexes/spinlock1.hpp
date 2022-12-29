class spinlock {
public:
	spinlock() {clear(m_);}
	spinlock(const spinlock&) = delete;
	spinlock& operator=(const spinlock&) = delete;
	void lock() noexcept {
		while (test_and_set(m_)) {}
	}
	bool try_lock() noexcept {
		return !test_and_set(m_);
	}
	void unlock() noexcept {
		clear(m_);
	}
private:
	static bool test_and_set(bool& x) noexcept {
		return __atomic_test_and_set(&x, __ATOMIC_ACQUIRE); // GCC/Clang
	}
	static void clear(bool& x) noexcept {
		__atomic_clear(&x, __ATOMIC_RELEASE); // GCC/Clang
	}
	bool m_;
	static_assert(__atomic_always_lock_free(sizeof(char), 0));
};
