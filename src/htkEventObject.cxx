/*=========================================================================

 *
 *=========================================================================*/
#include "htkEventObject.h"

namespace htk
{

void EventObject
::Print(std::ostream & os) const
{
  Indent indent;
  this->PrintHeader(os, 0);
  this->PrintSelf( os, indent.GetNextIndent() );
  this->PrintTrailer(os, 0);
}

/**
 * Define a default print header for all objects.
 */
void EventObject
	::PrintHeader(std::ostream & os, Indent indent) const
{
  os << std::endl;
  os << indent << "htk::" << this->GetEventName() << " (" << this << ")\n";
}

/**
 * Define a default print trailer for all objects.
 */
void
EventObject
::PrintTrailer(std::ostream & os, Indent indent) const
{
  os << indent << std::endl;
}

void
EventObject
::PrintSelf(std::ostream &, Indent) const
{}


} // end namespace htk
