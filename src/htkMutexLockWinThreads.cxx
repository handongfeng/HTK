/*=========================================================================
 *=========================================================================*/
#include "htkMutexLock.h"

namespace htk
{

// Construct a new MutexLock
SimpleMutexLock::SimpleMutexLock()
{
  m_MutexLock = CreateMutex(NULL, FALSE, NULL);
}

// Destruct the MutexVariable
SimpleMutexLock::~SimpleMutexLock()
{
  CloseHandle(m_MutexLock);
}

// Lock the MutexLock
void SimpleMutexLock::Lock()
{
  WaitForSingleObject(m_MutexLock, INFINITE);
}

// Unlock the MutexLock
void SimpleMutexLock::Unlock()
{
  ReleaseMutex(m_MutexLock);
}

} //end namespace htk
