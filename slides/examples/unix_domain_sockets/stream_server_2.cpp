#include <cstring>
#include <iostream>
#include <string>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

ssize_t recv_message(int fd, void* buf, size_t count) {
	struct iovec iov = {.iov_base = buf, .iov_len = count};
	struct msghdr hdr;
	hdr.msg_name = nullptr;
	hdr.msg_namelen = 0;
	hdr.msg_iov = &iov;
	hdr.msg_iovlen = 1;
	hdr.msg_control = nullptr;
	hdr.msg_controllen = 0;
	hdr.msg_flags = 0;
	ssize_t ret = recvmsg(fd, &hdr, 0);
	if (ret >= 0 && (hdr.msg_flags & MSG_TRUNC) != 0) {return -1;}
	return ret;
}

int main(int argc, char** argv) {
	std::string pathname("/tmp/demo");
	if (argc >= 2) {pathname = argv[1];}
	int sfd = socket(AF_UNIX, SOCK_SEQPACKET, 0);
	struct sockaddr_un addr;
	memset(&addr, 0, sizeof(struct sockaddr_un));
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, pathname.c_str(), sizeof(addr.sun_path) - 1);
	if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
	  {std::cerr << "bind failed\n"; return 1;}
	constexpr int bufsize = 64;
	char buf[bufsize];
	if (listen(sfd, 1)) {std::cerr << "listen failed\n"; return 1;}
	for (;;) {
		socklen_t addr_len = sizeof(sockaddr_un);
		int fd = accept(sfd, (struct sockaddr*) &addr, &addr_len);
		if (fd < 0) {std::cerr << "accept failed\n"; return 1;}
		for (;;) {
			int ret;
			if ((ret = recv_message(fd, buf, bufsize)) < 0)
			  {std::cerr << "recv failed\n"; return 1;}
			if (ret == 0) {std::cerr << "EOF\n"; break;}
			buf[ret] = '\0';
			std::cerr << addr_len << ' ' << ret << ' ' << buf << '\n';
			if (!strcmp(buf, "end")) {std::cerr << "END\n"; break;}
			if (!strcmp(buf, "shutdown")) {
				std::cerr << "SHUTDOWN\n";
				close(sfd);
				if (unlink(pathname.c_str()) == -1 && errno != ENOENT)
				  {std::cerr << "unlink failed\n"; return 1;}
			}
		}
		close(fd);
	}
	if (unlink(pathname.c_str()) == -1 && errno != ENOENT)
	  {std::cerr << "unlink failed\n"; return 1;}
}
