#include <thread>
#include <memory>

#include "bsw/logger/active_object.hh"
#include "bsw/logger/logger.hh"

namespace mt
{
    // the constructor just launches some amount of workers
    active_object::active_object(size_t threads)
        :
        workers(threads),
        stop(false)
    {
        auto svc = [this]()
        {
            for (;;)
            {
                std::function<void()> task;

                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    this->condition.wait(lock,

                        [this] { return this->stop || !this->tasks.empty(); }
                    );
                    if (this->stop && this->tasks.empty())
                    {
                        return;
                    }
                    task = std::move(this->tasks.front());
                    this->tasks.pop();
                }
                try
                {
                    task();
                }
                catch (std::exception& e)
                {
                    EVLOG_TRACE(EVLOG_FATAL, "Exception: ", e.what());
                }
                catch (...)
                {
                    EVLOG_TRACE(EVLOG_FATAL, "Exception occured");
                }
            }
        };
        for (size_t i = 0; i < threads; i++)
        {
            workers[i] = std::make_unique <std::thread>(svc);
        }
    }
    // =================================================================================
    // the destructor joins all threads
    active_object::~active_object()
    {
        try
        {
            shutdown();
            join();
        }
        catch (...)
        {

        }
    }
    // ----------------------------------------------------------------------------------
    void active_object::shutdown()
    {
        {

            std::unique_lock<std::mutex> lock(queue_mutex);
            if (stop)
            {
                return;
            }
            stop = true;
        }
        condition.notify_all();
    }
    // ----------------------------------------------------------------------------------
    void active_object::join()
    {
        for (const auto& worker : workers)
        {
            if (worker->joinable())
            {
                worker->join();
            }
        }
    }
    // ----------------------------------------------------------------------------------
    bool active_object::active() const
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        return !stop;
    }
} // ns mt

