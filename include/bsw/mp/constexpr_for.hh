//
// Created by igor on 02/06/2021.
//

#ifndef BSW_MP_CONSTEXPR_FOR_HH
#define BSW_MP_CONSTEXPR_FOR_HH

#include <type_traits>

namespace bsw::mp {
  template <auto Start, auto End, auto Inc, class F>
  constexpr void constexpr_for (F&& f) {
    if constexpr (Start < End) {
      f (std::integral_constant<decltype (Start), Start> ());
      constexpr_for<Start + Inc, End, Inc> (f);
    }
  }

  template<typename What, typename ... Args>
  struct is_present {
    static constexpr bool value {(std::is_same_v<What, Args> || ...)};
  };

  template <typename... Ts, typename F>
  constexpr void for_types(F&& f)
  {
    (f.template operator()<const Ts*>((const Ts*) nullptr), ...);
  }
}

#endif
