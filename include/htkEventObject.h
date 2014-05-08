/*=========================================================================
 *=========================================================================*/
#ifndef __htkEventObject_h
#define __htkEventObject_h

#include "htkIndent.h"

namespace htk
{
/** \class EventObject
 * \brief Abstraction of the Events used to communicating among filters
    and with GUIs.
 *
 * EventObject provides a standard coding for sending and receiving messages
 * indicating things like the initiation of processes, end of processes,
 * modification of filters.
 *
 * EventObjects form a hierarchy similar to the htk::ExceptionObject allowing
 * to factorize common events in a tree-like structure. Higher detail can
 * be assigned by users by subclassing existing htk::EventObjects.
 *
 * EventObjects are used by htk::Command and htk::Object for implementing the
 * Observer/Subject design pattern. Observers register their interest in
 * particular kinds of events produced by a specific htk::Object. This
 * mechanism decouples classes among them.
 *
 * As opposed to htk::Exception, htk::EventObject does not represent error
 * states, but simply flow of information allowing to trigger actions
 * as a consequence of changes occurring in state on some htk::Objects.
 *
 * htk::EventObject carries information in its own type, it relies on the
 * appropriate use of the RTTI (Run Time Type Information).
 *
 * A set of standard EventObjects is defined near the end of htkEventObject.h.
 *
 * \sa htk::Command
 * \sa htk::ExceptionObject
 */
class EventObject
{
public:
  /** Constructor and copy constructor.  Note that these functions will be
   * called when children are instantiated. */
  EventObject() {}

  EventObject(const EventObject &){}

  /** Virtual destructor needed  */
  virtual ~EventObject() {}

  /**  Create an Event of this type This method work as a Factory for
   *  creating events of each particular type. */
  virtual EventObject * MakeObject() const = 0;

  /** Print Event information.  This method can be overridden by
   * specific Event subtypes.  The default is to print out the type of
   * the event. */
  virtual void Print(std::ostream & os) const;

  /** Return the StringName associated with the event. */
  virtual const char * GetEventName(void) const = 0;

  /** Check if given event matches or derives from this event. */
  virtual bool CheckEvent(const EventObject *) const = 0;

protected:
  /** Methods invoked by Print() to print information about the object
   * including superclasses. Typically not called by the user (use Print()
   * instead) but used in the hierarchical print process to combine the
   * output of several classes.  */
  virtual void PrintSelf(std::ostream & os, Indent indent) const;

  virtual void PrintHeader(std::ostream & os, Indent indent) const;

  virtual void PrintTrailer(std::ostream & os, Indent indent) const;

private:
  typedef  EventObject *EventFactoryFunction ( );
  void operator=(const EventObject &);
};

/** Generic inserter operator for EventObject and its subclasses. */
inline std::ostream & operator<<(std::ostream & os, EventObject & e)
{
  ( &e )->Print(os);
  return os;
}


//#define HTKEvent_EXPORT HTKCommon_EXPORT

/**
 *  Macro for creating new Events
 */
#define htkEventMacro(classname, super)                                 \
  /** \class classname */                                               \
  class classname:public super                                          \
  {                                                                     \
public:                                                                 \
    typedef classname Self;                                             \
    typedef super     Superclass;                                       \
    classname() {}                                                      \
    virtual ~classname() {}                                             \
    virtual const char *GetEventName() const { return #classname; }     \
    virtual bool CheckEvent(const::htk::EventObject * e) const          \
               { return ( dynamic_cast< const Self * >( e ) != NULL ); }\
    virtual::htk::EventObject *MakeObject() const                       \
               { return new Self; }                                     \
    classname(const Self &s):super(s){};                                \
private:                                                                \
    void operator=(const Self &);                                       \
  };

/**
 *      Define some common HTK events
 */
htkEventMacro(NoEvent, EventObject)
htkEventMacro(AnyEvent, EventObject)
htkEventMacro(DeleteEvent, AnyEvent)
htkEventMacro(StartEvent, AnyEvent)
htkEventMacro(EndEvent, AnyEvent)
htkEventMacro(ProgressEvent, AnyEvent)
htkEventMacro(ExitEvent, AnyEvent)
htkEventMacro(AbortEvent, AnyEvent)
htkEventMacro(ModifiedEvent, AnyEvent)
htkEventMacro(InitializeEvent, AnyEvent)
htkEventMacro(IterationEvent, AnyEvent)
htkEventMacro(PickEvent, AnyEvent)
htkEventMacro(StartPickEvent, PickEvent)
htkEventMacro(EndPickEvent, PickEvent)
htkEventMacro(AbortCheckEvent, PickEvent)
htkEventMacro(FunctionEvaluationIterationEvent, IterationEvent)
htkEventMacro(GradientEvaluationIterationEvent, IterationEvent)
htkEventMacro(FunctionAndGradientEvaluationIterationEvent, IterationEvent)

htkEventMacro(UserEvent, AnyEvent)

//#undef HTKEvent_EXPORT
//#define HTKEvent_EXPORT HTK_ABI_EXPORT

} // end namespace htk

#endif
