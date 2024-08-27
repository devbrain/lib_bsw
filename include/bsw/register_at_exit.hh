#ifndef BSW_REGISTER_AT_EXIT_HH
#define BSW_REGISTER_AT_EXIT_HH

#include <functional>
#include <bsw/export.h>

namespace bsw {
  BSW_EXPORT void register_at_exit (std::function<void ()> callback);

  BSW_EXPORT void cleanup_registered_at_exit ();

  template <typename T>
  void delete_at_exit (T* ptr) {
    register_at_exit ([ptr] () { delete ptr; });
  }
}

#endif
