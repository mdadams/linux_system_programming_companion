class mutex {
public:
	mutex() : m_(0) {}
	mutex(const mutex&) = delete;
	mutex& operator=(const mutex&) = delete;
	void lock() noexcept;
	bool try_lock() noexcept;
	void unlock() noexcept;
private:
	static int atomic_cas(int* addr, int expected, int desired) noexcept;
	static void atomic_store(int* addr, int value) noexcept;
	int m_;
	static_assert(__atomic_always_lock_free(sizeof(int), 0));
};
