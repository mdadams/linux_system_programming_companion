#include <cstddef>
#include <new>
#include <limits>
#include <sys/mman.h>
#include <unistd.h>

template <class T> struct mmap_allocator {
	using value_type = T;
	mmap_allocator() noexcept {}
	template <class U> mmap_allocator(const mmap_allocator<U>&) noexcept {}
	T* allocate(std::size_t n) const;
	void deallocate(T* p, std::size_t n) const noexcept;
	template <class U> bool operator==(const mmap_allocator<U>&)
	  const noexcept {return true;}
	template <class U> bool operator!=(const mmap_allocator<U>&)
	  const noexcept {return false;}
};

template <class T>
T* mmap_allocator<T>::allocate(std::size_t n) const {
	if (!n) {return nullptr;}
	if (n > std::numeric_limits<std::size_t>::max() / sizeof(T))
	  {throw std::bad_array_new_length();}
	void* ptr = mmap(nullptr, n * sizeof(T),
	  PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (ptr == MAP_FAILED) {throw std::bad_alloc();}
	return static_cast<T*>(ptr);
}

template <class T>
void mmap_allocator<T>::deallocate(T* p, std::size_t n) const noexcept {
	if (!n) {return;}
	munmap(p, n * sizeof(T));
}
