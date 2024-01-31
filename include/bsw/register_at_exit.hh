#ifndef BSW_REGISTER_AT_EXIT_HH
#define BSW_REGISTER_AT_EXIT_HH

#include <functional>
#include <queue>
#include <cstdlib>
#include <utility>

namespace {
  using queue_t = std::queue<std::function<void ()>>;
  queue_t* callbacks = nullptr;

  struct queue_init {

    queue_init () {
      if (!callbacks) {
        callbacks = new queue_t;
        std::atexit ([] () {
          if (callbacks) {
            while (!callbacks->empty ()) {
              try {
                callbacks->front () ();
              }
              catch (...) {

              }
              callbacks->pop ();
            }
            delete callbacks;
          }
        });
      }
    }

    void add (std::function<void ()> f) {
      callbacks->push (f);
    }
  };

}

namespace bsw {
  inline
  void register_at_exit (std::function<void ()> callback) {
    static queue_init static_initializer;
    static_initializer.add (std::move(callback));
  }

  template <typename T>
  void delete_at_exit (T* ptr) {
    register_at_exit ([ptr] () { delete ptr; });
  }
}

#endif
