#pragma once

#include <sstream>
#include <syscall.h>
#include <unistd.h>
#include <sys/types.h>

pid_t gettid() {
	return syscall(SYS_gettid);
}

template <class... Ts>
void print(std::ostream& out, const Ts&... args) {
	std::stringstream ss;
	(ss << ... << args);
	out << ss.str();
}
