#ifndef DEFINE_TUPLE
#define DEFINE_TUPLE

#define __TUPLE_SELECT(_1, _2, _3, _4, _5, _6, _7, _8, NAME, ...) NAME
#define TUPLE_DEFINE(...) __TUPLE_SELECT(__VA_ARGS__, TUPLE_8_DEFINE, TUPLE_7_DEFINE, TUPLE_6_DEFINE, TUPLE_5_DEFINE, TUPLE_4_DEFINE, TUPLE_3_DEFINE, TUPLE_2_DEFINE)(__VA_ARGS__)
#include "Keywords.h"
#include "Macros.h"
// Defines a tuple of the given types.
#define t(...) JOIN(_, __VA_ARGS__)

#define TUPLE_2_DEFINE(T1, T2) typedef struct ValueTuple_##T1##_##T2 {  \
    T1 Item1;   \
    T2 Item2;   \
} t(T1,T2);     \
t(T1,T2) new(t(T1,T2))(T1 item1, T2 item2); \
void T1##_##T2##__dtor(t(T1,T2) source, out(T1) out1, out(T2) out2);

#define TUPLE_3_DEFINE(T1, T2, T3) typedef struct ValueTuple_##T1##_##T2##_##T3 {   \
    T1 Item1;   \
    T2 Item2;   \
    T3 Item3;   \
} t(T1,T2,T3);  \
t(T1,T2,T3) new(t(T1,T2,T3))(T1 item1, T2 item2, T3 item3); \
void Tuple_##T1##_##T2##_##T3##__dtor(t(T1,T2,T3) source, out(T1) out1, out(T2) out2, out(T3) out3);

#define TUPLE_4_DEFINE(T1, T2, T3, T4) typedef struct ValueTuple_##T1##_##T2##_##T3##_##T4 { \
    T1 Item1;   \
    T2 Item2;   \
    T3 Item3;   \
    T4 Item4;   \
} t(T1,T2,T3,T4); \
t(T1,T2,T3,T4) new(t(T1,T2,T3,T4))(T1 item1, T2 item2, T3 item3, T4 item4); \
void Tuple_##T1##_##T2##_##T3##_##T4##__dtor(t(T1,T2,T3,T4) source, out(T1) out1, out(T2) out2, out(T3) out3, out(T4) out4);

#define TUPLE_5_DEFINE(T1, T2, T3, T4, T5) typedef struct ValueTuple_##T1##_##T2##_##T3##_##T4##_##T5 { \
    T1 Item1;   \
    T2 Item2;   \
    T3 Item3;   \
    T4 Item4;   \
    T5 Item5;   \
} t(T1,T2,T3,T4,T5); \
t(T1,T2,T3,T4,T5) new(t(T1,T2,T3,T4,T5))(T1 item1, T2 item2, T3 item3, T4 item4, T5 item5); \
void Tuple_##T1##_##T2##_##T3##_##T4##_##T5##__dtor(t(T1,T2,T3,T4,T5) source, out(T1) out1, out(T2) out2, out(T3) out3, out(T4) out4, out(T5) out5);

#define TUPLE_6_DEFINE(T1, T2, T3, T4, T5, T6) typedef struct ValueTuple_##T1##_##T2##_##T3##_##T4##_##T5##_##T6 { \
    T1 Item1;   \
    T2 Item2;   \
    T3 Item3;   \
    T4 Item4;   \
    T5 Item5;   \
    T6 Item6;   \
} t(T1,T2,T3,T4,T5,T6); \
t(T1,T2,T3,T4,T5,T6) new(t(T1,T2,T3,T4,T5,T6))(T1 item1, T2 item2, T3 item3, T4 item4, T5 item5, T6 item6); \
void Tuple_##T1##_##T2##_##T3##_##T4##_##T5##_##T6##__dtor(t(T1,T2,T3,T4,T5,T6) source, out(T1) out1, out(T2) out2, out(T3) out3, out(T4) out4, out(T5) out5, out(T6) out6);

