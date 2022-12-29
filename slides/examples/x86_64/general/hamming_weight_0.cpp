unsigned int hamming_weight(unsigned int n) {
	unsigned int count = 0;
	for (; n; count += n & 1, n >>= 1) {}
	return count;
}
