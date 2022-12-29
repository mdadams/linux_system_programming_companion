#include <atomic>
#include <format>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <thread>
#include <typeinfo>
#include <vector>
#include <boost/thread/latch.hpp>
#include "util.hpp"

template <class Mutex>
class mutex_tester {
public:
	using mutex = Mutex;
	int main(int argc, char** argv);
private:
	void worker(int instance, boost::latch& start_latch);
	unsigned long long count_ = 0;
	std::vector<unsigned long long> per_thread_counts_;
	std::vector<unsigned long long> max_diffs_;
	std::vector<double> mean_diffs_;
	mutable mutex m_;
	std::atomic<unsigned long long> remaining_;
};

template<class Integer>
Integer dec_if_not_zero(std::atomic<Integer>& count) noexcept
{
	Integer c;
	for (;;) {
		c = count.load();
		if (!c) {
			break;
		}
		if (std::atomic_compare_exchange_strong(&count, &c, c - 1)) {
			break;
		}
	}
	return c;
}

template <class Mutex>
void mutex_tester<Mutex>::worker(int instance, boost::latch& start_latch) {
	int verbose = 0;
	unsigned long long max_diff = 0;
	double sum_diff = 0.0;
	unsigned long long num_iters = 0;
	start_latch.count_down_and_wait();
	for (; dec_if_not_zero(remaining_) != 0; ++num_iters) {
		if (verbose >= 2) {
			//print(std::cout, "{tester ", gettid(), ' ', "lock", "}\n");
			std::cout << std::format("{{tester {} lock}}\n", gettid());
		}
		unsigned long long pre_lock_count = remaining_.load();
		m_.lock();
		unsigned long long post_lock_count = remaining_.load();
		unsigned long long diff = pre_lock_count - post_lock_count;
		max_diff = std::max(max_diff, diff);
		sum_diff += diff;
		++count_;
		++per_thread_counts_[instance];
		if (verbose >= 1) {
			//print(std::cout, "{tester ", gettid(), ' ', "count ", count_, "}\n");
			std::cout << std::format("{{tester {} count {}}}\n", gettid(),
			  count_);
		}
#if 0
		if (verbose >= 0) {
			print(std::cout, ".");
		}
#endif
		if (verbose >= 2) {
			std::cout << std::format("{{tester {} unlock}}\n", gettid());
		}
		m_.unlock();
	}
	max_diffs_[instance] = max_diff;
	mean_diffs_[instance] = static_cast<double>(sum_diff) / num_iters;
}

template <class Mutex>
int mutex_tester<Mutex>::main(int argc, char** argv) {
	int num_threads = 4;
	int num_iters = 10'000;
	if (argc >= 3) {
		num_threads = atoi(argv[1]);
		num_iters = atoi(argv[2]);
	}
	std::cerr << std::format("type name: {}\n", typeid(mutex).name());
	std::vector<std::jthread> t;
	per_thread_counts_ = std::vector<unsigned long long>(num_threads, 0);
	max_diffs_ = std::vector<unsigned long long>(num_threads, 0);
	mean_diffs_ = std::vector<double>(num_threads, 0.0);
	remaining_ = num_threads * num_iters;
	boost::latch start_latch(num_threads);
	for (int i = 0; i < num_threads; ++i) {
		t.emplace_back([this, i, num_iters, &start_latch](){worker(i, start_latch);});
	}
	for (auto&& i : t) {i.join();}
	std::cout << "per thread counts:\n";
	for (int i = 0; i < num_threads; ++i) {
		std::cout << std::format("    {} {}\n", i, per_thread_counts_[i]);
	}
	unsigned long long total = std::accumulate(per_thread_counts_.begin(),
	  per_thread_counts_.end(), 0ULL);
	std::cout << "total count: " << total << '\n';
	std::cout << "max diffs:\n";
	for (int i = 0; i < num_threads; ++i) {
		std::cout << std::format("    {} {}\n", i, max_diffs_[i]);
	}
	std::cout << "avg diffs:\n";
	for (int i = 0; i < num_threads; ++i) {
		std::cout << std::format("    {} {}\n", i, mean_diffs_[i]);
	}
	return 0;
}
