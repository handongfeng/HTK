

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
//该类用于清除所有已经注册的对象工厂.
class CleanUpObjectFactory
{
public:
	inline void Use(){	}
	~CleanUpObjectFactory()
	{
		ObjectFactoryBase::UnRegisterAllFactories();
	}  
};

//定义一全局静态变量, 在程序生命周期结束时析构, 用于注销所有对象工厂
static CleanUpObjectFactory CleanUpObjectFactoryGlobal;

////////////////////////////////////////
//StringOverMap, 使用 typedef 进行类型定义, 避免出现名字过长的警告.
typedef std::multimap<std::string, ObjectFactoryBase::OverrideInformation>  StringOverMapType;

//用于存储 <classID, facotoryInfo>
//classID     对象工厂的标识
//factoryInfo 该对象工厂的信息, 包含其可以创建的对象的所有版本信息
class OverRideMap : public StringOverMapType
{
};

/////////////////////////////////////////////////////
//静态变量初始化, 已注册的对象工厂链表.
std::list<ObjectFactoryBase*>* 
ObjectFactoryBase::m_RegisteredFactories = 0;

//构造函数
ObjectFactoryBase::ObjectFactoryBase()
{
	m_OverrideMap = new OverRideMap;
}

//析构, 卸载已加载的对象工厂.
ObjectFactoryBase::~ObjectFactoryBase()
{
	m_OverrideMap->erase(m_OverrideMap->begin(), m_OverrideMap->end());
	delete m_OverrideMap;
}

//使用已注册的对象工厂创建类名为 classname 的对象实例.
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
		//遍历已注册的对象列表, 如果某个对象工厂成功创建对象, 则返回
		Object::Pointer newobject = (*i)->CreateObject(classname);
		if(newobject)
		{
			newobject->Register();
			return newobject;
		}
	}
	return 0;
}

//初始化
void ObjectFactoryBase::Initialize()
{
	//全局静态变量, 用于最后注销所有的对象工厂
	CleanUpObjectFactoryGlobal.Use();

	//已经初始化过了, 返回
	if( ObjectFactoryBase::m_RegisteredFactories )
	{
		return;
	}

	ObjectFactoryBase::m_RegisteredFactories =
		new std::list<ObjectFactoryBase*>;
}

//注册对象工厂, 添加至链表中.
void ObjectFactoryBase::RegisterFactory(ObjectFactoryBase* factory)
{
	//先进行初始化，然后将该对象工厂加入到对象工厂链表中
	ObjectFactoryBase::Initialize();
	ObjectFactoryBase::m_RegisteredFactories->push_back(factory);
	factory->Register();
}

//清除已经清册的名为 factory 的对象工厂
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

//清除所有已注册的对象工厂, 并删除对象工厂列表
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
		//删除对象工厂链表...
		delete ObjectFactoryBase::m_RegisteredFactories;
		ObjectFactoryBase::m_RegisteredFactories = 0;
	}
}

//返回包含所有已注册的对象工厂的链表...
std::list<ObjectFactoryBase*> 
ObjectFactoryBase::GetRegisteredFactories()
{
	return *ObjectFactoryBase::m_RegisteredFactories;
}

///////////////////////////////
//描述对象工厂及其能够创建的对象
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

//注册对象工厂, 以及其可以创建的对象的版本信息
void ObjectFactoryBase::RegisterOverride(
	const char* classOverride,
	const char* subclass,
	const char* description,
	bool enableFlag,
	CreateObjectFunctionBase* createFunction)
{
	ObjectFactoryBase::OverrideInformation info;

	info.m_Description		= description;
	info.m_OverrideWithName = subclass;			//具体的子类
	info.m_EnabledFlag		= enableFlag;		//是否使用该子类版本
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
		//(*i).second.m_EnabledFlag = true; 则使用该工厂创建该版本的对象实例
		if ( i != m_OverrideMap->end() && (*i).second.m_EnabledFlag)
		{
			//调用了 CreateObjectFunction::CreateObject();
			//CreateObjectFunction 是用于创建对象的回调函数
			return (*i).second.m_CreateObject->CreateObject();
		}
	}
	return 0;
}

///////////////////////////
//m_EnabledFlag: 控制对象工厂可以创建的对象版本
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