

#ifndef __htkMacro_h
#define __htkMacro_h



//�ú�������������������ڴ�������ĳ�Ա����:
//1.static Pointer New(); �ͻ�����, ���ڴ�������Ľӿ�, ʹ�ö��󹤳�����
//���ʹ�ö��󹤳�����ʧ��, ��ʹ�� new ��������������.
//New() ��һ��ԭʼָ�븳ֵ�� SmartPointer, Ȼ���ٵ���ԭʼָ��� UnRegister() 
//�������ü���, ��Ϊ MyObject �Ĺ��캯���Ὣ���ü�����ʼ��Ϊ 1.
//2.virtual MyObject::Pointer CreateAnother(void) const; �������ж��󴴽�һ����ȫ��ͬ�Ķ���
//������Ҫ��������ת��ʱ�ǳ�����.
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

//�ú�������������������ڴ�������ĳ�Ա����,����ʹ�ö��󹤳�:
//1.static Pointer New(); �ͻ�����, ���ڴ�������Ľӿ�, ע: ��ʹ�ö��󹤳�.
//New() ��һ��ԭʼָ�븳ֵ�� SmartPointer, Ȼ���ٵ���ԭʼָ��� UnRegister() 
//�������ü���, ��Ϊ MyObject �Ĺ��캯���Ὣ���ü�����ʼ��Ϊ 1.
//2.virtual MyObject::Pointer CreateAnother(void) const; �������ж��󴴽�һ����ȫ��ͬ�Ķ���
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

//����ʱ����ʶ��.
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
