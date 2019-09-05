#pragma once


#define DEF_SWITCH_2(name, title, hint, name1, name2, state, func)                              \
static char *n##name[] = { name1, name2, nullptr };                                             \
static Switch name(title, hint, n##name, nullptr, &state, func);

#define DEF_SWITCH_UGO_2(name, title, hint, name1, name2, ugo1, ugo2, state, func)              \
static char *n##name[] = { name1, name2, nullptr };                                             \
static char *ugo##name[] = { ugo1, ugo2, nullptr };                                                \
static Switch name(title, hint, n##name, ugo##name, &state, func);


#define DEF_SWITCH_3(name, title, hint, name1, name2, name3, state, func)                       \
static char *n##name[] = { name1, name2, name3, nullptr };                                      \
static Switch name(title, hint, n##name, nullptr, &state, func);

#define DEF_SWITCH_5(name, title, hint, name1, name2, name3, name4, name5, state, func)         \
static char *n##name[] = { name1, name2, name3, name4, name5, nullptr };                        \
static Switch name(title, hint, n##name, nullptr, &state, func);

#define DEF_SWITCH_6(name, title, hint, name1, name2, name3, name4, name5, name6, state, func)  \
static char *n##name[] = { name1, name2, name3, name4, name5, name6, nullptr };                 \
static Switch name(title, hint, n##name, nullptr, &state, func);