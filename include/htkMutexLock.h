/*=========================================================================
 *=========================================================================*/
#ifndef __htkMutexLock_h
#define __htkMutexLock_h

#include "htkObject.h"
#include "htkObjectFactory.h"
#include "htkThreadSupport.h"
#include "htkIndent.h"

namespace htk
{

/** \class SimpleMutexLock
 * \brief Simple mutual exclusion locking class.

 * SimpleMutexLock allows the locking of variables which are accessed
 * through different threads.  This header file also defines
 * SimpleMutexLock which is not a subclass of Object.
 *
 * \ingroup OSSystemObjects
 * \ingroup HTKCommon
 */
class SimpleMutexLock
{
public:
  /** Standard class typedefs.  */
  typedef SimpleMutexLock Self;

  /** Constructor and destructor left public purposely. */
  SimpleMutexLock();
  virtual ~SimpleMutexLock();

  /** Methods for creation and destruction through the object factory. */
  static SimpleMutexLock * New();

  void Delete() { delete this; }

  /** Used for debugging and other run-time purposes. */
  virtual const char * GetNameOfClass() { return "htkSimpleMutexLock"; }

  /** Lock the MutexLock. */
  void Lock(void);

  /** Unlock the MutexLock. */
  void Unlock(void);

  /** Access the MutexType member variable from outside this class */
  MutexType & GetMutexLock()
  {
    return m_MutexLock;
  }

  MutexType GetMutexLock() const
  {
    return *( const_cast< MutexType * >( &m_MutexLock ) );
  }

protected:
  MutexType m_MutexLock;
};

/** \class MutexLock
 * \brief Mutual exclusion locking class.
 *
 * MutexLock allows the locking of variables which are accessed
 * through different threads.  This header file also defines
 * SimpleMutexLock which is not a subclass of htkObject.
 *
 * \ingroup OSSystemObjects
 * \ingroup HTKCommon
 */
class MutexLock:public Object
{
public:
  /** Standard class typedefs. */
  typedef MutexLock                  Self;
  typedef Object                     Superclass;
  typedef SmartPointer< Self >       Pointer;
  typedef SmartPointer< const Self > ConstPointer;

  /** Method for creation. */
  htkNewMacro(Self);

  /** Run-time information. */
  htkTypeMacro(MutexLock, Object);

  /** Lock the htkMutexLock. */
  void Lock(void);

  /** Unlock the MutexLock. */
  void Unlock(void);

protected:
  MutexLock() {}
  ~MutexLock() {}

  SimpleMutexLock m_SimpleMutexLock;
  void PrintSelf(std::ostream & os, Indent indent) const;

private:
  MutexLock(const Self &);      //purposely not implemented
  void operator=(const Self &); //purposely not implemented
};

inline void MutexLock::Lock(void)
{
  m_SimpleMutexLock.Lock();
}

inline void MutexLock::Unlock(void)
{
  m_SimpleMutexLock.Unlock();
}
} //end htk namespace
#endif
