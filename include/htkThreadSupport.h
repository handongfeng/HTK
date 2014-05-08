/*=========================================================================
 *=========================================================================*/
#ifndef __htkThreadSupport_h
#define __htkThreadSupport_h


// This implementation uses a routine called SignalObjectAndWait()
// which is only defined on WinNT 4.0 or greater systems.  We need to
// define this symbol in order to get the prototype for the
// routine. This needs to be done before we load any system headers.

#define HTK_USE_WIN32_THREADS



#define One 1;

#ifdef HTK_USE_WIN32_THREADS
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#endif

#if defined(HTK_USE_PTHREADS)
#include <pthread.h>
#elif defined(HTK_USE_WIN32_THREADS)
//#include "itkWindows.h"

#if defined(_MSC_VER) || defined(__MINGW32__) // if on Windows
#ifndef NOMINMAX
#define NOMINMAX
#endif
#ifdef WIN32_LEAN_AND_MEAN
#undef WIN32_LEAN_AND_MEAN
#endif
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <winbase.h>
#endif

#include <winbase.h>

#endif


namespace htk
{
  /** Platform specific typedefs for simple types
   */
#if defined(HTK_USE_PTHREADS)
#define HTK_MAX_THREADS              128
  typedef pthread_mutex_t MutexType;
  typedef pthread_mutex_t FastMutexType;
  typedef void *( * ThreadFunctionType )(void *);
  typedef pthread_t ThreadProcessIDType;
#define HTK_THREAD_RETURN_VALUE  NULL
#define HTK_THREAD_RETURN_TYPE   void *

#elif defined(HTK_USE_WIN32_THREADS)

#define HTK_MAX_THREADS              128
  typedef HANDLE                 MutexType;
  typedef CRITICAL_SECTION       FastMutexType;
  typedef LPTHREAD_START_ROUTINE ThreadFunctionType;
  typedef HANDLE                 ThreadProcessIDType;
#define HTK_THREAD_RETURN_VALUE 0
#define HTK_THREAD_RETURN_TYPE DWORD __stdcall

#else

#define HTK_MAX_THREADS              1
  typedef int     MutexType;
  typedef int     FastMutexType;
  typedef void ( *ThreadFunctionType )(void *);
  typedef int     ThreadProcessIDType;
#define HTK_THREAD_RETURN_VALUE
#define HTK_THREAD_RETURN_TYPE void

#endif

  /** Platform specific Conditional Variable typedef
   */
#if defined(HTK_USE_PTHREADS)
  typedef struct {
  pthread_cond_t m_ConditionVariable;
  } ConditionVariableType;
#elif defined(HTK_USE_WIN32_THREADS)
  typedef struct {
  int m_NumberOfWaiters;                   // number of waiting threads
  CRITICAL_SECTION m_NumberOfWaitersLock;  // Serialize access to
                                           // m_NumberOfWaiters

  HANDLE m_Semaphore;                      // Semaphore to queue threads
  HANDLE m_WaitersAreDone;                 // Auto-reset event used by the
                                           // broadcast/signal thread to
                                           // wait for all the waiting
                                           // threads to wake up and
                                           // release the semaphore

  int m_WasBroadcast;                      // Used as boolean. Keeps track of whether
                                           // we were broadcasting or signaling
  } ConditionVariableType;
#else
  typedef struct { } ConditionVariableType;
#endif





/** Type to count and reference number of threads */
typedef unsigned int  ThreadIdType;


}
#endif