#define TUPLE_7_DEFINE(T1, T2, T3, T4, T5, T6, T7) typedef struct ValueTuple_##T1##_##T2##_##T3##_##T4##_##T5##_##T6##_##T7 { \
    T1 Item1;   \
    T2 Item2;   \
    T3 Item3;   \
    T4 Item4;   \
    T5 Item5;   \
    T6 Item6;   \
    T7 Item7;   \
} t(T1,T2,T3,T4,T5,T6,T7); \
t(T1,T2,T3,T4,T5,T6,T7) new(t(T1,T2,T3,T4,T5,T6,T7))(T1 item1, T2 item2, T3 item3, T4 item4, T5 item5, T6 item6, T7 item7); \
void Tuple_##T1##_##T2##_##T3##_##T4##_##T5##_##T6##_##T7##__dtor(t(T1,T2,T3,T4,T5,T6,T7) source, out(T1) out1, out(T2) out2, out(T3) out3, out(T4) out4, out(T5) out5, out(T6) out6, out(T7) out7);

#define TUPLE_8_DEFINE(T1, T2, T3, T4, T5, T6, T7, T8) typedef struct ValueTuple_##T1##_##T2##_##T3##_##T4##_##T5##_##T6##_##T7##_##T8 { \
    T1 Item1;   \
    T2 Item2;   \
    T3 Item3;   \
    T4 Item4;   \
    T5 Item5;   \
    T6 Item6;   \
    T7 Item7;   \
    T8 Item8;   \
} t(T1,T2,T3,T4,T5,T6,T7,T8); \
t(T1,T2,T3,T4,T5,T6,T7,T8) new(t(T1,T2,T3,T4,T5,T6,T7,T8))(T1 item1, T2 item2, T3 item3, T4 item4, T5 item5, T6 item6, T7 item7, T8 item8); \
void Tuple_##T1##_##T2##_##T3##_##T4##_##T5##_##T6##_##T7##_##T8##__dtor(t(T1,T2,T3,T4,T5,T6,T7,T8) source, out(T1) out1, out(T2) out2, out(T3) out3, out(T4) out4, out(T5) out5, out(T6) out6, out(T7) out7, out(T8) out8);

#pragma region Implementations

#define TUPLE_IMPLEMENT(...) __TUPLE_SELECT(__VA_ARGS__, TUPLE_8_IMPLEMENT, TUPLE_7_IMPLEMENT, TUPLE_6_IMPLEMENT, TUPLE_5_IMPLEMENT, TUPLE_4_IMPLEMENT, TUPLE_3_IMPLEMENT, TUPLE_2_IMPLEMENT)(__VA_ARGS__)

#define TUPLE_2_IMPLEMENT(T1, T2) \
t(T1,T2) new(t(T1,T2))(T1 item1, T2 item2) { return (t(T1,T2)) {item1, item2}; } \
void Tuple_##T1##_##T2##__dtor(t(T1,T2) source, out(T1) out1, out(T2) out2) \
{                                   \
    if (out1) *out1 = source.Item1; \
    if (out2) *out2 = source.Item2; \
}

#define TUPLE_3_IMPLEMENT(T1, T2, T3) \
t(T1,T2,T3) new(t(T1,T2,T3))(T1 item1, T2 item2, T3 item3) { return (t(T1,T2,T3)) {item1, item2, item3}; } \
void Tuple_##T1##_##T2##_##T3##__dtor(t(T1,T2,T3) source, out(T1) out1, out(T2) out2, out(T3) out3) \
{                                   \
    if (out1) *out1 = source.Item1; \
    if (out2) *out2 = source.Item2; \
    if (out3) *out3 = source.Item3; \
}

#define TUPLE_4_IMPLEMENT(T1, T2, T3, T4) \
t(T1,T2,T3,T4) new(t(T1,T2,T3,T4))(T1 item1, T2 item2, T3 item3, T4 item4) { return (t(T1,T2,T3,T4)) {item1, item2, item3, item4}; } \
void Tuple_##T1##_##T2##_##T3##_##T4##__dtor(t(T1,T2,T3,T4) source, out(T1) out1, out(T2) out2, out(T3) out3, out(T4) out4) \
{                                   \
    if (out1) *out1 = source.Item1; \
    if (out2) *out2 = source.Item2; \
    if (out3) *out3 = source.Item3; \
    if (out4) *out4 = source.Item4; \
}

