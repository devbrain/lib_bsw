//
// Created by igor on 8/27/24.
//

#include <queue>
#include <cstdlib>
#include <utility>

#include <bsw/register_at_exit.hh>

namespace {
    using queue_t = std::queue <std::function <void ()>>;
    queue_t* callbacks = nullptr;

    struct queue_init {
        queue_init() {
            if (!callbacks) {
                callbacks = new queue_t;
                std::atexit(bsw::cleanup_registered_at_exit);
            }
        }

        void add(std::function <void ()> f) {
            callbacks->push(f);
        }
    };
}

namespace bsw {
    void register_at_exit(std::function <void ()> callback) {
        static queue_init static_initializer;
        static_initializer.add(std::move(callback));
    }

    void cleanup_registered_at_exit() {
        if (callbacks) {
            while (!callbacks->empty()) {
                try {
                    auto cbk = callbacks->front();
                    cbk();
                } catch (...) {
                }
                callbacks->pop();
            }
            delete callbacks;
            callbacks = nullptr;
        }
    }
}
