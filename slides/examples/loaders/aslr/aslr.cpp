#include <iostream>
#include <iomanip>
#include <cstdint>

int var = 42;

int func() {return 42;}

int main() {
	int i;
	std::cout << std::hex << std::setfill('0')
	  << " addr(var): " << std::setw(16)
	  << reinterpret_cast<uintptr_t*>(&var) << '\n'
	  << "addr(func): " << std::setw(16)
	  << reinterpret_cast<uintptr_t>(func) << '\n'
	  << "   addr(i): " << std::setw(16)
	  << reinterpret_cast<uintptr_t>(&i) << '\n'
	  << "addr(var) - addr(func): " << std::setw(16)
	  << reinterpret_cast<uintptr_t>(&var) -
	  reinterpret_cast<uintptr_t>(&func) << '\n';
}
