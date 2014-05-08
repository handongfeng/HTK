/*=========================================================================
 *=========================================================================*/
#include "htkMutexLock.h"


#include "htkThreadSupport.h"

#if defined(HTK_USE_PTHREADS)
#include "htkMutexLockPThreads.cxx"
#elif defined(HTK_USE_WIN32_THREADS)
#include "htkMutexLockWinThreads.cxx"
#else
#include "htkMutexLockNoThreads.cxx"
#endif

namespace htk
{

// New for the SimpleMutex
SimpleMutexLock * SimpleMutexLock::New()
{
  return new SimpleMutexLock;
}

void MutexLock::PrintSelf(std::ostream & os, Indent indent) const
{
  //Superclass::PrintSelf(os, indent);
}


} //end namespace htk
