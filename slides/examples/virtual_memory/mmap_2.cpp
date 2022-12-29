#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <sys/mman.h>
#include <sys/wait.h>
#include <unistd.h>

int main() {
	long page_size = sysconf(_SC_PAGE_SIZE);
	void* ptr = mmap(nullptr, page_size,
	  PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	if (ptr == MAP_FAILED) {std::cerr << "mmap failed\n"; return 1;}
	char* cptr = static_cast<char*>(ptr);
	assert(*cptr == '\0');
	if (int child_pid = fork(); child_pid > 0) {
		int status;
		if (waitpid(child_pid, &status, 0) < 0)
		  {std::cerr << "wait failed\n"; return 1;}
		if (!(WIFEXITED(status) && WEXITSTATUS(status) == 0))
		  {std::cerr << "child failed\n"; return 1;}
		std::cerr << cptr;
	} else if (child_pid == 0) {
		std::string hello("Hello, World!\n");
		std::copy(hello.begin(), hello.end(), cptr);
	} else {std::cerr << "fork failed\n"; return 1;}
}
