#ifndef MT_ACTIVE_OBJECT_HH
#define MT_ACTIVE_OBJECT_HH

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include <sstream>



namespace mt
{
    class active_object
    {
    public:
        active_object(size_t num_threads);

        template<class F, class... Args>
        auto enqueue(F&& f, Args&&... args)->std::future<typename std::result_of<F(Args...)>::type>;

        virtual ~active_object();

        virtual void shutdown();
        void join();
        bool active() const;
    private:
        // need to keep track of threads so we can join them
        std::vector< std::unique_ptr <std::thread> > workers;
        // the task queue
        std::queue< std::function<void()> > tasks;

        // synchronization
        mutable std::mutex queue_mutex;
        std::condition_variable condition;
        bool stop;
    };

} // ns mt

// =========================================================================
// Implementation
// =========================================================================

namespace mt
{
    // add new work item to the pool
    template<class F, class... Args>
    auto active_object::enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>
    {
        using return_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared< std::packaged_task<return_type()> >
            (
                std::bind(std::forward<F>(f), std::forward<Args>(args)...)
                );

        std::future<return_type> res = task->get_future();
        {
            std::unique_lock<std::mutex> lock(queue_mutex);

            // don't allow enqueueing after stopping the pool
            if (stop)
            {
                std::ostringstream os;
                os << " enqueue on stopped active object";
                throw std::runtime_error(os.str());
            }

            tasks.push([task]() { (*task)(); });
        }
        condition.notify_one();
        return res;
    }
}

#endif
