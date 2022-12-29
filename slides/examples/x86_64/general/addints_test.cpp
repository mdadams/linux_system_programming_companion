#include <cassert>

int add(int a0, int a1, int a2, int a3, int a4, int a5, int a6, int a7,
  int a8, int a9);
int do_add();

int main() {
	assert(do_add() == add(1, 2, 3, 4, 5, -5, -4, -3, -2, -1));
}
