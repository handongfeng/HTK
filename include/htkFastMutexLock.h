
#ifndef __htkFastMutexLock_h
#define __htkFastMutexLock_h

#include <iostream>


//#if defined(_WIN32)
//	#include "windows.h"
//	#include <winbase.h>
//	typedef CRITICAL_SECTION FastMutexType;
//#endif


#ifdef _WIN32  
    typedef int FastMutexType;    
#endif 


namespace htk
{


class FastMutexLock
{
public:
	typedef FastMutexLock   Self;

	//可以在栈上创建临界区对象, 所以将构造函数与析构函数设为 pubilc.
	FastMutexLock(){};
	~FastMutexLock(){};

	//Lock, 加锁, 使其它任何对象都不能修改被加锁的对象
	void Lock() const;

	//Unlock, 解锁
	void Unlock() const;

protected:
	mutable FastMutexType   m_FastMutexLock;
};



inline void FastMutexLock::Lock(void) const
{
  //m_SimpleFastMutexLock.Lock();
}

inline void FastMutexLock::Unlock(void)const
{
  //m_SimpleFastMutexLock.Unlock();
}



} // end namespace htk

#endif //__htkFastMutexLock_h
