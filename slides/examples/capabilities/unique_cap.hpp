#include "unique_handle.hpp"
#include <sys/capability.h>

struct cap_uh_policy {
    using handle_type = cap_t;
    static void free(handle_type h) {cap_free(h);}
    static handle_type get_null() {return nullptr;}
    static bool is_null(handle_type h) {return !h;}
};

using unique_cap = unique_handle<cap_uh_policy>;
