#include <pthread.h>
#include "Thread.h"
#include "Logger.h"

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int number = 0;

// TODO: mutexes
class Thread {
public:
	void Create(void* func(void*)) {
		pthread_create(&handle, NULL, func, NULL);
	}
	void Cancel(Thread* t) {
		pthread_cancel(t->handle);
	}
	pthread_t handle;
};

void *print_stuff(void *arg) {
	Log->Print("this is why you want a mutex: %d", ++number);
	return NULL;
}

void test_threads() {
	int n_threads = 10;
	Thread threads[n_threads];
	for (int i = 0; i<n_threads; i++) {
		threads[i].Create(&print_stuff);
	}
}
