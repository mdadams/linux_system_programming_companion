#include <sys/ucontext.h>

extern "C" void breakpoint(int);

inline int breakpoint_type(void* ip) {
	auto p = static_cast<unsigned char*>(ip);
	if (p[-1] == 0xcc) {return 0;}
	else if (p[-1] == 0x03 && p[-2] == 0xcd) {return 1;}
	else {return -1;}
}

inline void* get_ip(void* context) {
	ucontext_t* ucontext = static_cast<ucontext_t*>(context);
	return reinterpret_cast<void*>(ucontext->uc_mcontext.gregs[REG_RIP]);
}
