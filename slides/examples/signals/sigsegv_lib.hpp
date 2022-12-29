#include <sys/ucontext.h>

extern "C" int safe_read(void* addr);
extern char safe_read_ins;

inline bool is_safe_read(void* context, void* addr) {
	ucontext_t* ucontext = static_cast<ucontext_t*>(context);
	unsigned char* rip = reinterpret_cast<unsigned char*>(
	  ucontext->uc_mcontext.gregs[REG_RIP]);
	return rip == reinterpret_cast<unsigned char*>(&safe_read_ins) &&
	  reinterpret_cast<void*>(ucontext->uc_mcontext.gregs[REG_RDI]) == addr;
}

inline void safe_read_fail(void* context) {
	ucontext_t* ucontext = static_cast<ucontext_t*>(context);
	unsigned char* rip = reinterpret_cast<unsigned char*>(
	  ucontext->uc_mcontext.gregs[REG_RIP]);
	unsigned long rax = ucontext->uc_mcontext.gregs[REG_RAX];
	rip += 2;
	rax = 0xffffffffU;
	ucontext->uc_mcontext.gregs[REG_RIP] = reinterpret_cast<unsigned long>(rip);
	ucontext->uc_mcontext.gregs[REG_RAX] = rax;
}
