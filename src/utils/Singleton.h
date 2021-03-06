#ifndef _SINGLETON_H_
#define _SINGLETON_H_

template <class T>
struct Singleton
{
	static T *GetSingleton()
	{
		static T ptr;
		return &ptr;
	}
};

#endif
