#ifndef BSW_LOGGER_DEVICE_HH
#define BSW_LOGGER_DEVICE_HH

#include "bsw/logger/predicate.hh"
#include "bsw/logger/logger_fwd.hh"


namespace logger
{
    class basic_device
    {
    public:
        basic_device(predicate_ptr_t predicate);

        virtual ~basic_device();

        bool open();
        bool print(record_ptr_t record);
        void close();

        bool is_opened() const;
    protected:
        predicate_ptr_t  _predicate() const;
        bool             _filter(const record_ptr_t record);
    protected:
        virtual bool     _do_open() = 0;
        virtual bool     _do_print(record_ptr_t record) = 0;
        virtual void     _do_close() = 0;
    private:
#include <bsw/warn/push.hh>
#include <bsw/warn/dll_interface>
        predicate_ptr_t m_predicate;
#include <bsw/warn/pop.hh>
        bool            m_is_opened;
    };
}

#endif 
