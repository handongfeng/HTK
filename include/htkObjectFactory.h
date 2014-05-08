
#ifndef __htkObjectFactory_h
#define __htkObjectFactory_h

#include "htkObjectFactoryBase.h"

//ObjectFactory:
//��һ��������(a helper class), ʵ�� Create() ����, ���ڴ���һ������ʵ��.
//���󹤳�����������ʱ�����滻.
//���󹤳�ͨ������ʱ����ʶ�� RTTI ʵ��.

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
		//ȡ��ԭʼָ��, ������������ת��Ϊ T*
		if(ret){printf("ret\n");}
		else {printf("!ret will return 0 in ObjectFactory::Create() \n");}
		return dynamic_cast<T*>(ret.GetPointer());
	}
};



} // end namespace htk

#endif //__htkObjectFactory_h
