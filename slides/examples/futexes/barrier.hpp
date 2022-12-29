#include "mutex2.hpp"
#include <climits>

class barrier {
public:
	barrier(unsigned int count) : m_(), event_(0), threshold_(count),
	  count_(count) {}
	barrier(const barrier&) = delete;
	barrier& operator=(const barrier&) = delete;
	void wait() noexcept;
private:
	mutex m_;
	unsigned int event_;
	unsigned int threshold_; // total number of threads
	unsigned int count_; // number of threads currently waiting
};
