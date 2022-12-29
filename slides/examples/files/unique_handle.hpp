#include <utility>

template<typename Policy>
class unique_handle {
public:
	using handle_type = typename Policy::handle_type;
	unique_handle() : h_(Policy::get_null()) {}
	unique_handle(handle_type handle) : h_(handle) {}
	unique_handle(unique_handle&& other) noexcept
	  {h_ = other.h_; other.h_ = Policy::get_null();}
	unique_handle& operator=(unique_handle&& other) noexcept
	  {reset(); h_ = other.h_; other.h_ = Policy::get_null(); return *this;}
	unique_handle(const unique_handle&) = delete;
	unique_handle& operator=(unique_handle&) = delete;
	~unique_handle() {reset();}
	handle_type get() const {return h_;}
	explicit operator bool() const {return !Policy::is_null(h_);}
	void reset(handle_type new_handle = Policy::get_null()) {
		handle_type old_handle = h_;
		h_ = new_handle;
		if(!Policy::is_null(old_handle)) {Policy::free(old_handle);}
	}
	void swap(unique_handle& other)
	  {using std::swap; swap(h_, other.h_);}
private:
	handle_type h_;
};
