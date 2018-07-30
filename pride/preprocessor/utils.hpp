
#pragma once

#define PRIDE_PP_EMPTY()
#define PRIDE_PP_DOT() .
#define PRIDE_PP_COMMA() ,
#define PRIDE_PP_DEFER(x) x PRIDE_PP_EMPTY()
#define PRIDE_PP_OBSTRUCT(...) __VA_ARGS__ PRIDE_PP_DEFER(PRIDE_PP_EMPTY)()
#define PRIDE_PP_EXPAND(...) __VA_ARGS__
#define PRIDE_PP_EAT(...)

#define PRIDE_PP_RAW(...) __VA_ARGS__

#define PRIDE_PP_IMPL_TOSTR(...) #__VA_ARGS__
#define PRIDE_PP_TOSTR(...) PRIDE_PP_IMPL_TOSTR(__VA_ARGS__)

#define PRIDE_PP_IMPL_CAT_2(m0, m1) m0##m1
#define PRIDE_PP_IMPL_CAT_3(m0, m1, m2) m0##m1##m2
#define PRIDE_PP_IMPL_CAT_4(m0, m1, m2, m3) m0##m1##m2##m3
#define PRIDE_PP_IMPL_CAT_5(m0, m1, m2, m3, m4) m0##m1##m2##m3##m4

#define PRIDE_PP_CAT_2(m0, m1) PRIDE_PP_IMPL_CAT_2(m0, m1)
#define PRIDE_PP_CAT_3(m0, m1, m2) PRIDE_PP_IMPL_CAT_3(m0, m1, m2)
#define PRIDE_PP_CAT_4(m0, m1, m2, m3) PRIDE_PP_IMPL_CAT_4(m0, m1, m2, m3)
#define PRIDE_PP_CAT_5(m0, m1, m2, m3, m4) PRIDE_PP_IMPL_CAT_5(m0, m1, m2, m3, m4)

#define PRIDE_PP_ANOYN_VARIABLE(str) PRIDE_PP_CAT_2(str, __COUNTER__)
