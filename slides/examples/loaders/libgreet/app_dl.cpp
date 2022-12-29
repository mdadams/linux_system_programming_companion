#include <format>
#include <iostream>
#include <dlfcn.h>
#include <stdlib.h>

int main(void) {

	using greet_func_t = bool (*)(void);

	constexpr char lib[] = "libgreet.so";
	const char *error;
	void *module;
	greet_func_t greet;

	module = dlopen(lib, RTLD_LAZY);
	if (!module) {
		std::cerr << std::format("cannot open {}: {}\n", lib, dlerror());
		return 1;
	}

	dlerror();
	greet = reinterpret_cast<greet_func_t>(dlsym(module, "_Z5greetv"));
	if ((error = dlerror())) {
		std::cerr << std::format("cannot find greet: {}\n", error);
		return 1;
	}

	bool status = greet();

	dlclose(module);
	return status ? 0 : 1;
}
