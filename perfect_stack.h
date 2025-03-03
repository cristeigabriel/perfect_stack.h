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
    InterlockedExchangePointer((volatile PVOID *)&self->m_els, NULL);          \
    InterlockedExchange((volatile LONG *)&self->m_len, 0);                     \
    InterlockedExchange((volatile LONG *)&self->m_cap, 0);                     \
  }                                                                            \
  static void Stack_new_cap_##t(Stack_##t *self, long cap) {                   \
    if (cap < 0)                                                               \
      return;                                                                  \
    else if (cap == 0) {                                                       \
      Stack_new_##t(self);                                                     \
      return;                                                                  \
    }                                                                          \
    InterlockedExchange((volatile LONG *)&self->m_cap, cap);                   \
    InterlockedExchange((volatile LONG *)&self->m_len, 0);                     \
    InterlockedExchangePointer((volatile PVOID *)&self->m_els,                 \
                               calloc(self->m_cap, sizeof(*self->m_els)));     \
  }                                                                            \
  static void Stack_reserve_##t(Stack_##t *self, long cap) {                   \
    if (!self->m_els) {                                                        \
      Stack_new_cap_##t(self, cap);                                            \
      return;                                                                  \
    }                                                                          \
    InterlockedExchangePointer(                                                \
        (volatile PVOID *)&self->m_els,                                        \
        realloc(self->m_els, sizeof(*self->m_els) * cap));                     \
  }                                                                            \
  static void Stack_push_##t(Stack_##t *self, t el) {                          \
    if (!self->m_els)                                                          \
      Stack_new_cap_##t(self, 1);                                              \
    long expected_len;                                                         \
    if (self->m_cap < (expected_len = (self->m_len + 1))) {                    \
      if (LOGARITHMIC_CAPACITY == 0) {                                         \
        InterlockedAdd((volatile LONG *)&self->m_cap, 1);                      \
        InterlockedExchangePointer(                                            \
            (volatile PVOID *)&self->m_els,                                    \
            realloc(self->m_els, sizeof(*self->m_els) * self->m_cap));         \
      } else {                                                                 \
        InterlockedAdd((volatile LONG *)&self->m_cap, self->m_cap);            \
        InterlockedExchangePointer(                                            \
            (volatile PVOID *)&self->m_els,                                    \
            realloc(self->m_els, sizeof(*self->m_els) * self->m_cap));         \
      }                                                                        \
    }                                                                          \
    InterlockedAdd((volatile LONG *)&self->m_len, 1);                          \
    self->m_els[expected_len - 1] = el;                                        \
  }                                                                            \
  static int Stack_pop_##t(Stack_##t *self, t *result) {                       \
    if (!self->m_els || self->m_len < 1)                                       \
      return 0;                                                                \
    memcpy((void *)result,                                                     \
           (const void *)&self                                                 \
               ->m_els[InterlockedAdd((volatile LONG *)&self->m_len, -1)],     \
           sizeof(t));                                                         \
    return 1;                                                                  \
  }                                                                            \
  static void Stack_clear_##t(Stack_##t *self) {                               \
    long len;                                                                  \
    len = self->m_len;                                                         \
    InterlockedExchange((volatile LONG *)&self->m_len, 0);                     \
    memset(self->m_els, 0, sizeof(*self->m_els) * len);                        \
  }                                                                            \
  static void Stack_free_##t(Stack_##t *self) {                                \
    InterlockedExchange((volatile LONG *)&self->m_len, 0);                     \
    InterlockedExchange((volatile LONG *)&self->m_cap, 0);                     \
    free(self->m_els);                                                         \
  }

#endif /* STACK_H */
