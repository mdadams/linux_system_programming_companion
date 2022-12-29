class ticketlock {
public:
	ticketlock() : ticket_(0), current_(0) {}
	ticketlock(const ticketlock&) = delete;
	ticketlock& operator=(const ticketlock&) = delete;
	void lock() noexcept {
		unsigned int ticket = atomic_fetch_and_inc(ticket_);
		while (atomic_load(current_) != ticket) {}
	}
	void unlock() noexcept {
		atomic_fetch_and_inc(current_);
	}
private:
	static unsigned int atomic_fetch_and_inc(unsigned int& x) noexcept {
		return __atomic_fetch_add(&x, 1, __ATOMIC_ACQ_REL); // GCC/Clang
	}
	static unsigned int atomic_load(unsigned int& x) noexcept {
		return __atomic_load_n(&x, __ATOMIC_ACQUIRE); // GCC/Clang
	}
	static constexpr int cacheline_size = 128;
	alignas(cacheline_size) unsigned int ticket_;
	alignas(cacheline_size) unsigned int current_;
	static_assert(__atomic_always_lock_free(sizeof(unsigned int), 0));
};
