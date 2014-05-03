

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "htkObjectFactoryBase.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <algorithm>
#include <map>


namespace htk
{



//////////////////////////////////////////////////////
//����������������Ѿ�ע��Ķ��󹤳�.
class CleanUpObjectFactory
{
public:
	inline void Use(){	}
	~CleanUpObjectFactory()
	{
		ObjectFactoryBase::UnRegisterAllFactories();
	}  
};

//����һȫ�־�̬����, �ڳ����������ڽ���ʱ����, ����ע�����ж��󹤳�
static CleanUpObjectFactory CleanUpObjectFactoryGlobal;

////////////////////////////////////////
//StringOverMap, ʹ�� typedef �������Ͷ���, ����������ֹ����ľ���.
typedef std::multimap<std::string, ObjectFactoryBase::OverrideInformation>  StringOverMapType;

//���ڴ洢 <classID, facotoryInfo>
//classID     ���󹤳��ı�ʶ
//factoryInfo �ö��󹤳�����Ϣ, ��������Դ����Ķ�������а汾��Ϣ
class OverRideMap : public StringOverMapType
{
};

/////////////////////////////////////////////////////
//��̬������ʼ��, ��ע��Ķ��󹤳�����.
std::list<ObjectFactoryBase*>* 
ObjectFactoryBase::m_RegisteredFactories = 0;

//���캯��
ObjectFactoryBase::ObjectFactoryBase()
{
	m_OverrideMap = new OverRideMap;
}

//����, ж���Ѽ��صĶ��󹤳�.
ObjectFactoryBase::~ObjectFactoryBase()
{
	m_OverrideMap->erase(m_OverrideMap->begin(), m_OverrideMap->end());
	delete m_OverrideMap;
}

//ʹ����ע��Ķ��󹤳���������Ϊ classname �Ķ���ʵ��.
Object::Pointer 
ObjectFactoryBase::CreateInstance(const char* classname)
{
	if( !ObjectFactoryBase::m_RegisteredFactories )
	{
		ObjectFactoryBase::Initialize();
	}

	for( std::list<ObjectFactoryBase*>::iterator 
		i = m_RegisteredFactories->begin();
		i != m_RegisteredFactories->end(); ++i )
	{
		//������ע��Ķ����б�, ���ĳ�����󹤳��ɹ���������, �򷵻�
		Object::Pointer newobject = (*i)->CreateObject(classname);
		if(newobject)
		{
			newobject->Register();
			return newobject;
		}
	}
	return 0;
}

//��ʼ��
void ObjectFactoryBase::Initialize()
{
	//ȫ�־�̬����, �������ע�����еĶ��󹤳�
	CleanUpObjectFactoryGlobal.Use();

	//�Ѿ���ʼ������, ����
	if( ObjectFactoryBase::m_RegisteredFactories )
	{
		return;
	}

	ObjectFactoryBase::m_RegisteredFactories =
		new std::list<ObjectFactoryBase*>;
}

//ע����󹤳�, �����������.
void ObjectFactoryBase::RegisterFactory(ObjectFactoryBase* factory)
{
	//�Ƚ��г�ʼ����Ȼ�󽫸ö��󹤳����뵽���󹤳�������
	ObjectFactoryBase::Initialize();
	ObjectFactoryBase::m_RegisteredFactories->push_back(factory);
	factory->Register();
}

//����Ѿ�������Ϊ factory �Ķ��󹤳�
void ObjectFactoryBase::UnRegisterFactory(ObjectFactoryBase* factory)
{ 
	if( !ObjectFactoryBase::m_RegisteredFactories )
		return;

	for ( std::list<ObjectFactoryBase*>::iterator 
		i = m_RegisteredFactories->begin();
		i != m_RegisteredFactories->end(); ++i )
	{
		if ( factory == *i )
		{
			factory->UnRegister();
			m_RegisteredFactories->remove(factory);
			return;
		}
	}
}

//���������ע��Ķ��󹤳�, ��ɾ�����󹤳��б�
void ObjectFactoryBase::UnRegisterAllFactories()
{
	if ( ObjectFactoryBase::m_RegisteredFactories )
	{
		for ( std::list<ObjectFactoryBase*>::iterator
			f = m_RegisteredFactories->begin();
			f != m_RegisteredFactories->end(); ++f )
		{
			(*f)->UnRegister();
		}
		//ɾ�����󹤳�����...
		delete ObjectFactoryBase::m_RegisteredFactories;
		ObjectFactoryBase::m_RegisteredFactories = 0;
	}
}

//���ذ���������ע��Ķ��󹤳�������...
std::list<ObjectFactoryBase*> 
ObjectFactoryBase::GetRegisteredFactories()
{
	return *ObjectFactoryBase::m_RegisteredFactories;
}

///////////////////////////////
//�������󹤳������ܹ������Ķ���
std::list<std::string> 
ObjectFactoryBase::GetClassOverrideNames()
{
	std::list<std::string> ret;
	for ( OverRideMap::iterator i = m_OverrideMap->begin();
		i != m_OverrideMap->end(); ++i )
	{
		ret.push_back((*i).first);
	}
	return ret;
}
std::list<std::string> 
ObjectFactoryBase::GetClassOverrideWithNames()
{
	std::list<std::string> ret;
	for ( OverRideMap::iterator i = m_OverrideMap->begin();
		i != m_OverrideMap->end(); ++i )
	{
		ret.push_back((*i).second.m_OverrideWithName);
	}
	return ret;
}
std::list<std::string> 
ObjectFactoryBase::GetClassOverrideDescriptions()
{ 
	std::list<std::string> ret;
	for ( OverRideMap::iterator i = m_OverrideMap->begin();
		i != m_OverrideMap->end(); ++i )
	{
		ret.push_back((*i).second.m_Description);
	}
	return ret;
}

//ע����󹤳�, �Լ�����Դ����Ķ���İ汾��Ϣ
void ObjectFactoryBase::RegisterOverride(
	const char* classOverride,
	const char* subclass,
	const char* description,
	bool enableFlag,
	CreateObjectFunctionBase* createFunction)
{
	ObjectFactoryBase::OverrideInformation info;

	info.m_Description		= description;
	info.m_OverrideWithName = subclass;			//���������
	info.m_EnabledFlag		= enableFlag;		//�Ƿ�ʹ�ø�����汾
	info.m_CreateObject		= createFunction;

	m_OverrideMap->insert(OverRideMap::value_type(classOverride, info));
}

//
Object::Pointer 
ObjectFactoryBase::CreateObject(const char* classname)
{
	OverRideMap::iterator start = m_OverrideMap->lower_bound(classname);
	OverRideMap::iterator end = m_OverrideMap->upper_bound(classname);

	int m = 0;
	for ( OverRideMap::iterator i = start; i != end; ++i )
	{
		//(*i).second.m_EnabledFlag = true; ��ʹ�øù��������ð汾�Ķ���ʵ��
		if ( i != m_OverrideMap->end() && (*i).second.m_EnabledFlag)
		{
			//������ CreateObjectFunction::CreateObject();
			//CreateObjectFunction �����ڴ�������Ļص�����
			return (*i).second.m_CreateObject->CreateObject();
		}
	}
	return 0;
}

///////////////////////////
//m_EnabledFlag: ���ƶ��󹤳����Դ����Ķ���汾
void ObjectFactoryBase::SetEnableFlag(bool flag,
				const char* className,const char* subclassName)
{
	OverRideMap::iterator start = m_OverrideMap->lower_bound(className);
	OverRideMap::iterator end = m_OverrideMap->upper_bound(className);
	for ( OverRideMap::iterator i = start; i != end; ++i )
	{
		if ( (*i).second.m_OverrideWithName == subclassName )
		{
			(*i).second.m_EnabledFlag = flag;
		}
	}
}
bool ObjectFactoryBase
::GetEnableFlag(const char* className, const char* subclassName)
{
	OverRideMap::iterator start = m_OverrideMap->lower_bound(className);
	OverRideMap::iterator end = m_OverrideMap->upper_bound(className);
	for ( OverRideMap::iterator i = start; i != end; ++i )
	{
		if ( (*i).second.m_OverrideWithName == subclassName )
		{
			return (*i).second.m_EnabledFlag;
		}
	}
	return 0;
}
void ObjectFactoryBase::Disable(const char* className)
{
	OverRideMap::iterator start = m_OverrideMap->lower_bound(className);
	OverRideMap::iterator end = m_OverrideMap->upper_bound(className);
	for ( OverRideMap::iterator i = start; i != end; ++i )
	{
		(*i).second.m_EnabledFlag = 0;
	}
}
std::list<bool> 
ObjectFactoryBase::GetEnableFlags()
{
	std::list<bool> ret;
	for( OverRideMap::iterator i = m_OverrideMap->begin();
		 i != m_OverrideMap->end(); ++i)
	{
		ret.push_back((*i).second.m_EnabledFlag);
	}
	return ret;
}







} // end namespace htk