void foo(int& i, int& j, int& k);

int bar(int a, int b, int c) {
	foo(a, b, c);
	return a + b + c;
}
