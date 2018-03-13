
#pragma once

#include "utils.hpp"

#define PRI_PP_IMPL_BOOL(mx) PRI_PP_IMPL_BOOL_##mx
#define PRI_PP_BOOL(mx) PRI_PP_IMPL_BOOL(mx)

#define PRI_PP_IMPL_BITNOT_0 1
#define PRI_PP_IMPL_BITNOT_1 0

#define PRI_PP_IMPL_BITAND_00 0
#define PRI_PP_IMPL_BITAND_01 0
#define PRI_PP_IMPL_BITAND_10 0
#define PRI_PP_IMPL_BITAND_11 1

#define PRI_PP_IMPL_BITOR_00 0
#define PRI_PP_IMPL_BITOR_01 1
#define PRI_PP_IMPL_BITOR_10 1
#define PRI_PP_IMPL_BITOR_11 1

#define PRI_PP_IMPL_BITNOR_00 1
#define PRI_PP_IMPL_BITNOR_01 0
#define PRI_PP_IMPL_BITNOR_10 0
#define PRI_PP_IMPL_BITNOR_11 0

#define PRI_PP_IMPL_BITXOR_00 0
#define PRI_PP_IMPL_BITXOR_01 1
#define PRI_PP_IMPL_BITXOR_10 1
#define PRI_PP_IMPL_BITXOR_11 0


#define PRI_PP_BITNOT(mx) \
    PRI_PP_CAT_2(PRI_PP_IMPL_BITNOT_, PRI_PP_DEFER(mx))

#define PRI_PP_BITAND(m0, m1) \
    PRI_PP_CAT_3(PRI_PP_IMPL_BITAND_, PRI_PP_DEFER(m0), PRI_PP_DEFER(m1))

#define PRI_PP_BITOR(m0, m1) \
    PRI_PP_CAT_3(PRI_PP_IMPL_BITOR_, PRI_PP_DEFER(m0), PRI_PP_DEFER(m1))

#define PRI_PP_BITNOR(m0, m1) \
    PRI_PP_CAT_3(PRI_PP_IMPL_BITNOR_, PRI_PP_DEFER(m0), PRI_PP_DEFER(m1))

#define PRI_PP_BITXOR(m0, m1) \
    PRI_PP_CAT_3(PRI_PP_IMPL_BITXOR_, PRI_PP_DEFER(m0), PRI_PP_DEFER(m1))

#define PRI_PP_IMPL_NOT(mx) PRI_PP_BITNOT(PRI_PP_BOOL(mx))
#define PRI_PP_IMPL_AND(m0, m1) PRI_PP_BITAND(PRI_PP_BOOL(m0), PRI_PP_BOOL(m1))
#define PRI_PP_IMPL_OR(m0, m1) PRI_PP_BITOR(PRI_PP_BOOL(m0), PRI_PP_BOOL(m1))
#define PRI_PP_IMPL_NOR(m0, m1) PRI_PP_BITNOR(PRI_PP_BOOL(m0), PRI_PP_BOOL(m1))
#define PRI_PP_IMPL_XOR(m0, m1) PRI_PP_BITXOR(PRI_PP_BOOL(m0), PRI_PP_BOOL(m1))

#define PRI_PP_NOT(mx) PRI_PP_IMPL_NOT(mx)
#define PRI_PP_AND(m0, m1) PRI_PP_IMPL_AND(m0, m1)
#define PRI_PP_OR(m0, m1) PRI_PP_IMPL_OR(m0, m1)
#define PRI_PP_NOR(m0, m1) PRI_PP_IMPL_NOR(m0, m1)
#define PRI_PP_XOR(m0, m1) PRI_PP_IMPL_XOR(m0, m1)

#define PRI_PP_IMPL_IF_0(mTrue, mFalse) mFalse
#define PRI_PP_IMPL_IF_1(mTrue, mFalse) mTrue
#define PRI_PP_IF(cond, mTrue, mFalse) \
    PRI_PP_CAT_2(PRI_PP_IMPL_IF_, PRI_PP_BOOL(cond))(mTrue, mFalse)

#define PRI_PP_COMMA_IF(cond) PRI_PP_IF(cond, PRI_PP_COMMA, PRI_PP_EMPTY)()
