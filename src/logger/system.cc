#include "bsw/predef.h"
#include "bsw/logger/active_object.hh"

#include "bsw/logger/system.hh"
#include "bsw/logger/device.hh"
#include "bsw/logger/record.hh"
#include "bsw/logger/console.hh"
#include "bsw/logger/debug_device.hh"
#include "bsw/object_manager.hh"

namespace logger
{
    namespace priv
    {
        static device_ptr_t get_default_device()
        {
            device_ptr_t console = console_device();
            if (console->open())
            {
                return console;
            }
#if !defined(NDEBUG)
#if defined(_MSC_VER)
            return debug_console_device();
#else
            return nullptr;
#endif
#else
            return nullptr;
#endif
        }
        // -------------------------------------------------------------------
        struct system_impl_s;
        class system
        {
        public:
            bool add_device(device_ptr_t device);

            void post(record_ptr_t record);

            void shutdown();
            system();
            ~system();

            static system* instance()
            {
                if (!s_instance)
                {
                    s_instance = new system;
                }
                return s_instance;
            }
        private:
            system(const system&) = delete;
            system& operator = (const system&) = delete;
        private:
            system_impl_s * m_pimpl;
        private:
            static system* s_instance;
        };
        system* system::s_instance = nullptr;
    } // ns priv
    typedef priv::system system;
} // ns logger


static bool was_shut_down = false;


namespace logger
{
    namespace priv
    {
        struct system_impl_s
        {
            system_impl_s()
                : m_default_device(get_default_device()),
                m_ao(nullptr)
            {

            }

            ~system_impl_s()
            {
                if (m_ao)
                {
                    delete m_ao;
                }
            }

            void setup_ao()
            {
                if (m_ao == nullptr)
                {
                    m_ao = new mt::active_object(1);
                }
            }

            void shutdown()
            {
                if (!was_shut_down)
                {
                    was_shut_down = true;
                    if (m_ao)
                    {
                        m_ao->shutdown();
                        m_ao->join();
                    }
                }
            }

            void post(record_ptr_t record)
            {
                bool at_least_one_printed = false;
                for (auto dev : m_devices)
                {
                    if (dev->print(record))
                    {
                        at_least_one_printed = true;
                    }
                }

                if (!at_least_one_printed && m_default_device)
                {
                    m_default_device->print(record);
                }
            }
            std::list <device_ptr_t>  m_devices;
            device_ptr_t              m_default_device;
            mt::active_object*        m_ao;
        };

        // ============================================================
        system::system()
        {
            m_pimpl = new system_impl_s;
            core::object_manager::instance().call_on_exit(&logger::shutdown);
        }
        // ------------------------------------------------------------
        system::~system()
        {
            m_pimpl->shutdown();
            delete m_pimpl;
        }
        // -------------------------------------------------------------
        void system::shutdown()
        {
            m_pimpl->shutdown();
            delete s_instance;
            s_instance = nullptr;
        }
        // -------------------------------------------------------------
        bool system::add_device(device_ptr_t device)
        {

            if (!device->is_opened())
            {
                if (!device->open())
                {
                    return false;
                }
            }
            m_pimpl->setup_ao();
            m_pimpl->m_devices.push_back(device);
            return true;
        }
        // -------------------------------------------------------------
        void system::post(record_ptr_t record)
        {
            if (m_pimpl->m_ao)
            {
                m_pimpl->m_ao->enqueue([this](record_ptr_t r) {m_pimpl->post(r); }, record);
            }
            else
            {
                m_pimpl->post(record);
            }
        }
    } // ns priv

    void add_device(device_ptr_t device)
    {
        system::instance()->add_device(device);
    }

    void post(record_ptr_t record)
    {
        system::instance()->post(record);
    }

    void shutdown()
    {
        system::instance()->shutdown();
    }
} // ns logger


