
#ifndef __htkObjectFactory_h
#define __htkObjectFactory_h

#include "htkObjectFactoryBase.h"

//ObjectFactory:
//是一个帮助类(a helper class), 实现 Create() 方法, 用于创建一个对象实例.
//对象工厂允许在运行时进行替换.
//对象工厂通过运行时类型识别 RTTI 实现.

namespace htk
{


template <class T>
class ObjectFactory : public ObjectFactoryBase
{
public:
	static typename T::Pointer Create()
	{
		printf("in  ObjectFactory Create()\n");
		Object::Pointer ret = ObjectFactory::CreateInstance(typeid(T).name());
		//ObjectType *GetPointer () const { return m_Pointer; }
		//取得原始指针, 将其向下类型转换为 T*
		if(ret){printf("ret\n");}
		else {printf("!ret will return 0 in ObjectFactory::Create() \n");}
		return dynamic_cast<T*>(ret.GetPointer());
	}
};



} // end namespace htk

#endif //__htkObjectFactory_h
