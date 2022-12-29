#include <iostream>
#include <fcntl.h>
#include <unistd.h>

int main() {
	int fd = open("/etc/passwd", O_RDONLY);
	if (fd < 0) {
		std::cerr << "open failed\n";
		return 1;
	}
	// ... (use fd)
	close(fd); // note: we might forget to close
}
