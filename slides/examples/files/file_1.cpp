#include <iostream>
#include <fcntl.h>
#include <unistd.h>
#include "unique_fd.hpp"

int main() {
	unique_fd fd(open("/etc/passwd", O_RDONLY));
	if (!fd) {
		std::cerr << "open failed\n";
		return 1;
	}
	// ... (use fd)
	/* when destructor for fd invoked, any open file descriptor
	   associated with fd is closed */
}
