#pragma once

/* FORCE CONCAT */
#define M_CONCAT(lhs, rhs) lhs ## rhs
#define MM_CONCAT(lhs, rhs) M_CONCAT(lhs, rhs)

/* ARG_COUNT. Supports [1, 64]. Its' cool enough for this project */
#define ARG_COUNT_IMPL(_1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16, _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, _62, _63, _64, N, ...) N
#define ARG_COUNT(...) ARG_COUNT_IMPL(__VA_ARGS__, 64, 63, 62, 61, 60, 59, 58, 57, 56, 55, 54, 53, 52, 51, 50, 49, 48, 47, 46, 45, 44, 43, 42, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10, 9, 8, 7, 6, 5, 4, 3, 2, 1)

/* ARG_APPLY */
#define ARG_APPLY(M, ...) MM_CONCAT(ARG_APPLY_, ARG_COUNT(__VA_ARGS__))(M, __VA_ARGS__)
#define ARG_APPLY_1(M, _1) M(_1)
#define ARG_APPLY_2(M, _1, _2) M(_1), M(_2)
#define ARG_APPLY_3(M, _1, _2, _3) M(_1), M(_2), M(_3)
#define ARG_APPLY_4(M, _1, _2, _3, _4) M(_1), M(_2), M(_3), M(_4)
#define ARG_APPLY_5(M, _1, _2, _3, _4, _5) M(_1), M(_2), M(_3), M(_4), M(_5)
#define ARG_APPLY_6(M, _1, _2, _3, _4, _5, _6) M(_1), M(_2), M(_3), M(_4), M(_5), M(_6)
#define ARG_APPLY_7(M, _1, _2, _3, _4, _5, _6, _7) M(_1), M(_2), M(_3), M(_4), M(_5), M(_6), M(_7)
#define ARG_APPLY_8(M, _1, _2, _3, _4, _5, _6, _7, _8) M(_1), M(_2), M(_3), M(_4), M(_5), M(_6), M(_7), M(_8)
#define ARG_APPLY_9(M, _1, _2, _3, _4, _5, _6, _7, _8, _9) M(_1), M(_2), M(_3), M(_4), M(_5), M(_6), M(_7), M(_8), M(_9)
#define ARG_APPLY_10(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) M(_1), M(_2), M(_3), M(_4), M(_5), M(_6), M(_7), M(_8), M(_9), M(_10)
#define ARG_APPLY_11(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) M(_1), M(_2), M(_3), M(_4), M(_5), M(_6), M(_7), M(_8), M(_9), M(_10), M(_11)
#define ARG_APPLY_12(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) M(_1), M(_2), M(_3), M(_4), M(_5), M(_6), M(_7), M(_8), M(_9), M(_10), M(_11), M(_12)
#define ARG_APPLY_13(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) M(_1), M(_2), M(_3), M(_4), M(_5), M(_6), M(_7), M(_8), M(_9), M(_10), M(_11), M(_12), M(_13)
#define ARG_APPLY_14(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) M(_1), M(_2), M(_3), M(_4), M(_5), M(_6), M(_7), M(_8), M(_9), M(_10), M(_11), M(_12), M(_13), M(_14)
#define ARG_APPLY_15(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) M(_1), M(_2), M(_3), M(_4), M(_5), M(_6), M(_7), M(_8), M(_9), M(_10), M(_11), M(_12), M(_13), M(_14), M(_15)
#define ARG_APPLY_16(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) M(_1), M(_2), M(_3), M(_4), M(_5), M(_6), M(_7), M(_8), M(_9), M(_10), M(_11), M(_12), M(_13), M(_14), M(_15), M(_16)

