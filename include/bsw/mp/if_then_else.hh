//
// Created by igor on 06/06/2021.
//

#ifndef BSW_MP_IF_THEN_ELSE_HH
#define BSW_MP_IF_THEN_ELSE_HH

namespace bsw::mp {
  template <bool COND, typename YES, typename NO>
  struct if_then_else;
  /// @cond HIDDEN_SYMBOLS
  template <typename YES, typename NO>
  struct if_then_else<true, YES, NO> {
    using type = YES;
  };

  template <typename YES, typename NO>
  struct if_then_else<false, YES, NO> {
    using type = NO;
  };

  template <bool COND, typename YES, typename NO>
  using if_then_else_t = typename if_then_else<COND, YES, NO>::type;
}

#endif //BSW_IF_THEN_ELSE_HH
