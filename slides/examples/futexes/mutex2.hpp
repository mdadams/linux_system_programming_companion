class mutex {
public:
	mutex() : m_(0) {}
	void lock() noexcept;
	void unlock() noexcept;
private:
	static int atomic_cas(int* addr, int expected, int desired) noexcept;
	static int atomic_dec(int* ptr) noexcept;
	int m_;
};
