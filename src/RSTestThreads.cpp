#include "threads/ThreadGroup.h"
#include <iostream>

using namespace std;

void *SomeRandomFunctionThatAThreadCalls(void *data)
{
	cout << "HARRO FROM CHILD THREAD ^_^" << endl;
	return NULL;
}

int main(int argc, char **argv)
{
	cout << "Rhythm-Station test suite: Threads" << endl;
	RSThread thread;

	thread.Start(SomeRandomFunctionThatAThreadCalls);

	cout << "greetings hi hello this is parent thread yes hello" << endl;

	thread.Wait();

	return 0;
}
