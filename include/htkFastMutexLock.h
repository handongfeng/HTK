
#ifndef __htkFastMutexLock_h
#define __htkFastMutexLock_h

#include <iostream>


#if defined(_WIN32)
	#include "windows.h"
	#include <winbase.h>
	typedef CRITICAL_SECTION FastMutexType;
#endif


#ifndef _WIN32  
    typedef int FastMutexType;    
#endif 


namespace htk
{


class FastMutexLock
{
public:
	typedef FastMutexLock   Self;

	//������ջ�ϴ����ٽ�������, ���Խ����캯��������������Ϊ pubilc.
	FastMutexLock();
	~FastMutexLock();

	//Lock, ����, ʹ�����κζ��󶼲����޸ı������Ķ���
	void Lock() const;

	//Unlock, ����
	void Unlock() const;

protected:
	mutable FastMutexType   m_FastMutexLock;
};

} // end namespace htk

#endif //__htkFastMutexLock_h
