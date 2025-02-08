# perfect_stack.h
The perfect, single-header, atomic lock-free stack library for C on Windows. The way C was meant to be written.

# Motivation
For my silly, educational threadpool + async engine (no proper scheduler so I can just `pop`.

# Rant
Open source libraries for C are non-isolated and absolutely terrible. You should write better code. At some point I'll have to release every kind of generic piece of code that properly implements a generic interface in the same way generic instancing is done by a compiler, similar to what's seen here.

# Usage example
```c
#include "stack.h"
#define LOGARITHMIC_CAPACITY 1

	typedef void(*worker_t)(void*);
	typedef struct work {
		uint32_t worker;
		ptrdiff_t arg;
	} WORK, * PWORK;


	Stack_new_WORK(&threadpool->workload);

	Stack_push_WORK(&threadpool->workload, work);
	/* e.g like this...
	for (int i = 0; i < 4 * 1000; i += 4) {
		tp_push(&threadpool, CREATE_WORK(count_to_100k, (void*)i));
		tp_push(&threadpool, CREATE_WORK(count_to_100k, (void*)(i + 1)));
		tp_push(&threadpool, CREATE_WORK(count_to_100k, (void*)(i + 2)));
		tp_push(&threadpool, CREATE_WORK(count_to_100k, (void*)(i + 3)));
	}*/

Lrestart:
	PWORK work = {};
	while (NULL == (work = Stack_pop_WORK(&ctx->workload))) {
		// Check if threadpool should perish
		if (_InterlockedCompareExchange(&g_finished, 1, 1) != 0) {
			return 0;
		}
		Sleep(500);
	}

	worker_t worker = WORK_WORKER(*work);
	void *arg = WORK_ARG(*work);
	worker(arg);
	goto Lrestart;
```

