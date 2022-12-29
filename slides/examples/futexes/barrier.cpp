#include <climits>
#include "barrier.hpp"
#include "futex.hpp"

void barrier::wait() noexcept {
	m_.lock();
	if (count_-- > 1) {
		unsigned int e = event_;
		m_.unlock();
		do {
			futex_wait_private(reinterpret_cast<int*>(&event_), e, nullptr);
		} while (event_ == e);
	} else {
		++event_;
		count_ = threshold_;
		futex_wake_private(reinterpret_cast<int*>(&event_), INT_MAX);
		m_.unlock();
	}
}