#define TUPLE_5_IMPLEMENT(T1, T2, T3, T4, T5) \
t(T1,T2,T3,T4,T5) new(t(T1,T2,T3,T4,T5))(T1 item1, T2 item2, T3 item3, T4 item4, T5 item5) { return (t(T1,T2,T3,T4,T5)) {item1, item2, item3, item4, item5}; } \
void Tuple_##T1##_##T2##_##T3##_##T4##_##T5##__dtor(t(T1,T2,T3,T4,T5) source, out(T1) out1, out(T2) out2, out(T3) out3, out(T4) out4, out(T5) out5) \
{                                   \
    if (out1) *out1 = source.Item1; \
    if (out2) *out2 = source.Item2; \
    if (out3) *out3 = source.Item3; \
    if (out4) *out4 = source.Item4; \
    if (out5) *out5 = source.Item5; \
}

#define TUPLE_6_IMPLEMENT(T1, T2, T3, T4, T5, T6) \
t(T1,T2,T3,T4,T5,T6) new(t(T1,T2,T3,T4,T5,T6))(T1 item1, T2 item2, T3 item3, T4 item4, T5 item5, T6 item6) { return (t(T1,T2,T3,T4,T5,T6)) {item1, item2, item3, item4, item5, item6}; } \
void Tuple_##T1##_##T2##_##T3##_##T4##_##T5##_##T6##__dtor(t(T1,T2,T3,T4,T5,T6) source, out(T1) out1, out(T2) out2, out(T3) out3, out(T4) out4, out(T5) out5, out(T6) out6) \
{                                   \
    if (out1) *out1 = source.Item1; \
    if (out2) *out2 = source.Item2; \
    if (out3) *out3 = source.Item3; \
    if (out4) *out4 = source.Item4; \
    if (out5) *out5 = source.Item5; \
    if (out6) *out6 = source.Item6; \
}

#define TUPLE_7_IMPLEMENT(T1, T2, T3, T4, T5, T6, T7) \
t(T1,T2,T3,T4,T5,T6,T7) new(t(T1,T2,T3,T4,T5,T6,T7))(T1 item1, T2 item2, T3 item3, T4 item4, T5 item5, T6 item6, T7 item7) { return (t(T1,T2,T3,T4,T5,T6,T7)) {item1, item2, item3, item4, item5, item6, item7}; } \
void Tuple_##T1##_##T2##_##T3##_##T4##_##T5##_##T6##_##T7##__dtor(t(T1,T2,T3,T4,T5,T6,T7) source, out(T1) out1, out(T2) out2, out(T3) out3, out(T4) out4, out(T5) out5, out(T6) out6, out(T7) out7) \
{                                   \
    if (out1) *out1 = source.Item1; \
    if (out2) *out2 = source.Item2; \
    if (out3) *out3 = source.Item3; \
    if (out4) *out4 = source.Item4; \
    if (out5) *out5 = source.Item5; \
    if (out6) *out6 = source.Item6; \
    if (out7) *out7 = source.Item7; \
}

#define TUPLE_8_IMPLEMENT(T1, T2, T3, T4, T5, T6, T7, T8) \
t(T1,T2,T3,T4,T5,T6,T7,T8) new(t(T1,T2,T3,T4,T5,T6,T7,T8))(T1 item1, T2 item2, T3 item3, T4 item4, T5 item5, T6 item6, T7 item7, T8 item8) { return (t(T1,T2,T3,T4,T5,T6,T7,T8)) {item1, item2, item3, item4, item5, item6, item7, item8}; } \
void Tuple_##T1##_##T2##_##T3##_##T4##_##T5##_##T6##_##T7##_##T8##__dtor(t(T1,T2,T3,T4,T5,T6,T7,T8) source, out(T1) out1, out(T2) out2, out(T3) out3, out(T4) out4, out(T5) out5, out(T6) out6, out(T7) out7, out(T8) out8) \
{                                   \
    if (out1) *out1 = source.Item1; \
    if (out2) *out2 = source.Item2; \
    if (out3) *out3 = source.Item3; \
    if (out4) *out4 = source.Item4; \
    if (out5) *out5 = source.Item5; \
    if (out6) *out6 = source.Item6; \
    if (out7) *out7 = source.Item7; \
    if (out8) *out8 = source.Item8; \
}

#pragma endregion

#endif