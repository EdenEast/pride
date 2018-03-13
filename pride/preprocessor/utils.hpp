
#pragma once

#define PRI_PP_EMPTY()
#define PRI_PP_DOT() .
#define PRI_PP_COMMA() ,
#define PRI_PP_DEFER(x) x PRI_PP_EMPTY()
#define PRI_PP_OBSTRUCT(...) __VA_ARGS__ PRI_PP_DEFER(PRI_PP_EMPTY)()
#define PRI_PP_EXPAND(...) __VA_ARGS__
#define PRI_PP_EAT(...)

#define PRI_PP_RAW(...) __VA_ARGS__

#define PRI_PP_IMPL_TOSTR(...) #__VA_ARGS__
#define PRI_PP_TOSTR(...) PRI_PP_IMPL_TOSTR(__VA_ARGS__)

#define PRI_PP_IMPL_CAT_2(m0, m1) m0##m1
#define PRI_PP_IMPL_CAT_3(m0, m1, m2) m0##m1##m2
#define PRI_PP_IMPL_CAT_4(m0, m1, m2, m3) m0##m1##m2##m3
#define PRI_PP_IMPL_CAT_5(m0, m1, m2, m3, m4) m0##m1##m2##m3##m4

#define PRI_PP_CAT_2(m0, m1) PRI_PP_IMPL_CAT_2(m0, m1)
#define PRI_PP_CAT_3(m0, m1, m2) PRI_PP_IMPL_CAT_3(m0, m1, m2)
#define PRI_PP_CAT_4(m0, m1, m2, m3) PRI_PP_IMPL_CAT_4(m0, m1, m2, m3)
#define PRI_PP_CAT_5(m0, m1, m2, m3, m4) PRI_PP_IMPL_CAT_5(m0, m1, m2, m3, m4)

#define PRI_PP_ANOYN_VARIABLE(str) PRI_PP_CAT_2(str, __COUNTER__)
