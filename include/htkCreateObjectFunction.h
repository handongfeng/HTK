
#ifndef __htkCreateObjectFunction_h
#define __htkCreateObjectFunction_h

#include "htkObject.h"


//CreateObjectFunctionBase, 抽象基类.  
//定义创建对象的回调函数(callback) 

namespace htk
{

class CreateObjectFunctionBase: public Object
{

public:
	typedef CreateObjectFunctionBase	Self;
	typedef Object                      Superclass
	typedef SmartPointer<Self>          Pointer;		
	typedef SmartPointer<const Self>    ConstPointer;

	//创建一个对象,并返回一个指向该对象的指针.
	virtual SmartPointer<Object> CreateObject() = 0;

protected:
	CreateObjectFunctionBase() { }
	~CreateObjectFunctionBase(){ }

private:
	CreateObjectFunctionBase(const Self&);
	void operator=(const Self&);  
};

//CreateObjectFunction: 
//具体的用于创建对象的回调函数, 与 "对象工厂" 一起使用.
template <class T>
class CreateObjectFunction : public CreateObjectFunctionBase
{
public:
	typedef CreateObjectFunction	  Self;
	typedef CreateObjectFunctionBase  Superclass;
	typedef SmartPointer<Self>		  Pointer;
	typedef SmartPointer<const Self>  ConstPointer;

	//用于创建 Object 对象的方法.
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
