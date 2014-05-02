
#ifndef __htkCreateObjectFunction_h
#define __htkCreateObjectFunction_h

#include "htkObject.h"


//CreateObjectFunctionBase, �������.  
//���崴������Ļص�����(callback) 

namespace htk
{

class CreateObjectFunctionBase: public Object
{

public:
	typedef CreateObjectFunctionBase	Self;
	typedef Object                      Superclass
	typedef SmartPointer<Self>          Pointer;		
	typedef SmartPointer<const Self>    ConstPointer;

	//����һ������,������һ��ָ��ö����ָ��.
	virtual SmartPointer<Object> CreateObject() = 0;

protected:
	CreateObjectFunctionBase() { }
	~CreateObjectFunctionBase(){ }

private:
	CreateObjectFunctionBase(const Self&);
	void operator=(const Self&);  
};

//CreateObjectFunction: 
//��������ڴ�������Ļص�����, �� "���󹤳�" һ��ʹ��.
template <class T>
class CreateObjectFunction : public CreateObjectFunctionBase
{
public:
	typedef CreateObjectFunction	  Self;
	typedef CreateObjectFunctionBase  Superclass;
	typedef SmartPointer<Self>		  Pointer;
	typedef SmartPointer<const Self>  ConstPointer;

	//���ڴ��� Object ����ķ���.
	htkFactorylessNewMacro(Self);
	Object::Pointer CreateObject() 
	{ 
		//std::cout << "CreateObjectFunction::CreateObject() ..." << std::endl;
		return T::New().GetPointer();
	}

protected:
	CreateObjectFunction()  { }
	~CreateObjectFunction() { }
private:
	CreateObjectFunction(const Self&);
	void operator=(const Self&);
};

} // end namespace htk

#endif //__htkCreateObjectFunction_h
