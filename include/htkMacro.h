

#ifndef __htkMacro_h
#define __htkMacro_h



//该宏会在类中生成两个用于创建对象的成员函数:
//1.static Pointer New(); 客户调用, 用于创建对象的接口, 使用对象工厂创建
//如果使用对象工厂创建失败, 则使用 new 操作符创建对象.
//New() 将一个原始指针赋值给 SmartPointer, 然后再调用原始指针的 UnRegister() 
//减少引用计数, 因为 MyObject 的构造函数会将引用计数初始化为 1.
//2.virtual MyObject::Pointer CreateAnother(void) const; 利用现有对象创建一个完全相同的对象
//这在需要向上类型转换时非常有用.
#define htkNewMacro(x)                                       \
	static Pointer New(void)                                 \
	{                                                        \
		printf("in htkNewMacro::New() 123\n");\
		Pointer smartPtr = htk::ObjectFactory<x>::Create();  \
		if(smartPtr.GetPointer() == NULL)                    \
		{            \
			printf("in htkNewMacro::New() 456\n");\
			smartPtr = new x;                                \
		}                                                    \
		smartPtr->UnRegister();                              \
		return smartPtr;                                     \
	}														 \
	virtual htk::Object::Pointer CreateAnother(void) const   \
	{														 \
		htk::Object::Pointer smartPtr;                       \
		smartPtr = x::New().GetPointer();                    \
		return smartPtr;                                     \
	}

//该宏会在类中生成两个用于创建对象的成员函数,但不使用对象工厂:
//1.static Pointer New(); 客户调用, 用于创建对象的接口, 注: 不使用对象工厂.
//New() 将一个原始指针赋值给 SmartPointer, 然后再调用原始指针的 UnRegister() 
//减少引用计数, 因为 MyObject 的构造函数会将引用计数初始化为 1.
//2.virtual MyObject::Pointer CreateAnother(void) const; 利用现有对象创建一个完全相同的对象
#define htkFactorylessNewMacro(x)                            \
	static Pointer New(void)                                 \
	{														 \
		Pointer smartPtr;									 \
		x *rawPtr = new x;									 \
		smartPtr = rawPtr;									 \
		rawPtr->UnRegister();								 \
		return smartPtr;									 \
	}														 \
	virtual htk::Object::Pointer CreateAnother(void) const   \
	{														 \
		htk::Object::Pointer smartPtr;						 \
		smartPtr = x::New().GetPointer();					 \
		return smartPtr;									 \
	}

//运行时类型识别.
//virtual const char* GetNameOfClass() const;
#define htkTypeMacro(thisClass,superclass)                   \
	virtual const char *GetNameOfClass() const               \
	{                                                        \
		return #thisClass;                                   \
	}





/** Get built-in type.  Creates member Get"name"() (e.g., GetVisibility());
 * This is the "const" form of the itkGetMacro.  It should be used unless
 * the member can be changed through the "Get" access routine. */
#define htkGetConstMacro(name, type)                                  \
  virtual type Get##name () const                                   \
    {                                                                 \
    return this->m_##name;                                          \
    }




#include "htkExceptionObject.h"

#define HTK_LOCATION __FUNCSIG__

/** The exception macro is used to print error information (i.e., usually
 * a condition that results in program failure). Example usage looks like:
 * itkExceptionMacro(<< "this is error info" << this->SomeVariable); */
#define htkExceptionMacro(x)                                                            \
    {                                                                                   \
    std::ostringstream message;                                                         \
    message << "htk::ERROR: " << this->GetNameOfClass()                                 \
            << "(" << this << "): " x;                                                  \
    ::htk::ExceptionObject e_(__FILE__, __LINE__, message.str().c_str(), HTK_LOCATION); \
    throw e_; /* Explicit naming to work around Intel compiler bug.  */                 \
    }






#endif //end of htkMacro.h
