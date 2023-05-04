//
// Created by igor on 01/07/2020.
//

#ifndef BSW_SINGLETON_HH
#define BSW_SINGLETON_HH
#include <bsw/register_at_exit.hh>

namespace bsw {
  template <class T>
  class singleton {
    public:
      static T& instance () {
        static holder initializer;
        return *initializer.object;
      }

    private:
      struct holder {
        T* object;

        holder () {
          object = new T;
          delete_at_exit (object);
        }
      };
  };
}

#endif
