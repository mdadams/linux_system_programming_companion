unsigned long long factorial(unsigned long long n) noexcept {
	unsigned long long result = 1;
	for (; n > 1; result *= n, --n) {}
	return result;
}
