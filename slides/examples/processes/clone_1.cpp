#include <atomic>
#include <cassert>
#include <format>
#include <iostream>
#include <linux/futex.h>
#include <sched.h>
#include <syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

pid_t gettid() {return syscall(SYS_gettid);}

int futex(int *uaddr, int futex_op, int val, const struct timespec *timeout,
  int *uaddr2, int val3)
{return syscall(SYS_futex, uaddr, futex_op, val, timeout, uaddr2, val3);}

int atomic_load_int(int* ptr) {
	// WARNING: This is ugly and non-portable.
	static_assert(sizeof(std::atomic<int>) == sizeof(int));
	return reinterpret_cast<std::atomic<int>*>(ptr)->load();
}

struct child_args {
	bool make_thread; // invoker creating thread (as opposed to process)
	pid_t tid; // TID of invoker of clone
	pid_t pid; // PID of invoker of clone
	pid_t ppid; // PPID of invoker of clone
	int ret; // return value (used in thread case)
};

int child_func(void* arg) {
	child_args* args = static_cast<child_args*>(arg);
	assert(args->tid != gettid()); // not invoker of clone
	if (args->make_thread) {
		std::cerr << std::format("[thread] PID {}\n", getpid());
		assert(args->pid == getpid()); // running in same process
		assert(args->ppid == getppid()); // has same parent process
		assert(gettid() != getpid()); // not main thread of process
	} else {
		std::cerr << std::format("[process] PID {}\n", getpid());
		assert(args->pid != getpid()); // running in different process
		assert(args->pid == getppid()); // child of invoker of clone
		assert(gettid() == getpid()); // main thread of process
	}
	sleep(2);
	std::cout << "Hello, World!\n" << std::flush;
	args->ret = std::cout ? 0 : 1;
	return args->ret;
}

int main(int argc, char** argv) {
	bool make_thread = argc < 2;
	std::cerr << std::format("[main] PID {}\n", getpid());
	constexpr std::size_t stack_size = 64 * 1024;
	static char stack[stack_size];
	int clone_flags = (make_thread) ? (CLONE_THREAD | CLONE_SIGHAND |
	  CLONE_VM | CLONE_FILES | CLONE_FS | CLONE_CHILD_CLEARTID |
	  CLONE_PARENT_SETTID) : SIGCHLD;
	alignas(std::atomic<int>) int tid;
	child_args arg = {make_thread, gettid(), getpid(), getppid(), -1};
	pid_t child_tid;
	if ((child_tid = clone(child_func, stack + stack_size, clone_flags, &arg,
	 &tid, 0, &tid)) < 0)
	  {std::cerr << "clone failed\n"; return 1;}
	int exit_status;
	if (make_thread) {
		int tmp_tid;
		while ((tmp_tid = atomic_load_int(&tid)) == child_tid) {
			std::cerr << "[main] sleeping\n";
			int ret;
			while ((ret = futex(&tid, FUTEX_WAIT, tmp_tid, nullptr, 0, 0)) < 0
			  && ret == EAGAIN) {}
			if (ret < 0) {std::cerr << "futex failed\n"; return 1;}
			std::cerr << "[main] awoken\n";
		}
		exit_status = arg.ret;
	} else {
		int status;
		if (waitpid(child_tid, &status, 0) != child_tid)
		  {std::cerr << "wait failed\n"; return 1;}
		exit_status = (WIFEXITED(status)) ? WEXITSTATUS(status) : 1;
	}
	std::cerr << std::format("[main] exit status {}\n", exit_status);
}
