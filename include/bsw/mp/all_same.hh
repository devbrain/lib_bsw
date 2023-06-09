//
// Created by igor on 31/05/2021.
//

#ifndef BSW_MP_ALL_SAME_HH
#define BSW_MP_ALL_SAME_HH

#include <type_traits>

namespace bsw::mp {
  /**
   * Check a parameter pack for all of type A
   * @tparam A Type to test
   * @tparam Ts Parameter pack
   */
  template <typename A, typename ...Ts>
  inline constexpr bool all_same_v = std::conjunction_v<std::is_same<A, Ts>...>;
}

#endif
