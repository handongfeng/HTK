/*=========================================================================

 *=========================================================================*/

#ifndef __itkIndent_h
#define __itkIndent_h

#include "htkMacro.h"
#include <iostream>

namespace htk
{
/** \class Indent
 * \brief Control indentation during Print() invocation.
 *
 * Indent is used to control indentation during the chaining print
 * process. This way nested objects can correctly indent themselves.
 * This class works with the print methods defined in Object (i.e.,
 * the public method Print() and the protected methods PrintSelf(),
 * PrintHeader(), and PrintTrailer().
 *
 * \ingroup OSSystemObjects
 * \ingroup ITKCommon
 */

//class ITKCommon_EXPORT Indent
class Indent
{
public:
  /** Standard class typedefs. */
  typedef Indent Self;

  /** Method for creation through the object factory. */
  static Self * New();

  /** Destroy this instance. */
  void Delete() { delete this; }

  /** Construct the object with an initial indentation level. */
  Indent(int ind = 0) { m_Indent = ind; }

  /** Return the name of the class. */
  static const char * GetNameOfClass() { return "Indent"; }

  /** Determine the next indentation level. Keep indenting by two until the
   * a maximum of forty spaces is reached.  */
  Indent GetNextIndent();

  /** Print out the indentation. Basically output a bunch of spaces.  */
  //friend ITKCommon_EXPORT std::ostream & operator<<(std::ostream & os, const Indent & o);
  friend std::ostream & operator<<(std::ostream & os, const Indent & o);

private:
  int m_Indent;
};
} // end namespace htk

#endif
