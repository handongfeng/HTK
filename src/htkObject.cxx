#include "htkObject.h"
#include "htkFastMutexLock.h"

#include "htkObjectFactory.h"

namespace htk
{



Object::Pointer Object::New()
{
  printf("htk::Object New()\n");
  Pointer      smartPtr;
  Object *rawPtr = ::htk::ObjectFactory< Object >::Create();

  if ( rawPtr == NULL )
  {
	//printf("  if ( rawPtr == NULL ) \n");
    rawPtr = new Object;
  }
  smartPtr = rawPtr;
  rawPtr->UnRegister();
  return smartPtr;
}



Object::Pointer Object::CreateAnother() const
{
  return Object::New();
}




void Object::Delete()
{
	this->UnRegister();
}



/**
 * Avoid DLL boundary problems.
 */
#ifdef _WIN32

void * Object::operator new(size_t n)
{
  return new char[n];
}

void * Object::operator new[](size_t n)
{
  return new char[n];
}

void Object::operator delete(void *m)
{
  delete[] (char *)m;
}

void Object::operator delete[](void *m, size_t)
{
  delete[] (char *)m;
}

#endif





/**
 * Increase the reference count (mark as used by another object).
 */
void Object::Register() const
{
  // Windows optimization
#if ( defined( WIN32 ) || defined( _WIN32 ) )
  InterlockedIncrement(&m_ReferenceCount);
  // General case
#else
  m_ReferenceCountLock.Lock();
  m_ReferenceCount++;
  m_ReferenceCountLock.Unlock();
#endif
}

/**
 * Decrease the reference count (release by another object).
 */
void Object::UnRegister() const
{
  // As ReferenceCount gets unlocked, we may have a race condition
  // to delete the object.

  // Windows optimization
#if ( defined( WIN32 ) || defined( _WIN32 ) )
  if ( InterlockedDecrement(&m_ReferenceCount) <= 0 )
    {
      delete this;
    }
// General case
#else
  m_ReferenceCountLock.Lock();
  InternalReferenceCountType tmpReferenceCount = --m_ReferenceCount;
  m_ReferenceCountLock.Unlock();

  if ( tmpReferenceCount <= 0 )
  {
    delete this;
   }

#endif

}


/**
 * Sets the reference count (use with care)
 */
void Object::SetReferenceCount(int ref)
{
  m_ReferenceCountLock.Lock();
  m_ReferenceCount = static_cast< InternalReferenceCountType >( ref );
  m_ReferenceCountLock.Unlock();

  if ( ref <= 0 )
  {
    delete this;
  }
}

Object::~Object()
{
  if ( m_ReferenceCount > 0 && !std::uncaught_exception() )
  {
    //itkWarningMacro("Trying to delete object with non-zero reference count.");
  }
}




} // end namespace htk

