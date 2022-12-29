#include <sys/ucontext.h>

extern "C" void illegal_instruction(int);

inline void* get_ip(void* context) {
	ucontext_t* ucontext = static_cast<ucontext_t*>(context);
	return reinterpret_cast<void*>(ucontext->uc_mcontext.gregs[REG_RIP]);
}

inline void set_ip(void* context, void* ip) {
	ucontext_t* ucontext = static_cast<ucontext_t*>(context);
	ucontext->uc_mcontext.gregs[REG_RIP] = reinterpret_cast<unsigned long>(ip);
}

inline int get_ins_length(void* ip) {
	auto p = static_cast<unsigned char*>(ip);
	if (p[0] == 0xf && p[1] == 0xb) {return 2;}
	else if (p[0] == 0x48 && p[1] == 0x0f) {
		if ((p[2] == 0xff && p[3] == 0xc0) || (p[2] == 0xb9 && p[3] == 0xc0))
		  {return 4;}
	}
	return -1;
}
