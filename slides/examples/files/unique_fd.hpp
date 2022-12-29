#include "unique_handle.hpp"

struct fd_uh_policy {
	using handle_type = int;
	static void free(handle_type h) {close(h);}
	static bool is_null(handle_type h) {return h < 0;}
	static handle_type get_null() {return -1;}
};

using unique_fd = unique_handle<fd_uh_policy>;