/* FOLDL */
#define FOLDL(M, ...) MM_CONCAT(FOLDL_, ARG_COUNT(__VA_ARGS__))(M, __VA_ARGS__)
#define FOLDL_1(M, _1) _1
#define FOLDL_2(M, _1, _2) M(_1, _2)
#define FOLDL_3(M, _1, _2, _3) M(FOLDL_2(M, _1, _2), _3)
#define FOLDL_4(M, _1, _2, _3, _4) M(FOLDL_3(M, _1, _2, _3), _4)
#define FOLDL_5(M, _1, _2, _3, _4, _5) M(FOLDL_4(M, _1, _2, _3, _4), _5)
#define FOLDL_6(M, _1, _2, _3, _4, _5, _6) M(FOLDL_5(M, _1, _2, _3, _4, _5), _6)
#define FOLDL_7(M, _1, _2, _3, _4, _5, _6, _7) M(FOLDL_6(M, _1, _2, _3, _4, _5, _6), _7)
#define FOLDL_8(M, _1, _2, _3, _4, _5, _6, _7, _8) M(FOLDL_7(M, _1, _2, _3, _4, _5, _6, _7), _8)
#define FOLDL_9(M, _1, _2, _3, _4, _5, _6, _7, _8, _9) M(FOLDL_8(M, _1, _2, _3, _4, _5, _6, _7, _8), _9)
#define FOLDL_10(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) M(FOLDL_9(M, _1, _2, _3, _4, _5, _6, _7, _8, _9), _10)
#define FOLDL_11(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) M(FOLDL_10(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10), _11)
#define FOLDL_12(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) M(FOLDL_11(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11), _12)
#define FOLDL_13(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) M(FOLDL_12(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12), _13)
#define FOLDL_14(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) M(FOLDL_13(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13), _14)
#define FOLDL_15(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) M(FOLDL_14(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14), _15)
#define FOLDL_16(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) M(FOLDL_15(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15), _16)

/* APPLY */
#define APPLY(M, ...) MM_CONCAT(APPLY_, ARG_COUNT(__VA_ARGS__))(M, __VA_ARGS__)
#define APPLY_1(M, _1) M(_1); 
#define APPLY_2(M, _1, _2) M(_1); M(_2); 
#define APPLY_3(M, _1, _2, _3) M(_1); M(_2); M(_3); 
#define APPLY_4(M, _1, _2, _3, _4) M(_1); M(_2); M(_3); M(_4); 
#define APPLY_5(M, _1, _2, _3, _4, _5) M(_1); M(_2); M(_3); M(_4); M(_5); 
#define APPLY_6(M, _1, _2, _3, _4, _5, _6) M(_1); M(_2); M(_3); M(_4); M(_5); M(_6); 
#define APPLY_7(M, _1, _2, _3, _4, _5, _6, _7) M(_1); M(_2); M(_3); M(_4); M(_5); M(_6); M(_7); 
#define APPLY_8(M, _1, _2, _3, _4, _5, _6, _7, _8) M(_1); M(_2); M(_3); M(_4); M(_5); M(_6); M(_7); M(_8); 
#define APPLY_9(M, _1, _2, _3, _4, _5, _6, _7, _8, _9) M(_1); M(_2); M(_3); M(_4); M(_5); M(_6); M(_7); M(_8); M(_9); 
#define APPLY_10(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10) M(_1); M(_2); M(_3); M(_4); M(_5); M(_6); M(_7); M(_8); M(_9); M(_10); 
#define APPLY_11(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11) M(_1); M(_2); M(_3); M(_4); M(_5); M(_6); M(_7); M(_8); M(_9); M(_10); M(_11); 
#define APPLY_12(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12) M(_1); M(_2); M(_3); M(_4); M(_5); M(_6); M(_7); M(_8); M(_9); M(_10); M(_11); M(_12); 
#define APPLY_13(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13) M(_1); M(_2); M(_3); M(_4); M(_5); M(_6); M(_7); M(_8); M(_9); M(_10); M(_11); M(_12); M(_13); 
#define APPLY_14(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14) M(_1); M(_2); M(_3); M(_4); M(_5); M(_6); M(_7); M(_8); M(_9); M(_10); M(_11); M(_12); M(_13); M(_14); 
#define APPLY_15(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15) M(_1); M(_2); M(_3); M(_4); M(_5); M(_6); M(_7); M(_8); M(_9); M(_10); M(_11); M(_12); M(_13); M(_14); M(_15); 
#define APPLY_16(M, _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16) M(_1); M(_2); M(_3); M(_4); M(_5); M(_6); M(_7); M(_8); M(_9); M(_10); M(_11); M(_12); M(_13); M(_14); M(_15); M(_16); 
