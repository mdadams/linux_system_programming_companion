#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <sys/mman.h>
#include <unistd.h>

int main(int argc, char** argv) {
	long page_size = sysconf(_SC_PAGESIZE);
	char* ptr = static_cast<char*>(
	  std::aligned_alloc(page_size, 4 * page_size));
	std::fill_n(ptr, 4 * page_size, 'A');
	char* none_ptr = ptr;
	char* ro_ptr = ptr + 1 * page_size;
	char* wo_ptr = ptr + 2 * page_size;
	char* rw_ptr = ptr + 3 * page_size;
	if (mprotect(none_ptr, page_size, 0)) {abort();}
	if (mprotect(ro_ptr, page_size, PROT_READ)) {abort();}
	if (mprotect(wo_ptr, page_size, PROT_WRITE)) {abort();}
	if (mprotect(rw_ptr, page_size, PROT_READ | PROT_WRITE)) {abort();}
	char c;
	// c = *none_ptr; // SEGFAULT (cannot read)
	// *none_ptr = 'B'; // SEGFAULT (cannot write)
	c = *ro_ptr; // OK (can read)
	// *ro_ptr = 'B'; // SEGFAULT (cannot write)
	// c = *wo_ptr; // may SEGFAULT (read may be disallowed)
	*wo_ptr = 'B'; // OK (can write)
	c = *rw_ptr; // OK (can read)
	*rw_ptr = 'B'; // OK (can write)
}
