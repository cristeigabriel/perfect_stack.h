#ifndef STACK_H
#define STACK_H

// Perfect Stack. Written in 2021, atomized in 2025.

#ifdef _MSC_VER
#pragma once

// warning C4127: conditional expression is constant
#pragma warning(disable : 4127)
#endif

#include <memory.h>
#include <stdlib.h>

#define STACK(t)                                                               \
  typedef struct {                                                             \
    t *m_els;                                                                  \
    long m_len;                                                                \
    long m_cap;                                                                \
  } Stack_##t;                                                                 \
  static void Stack_new_##t(Stack_##t *self) {                                 \
    self->m_els = (t *)NULL;                                                   \
    self->m_len = self->m_cap = 0;                                             \
  }                                                                            \
  static void Stack_new_cap_##t(Stack_##t *self, long cap) {                   \
    if (cap < 0)                                                               \
      return;                                                                  \
    else if (cap == 0) {                                                       \
      Stack_new_##t(self);                                                     \
      return;                                                                  \
    }                                                                          \
    self->m_els = (t *)calloc(self->m_cap = cap, sizeof(*self->m_els));        \
    self->m_len = 0;                                                           \
  }                                                                            \
  static void Stack_reserve_##t(Stack_##t *self, long cap) {                   \
    if (!self->m_els) {                                                        \
      Stack_new_cap_##t(self, cap);                                            \
      return;                                                                  \
    }                                                                          \
    self->m_els = (t *)realloc(self->m_els, sizeof(*self->m_els) * cap);       \
  }                                                                            \
  static t *Stack_push_##t(Stack_##t *self, t el) {                            \
    if (!self->m_els)                                                          \
      Stack_new_cap_##t(self, 1);                                              \
    long expected_len;                                                         \
    if (self->m_cap < (expected_len = (self->m_len + 1))) {                    \
      if (LOGARITHMIC_CAPACITY == 0) {                                         \
        self->m_els = (t *)realloc(self->m_els,                                \
                                   sizeof(*self->m_els) * (self->m_cap += 1)); \
      } else {                                                                 \
        self->m_els = (t *)realloc(self->m_els,                                \
                                   sizeof(*self->m_els) * (self->m_cap *= 2)); \
      }                                                                        \
    }                                                                          \
    long len;                                                                  \
    len = self->m_len;                                                         \
    self->m_els[len] = el;                                                     \
    InterlockedAdd((volatile LONG *)&self->m_len, 1);                          \
    return &self->m_els[len];                                                  \
  }                                                                            \
  static t *Stack_pop_##t(Stack_##t *self) {                                   \
    if (!self->m_els || self->m_len < 1)                                       \
      return NULL;                                                             \
    InterlockedAdd((volatile LONG *)&self->m_len, -1);                         \
    return &(self->m_els[self->m_len]);                                        \
  }                                                                            \
  static void Stack_clear_##t(Stack_##t *self) {                               \
    memset(self->m_els, 0, sizeof(*self->m_els) * self->m_len);                \
    InterlockedExchange((volatile LONG *)&self->m_len, 0);                     \
  }                                                                            \
  static void Stack_free_##t(Stack_##t *self) {                                \
    free(self->m_els);                                                         \
    InterlockedExchange((volatile LONG *)&self->m_len, 0);                     \
    InterlockedExchange((volatile LONG *)&self->m_cap, 0);                     \
  }

#endif /* STACK_H */
