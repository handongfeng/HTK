
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
		Object::Pointer ret = ObjectFactory::CreateInstance(typeid(T).name());
		//ObjectType *GetPointer () const { return m_Pointer; }
		//ȡ��ԭʼָ��, ������������ת��Ϊ T*
		return dynamic_cast<T*>(ret.GetPointer());
	}
};



} // end namespace htk

#endif //__htkCreateObjectFunction_h
