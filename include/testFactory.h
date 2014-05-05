//TestMyObjectFactory.h
#pragma once

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "htkObjectFactory.h"
#include <string>
#include <list>

//测试对象工厂:
//1.对象工厂可以在运行时进行动态替换
//2.我们从 MyObject 继承得到两个版本的类: TestObject1, TestObject2
//然后创建一个创建该类对象的对象工厂, 并在开始时设置创建版本 TestObject1
//运行时, 通过设置 m_EnabledFlag变量, 可以将其动态替换为版本 TestObject2
//


//版本1



namespace htk
{

class TestObject1 : public Object
{
public:
	typedef TestObject1					Self;
	typedef Object					    Superclass;
	typedef SmartPointer<Self>		    Pointer;
	typedef SmartPointer<const Self>	ConstPointer;

	//通过对象工厂创建对象实例,返回指向对象实例的智能指针
	htkNewMacro(Self);

	//virtual const char *GetNameOfClass() const;
	htkTypeMacro(TestObject1, MyObject);

	//
	virtual ~TestObject1() {	}
	TestObject1(){	}
private:
	TestObject1(const Self&);
	void operator=(const Self&);
};

//版本2
class TestObject2 : public Object
{
public:
	typedef TestObject2					Self;
	typedef Object					Superclass;
	typedef SmartPointer<Self>		Pointer;
	typedef SmartPointer<const Self>	ConstPointer;

	//通过对象工厂创建对象实例,返回指向对象实例的智能指针
	htkNewMacro(Self);

	//virtual const char *GetNameOfClass() const;
	htkTypeMacro(TestObject2, MyObject);

	//
	virtual ~TestObject2() {	}
	TestObject2(){	}
private:
	TestObject2(const Self&);
	void operator=(const Self&);
};

//用于创建 TestObject1, TestObject2 的对象工厂.
class TestFactory : public ObjectFactoryBase
{
public:
	typedef TestFactory                Self;
	typedef ObjectFactoryBase        Superclass;
	typedef SmartPointer<Self>       Pointer;
	typedef SmartPointer<const Self> ConstPointer;

	//描述该对象工厂...
	const char* GetDescription() const { 
		return "A Test Factory";
	}

	//使用 new 操作符实例化对象工厂
	htkFactorylessNewMacro(Self);
	htkTypeMacro(TestFactory, ObjectFactoryBase);

	//注册该对象工厂.
	static void RegisterOneFactory(void)
	{
		TestFactory::Pointer factory = TestFactory::New();
		//调用对象工厂基类的静态方法
		ObjectFactoryBase::RegisterFactory(factory);    
	}

private:
	TestFactory(const Self&);    
	void operator=(const Self&);

	//构造函数
	//注册其能创建的对象,及其版本信息与描述
	TestFactory()
	{
		//TestFactory 可以创建两种对象: TestObject1, TestObject2;
		this->RegisterOverride(
			typeid(Object).name(),						//父类
			typeid(TestObject1).name(),						//自己
			"Test Object factory override 1",				//对工厂的描述
			true,											//true, 默认创建该版本
			CreateObjectFunction< TestObject1 >::New() ); //用于创建该对象的回调函数

		//this->RegisterOverride(
		//	typeid(Object).name(),
		//	typeid(TestObject2).name(),
		//	"Test Object factory override 2",
		//	false,											//false, 不创建该版本
		//	CreateObjectFunction< TestObject2 >::New());
	}
};

//测试指针 v 所指的对象是否是 expectedClassName 类的对象实例.
typedef Object::Pointer myPointer;
bool TestNewObject(myPointer v, const char* expectedClassName)
{
	std::cout << "v->GetNameOfClass(): " << v->GetNameOfClass();
	std::cout << ", expectedClassName: " << expectedClassName << std::endl;
	if(strcmp(v->GetNameOfClass(), expectedClassName) != 0)
	{
		std::cout << "Test Failed" << std::endl;
		return false;
	}
	return true;
}
//
////测试对象工厂
//int MyObjectFactoryTest(int argc, char* argv[])
//{
//	TestFactory::Pointer factory = TestFactory::New();
//	ObjectFactoryBase::RegisterFactory(factory);
//
//	//列出所有已经注册的对象工厂
//	std::list<ObjectFactoryBase *> factories =
//		ObjectFactoryBase::GetRegisteredFactories();
//
//	//打印已经注册的对象工厂的信息
//	//依次打印对象工厂链表上的各个对象工厂的信息, 当然这里只注册了一个对象工厂
//	std::cout << "-------- Registered factories --------" << std::endl;
//	for ( std::list<ObjectFactoryBase*>::iterator 
//		f = factories.begin(); f != factories.end(); ++f )
//	{
//		std::cout << "  Factory description: "
//			<< (*f)->GetDescription() << std::endl;
//
//		std::list<std::string> overrides = (*f)->GetClassOverrideNames();
//		std::list<std::string> names = (*f)->GetClassOverrideWithNames();
//		std::list<bool> enableflags = (*f)->GetEnableFlags();
//		std::list<std::string> descriptions = (*f)->GetClassOverrideDescriptions();
//
//		std::list<std::string>::const_iterator n = names.begin();
//		std::list<std::string>::const_iterator d = descriptions.begin();
//		std::list<bool>::const_iterator e = enableflags.begin();
//		//每个对象工厂可能能够创建同一对象的多种不同版本, 分别打印
//		for ( std::list<std::string>::const_iterator o = overrides.begin();
//			o != overrides.end(); ++o, ++n, ++d, ++e)
//		{
//			std::cout << "    Override " << *o         
//				<< " with " << *n << std::endl
//				<< "      described as " << *d << "" << std::endl
//				<< "      enabled " << *e << std::endl;
//		}
//	}
//	std::cout << "-----------                -----------" << std::endl;
//	//*/	
//
//	//Tets1: 测试是否能够创建版本 TestObject1 的实例
//	Object::Pointer v = Object::New();
//	int status = EXIT_SUCCESS;
//	if (!TestNewObject(v, "TestObject1"))  //Success
//	{
//		status = EXIT_FAILURE;    
//	}//*/
//	std::cout << "-------------------------------------------------------1" << std::endl;
//
//	//Test2: 关闭 ID 为 MyObject 的对象工厂, 这时无法创建 TestObject1 与 TestObject2
//	factory->Disable(typeid(Object).name());
//	v = Object::New();				//所以该语句得到的就是 MyObject
//	if (!TestNewObject(v, "Object"))
//	{
//		status = EXIT_FAILURE;    
//	}
//	std::cout << "-------------------------------------------------------2" << std::endl;
//
//	//Test3: 打开 ID 为 MyObject 的对象工厂, 并输出其相关信息进行验证
//	factory->SetEnableFlag(true,
//		typeid(Object).name(),
//		typeid(TestObject2).name());		//使其能够创建版本 TestObject2
//
//	std::cout << typeid(Object).name()
//		<< " overridden by "
//		<< typeid(TestObject2).name() << std::endl
//		<< "   EnableFlag is "
//		<< factory->GetEnableFlag(typeid(Object).name(),
//		typeid(TestObject2).name()) << std::endl;
//
//	v = Object::New();
//	if (!TestNewObject(v, "TestObject2"))
//	{
//		status = EXIT_FAILURE;    
//	}
//	std::cout << "-------------------------------------------------------3" << std::endl;
//
//	//Test4: 设置工厂可以创建版本 TestObject1, 不能创建版本 TestObject2
//	factory->SetEnableFlag(false,
//		typeid(Object).name(),
//		typeid(TestObject2).name());
//	factory->SetEnableFlag(true,
//		typeid(Object).name(),
//		typeid(TestObject1).name());
//
//	v = Object::New();
//	if (!TestNewObject(v, "TestObject1")) 
//	{
//		status = EXIT_FAILURE;    
//	}
//	std::cout << "-------------------------------------------------------4" << std::endl;
//
//	//Test5: 将所有的对象工厂注销
//	ObjectFactoryBase::UnRegisterFactory(factory);
//
//	v = Object::New();				//所以该语句得到的就是 MyObject
//	if (!TestNewObject(v, "Object"))
//	{
//		status = EXIT_FAILURE;    
//	}
//	std::cout << "-------------------------------------------------------5" << std::endl;
//
//	//Test6: 注销对象工厂后, 无法创建版本 TestObject1, TestObject2
//	v = Object::New();
//	if (!TestNewObject(v, "TestObject1")) //注销对象工厂后 Test Failed!
//	{
//		status = EXIT_FAILURE;             
//	}
//	std::cout << "-------------------------------------------------------6" << std::endl;
//
//	//Test7: 注册对象工厂, 恢复到 Test1 的状态.
//	TestFactory::RegisterOneFactory();	  //再次注册对象工厂
//	v = Object::New();
//	if (!TestNewObject(v, "TestObject1"))
//	{
//		status = EXIT_FAILURE;    
//	}
//	std::cout << "/nTest MyObjectFactory End...." << std::endl;
//
//	return status;
//}











//测试对象工厂
int MyHDFObjectFactoryTest(int argc, char* argv[])
{
	TestFactory::Pointer factory = TestFactory::New();
	ObjectFactoryBase::RegisterFactory(factory);

	//列出所有已经注册的对象工厂
	std::list<ObjectFactoryBase *> factories =
		ObjectFactoryBase::GetRegisteredFactories();


	TestObject1::Pointer p = TestObject1::New();



	////打印已经注册的对象工厂的信息
	////依次打印对象工厂链表上的各个对象工厂的信息, 当然这里只注册了一个对象工厂
	//std::cout << "-------- Registered factories --------" << std::endl;
	//for ( std::list<ObjectFactoryBase*>::iterator 
	//	f = factories.begin(); f != factories.end(); ++f )
	//{
	//	std::cout << "  Factory description: "
	//		<< (*f)->GetDescription() << std::endl;

	//	std::list<std::string> overrides = (*f)->GetClassOverrideNames();
	//	std::list<std::string> names = (*f)->GetClassOverrideWithNames();
	//	std::list<bool> enableflags = (*f)->GetEnableFlags();
	//	std::list<std::string> descriptions = (*f)->GetClassOverrideDescriptions();

	//	std::list<std::string>::const_iterator n = names.begin();
	//	std::list<std::string>::const_iterator d = descriptions.begin();
	//	std::list<bool>::const_iterator e = enableflags.begin();
	//	//每个对象工厂可能能够创建同一对象的多种不同版本, 分别打印
	//	for ( std::list<std::string>::const_iterator o = overrides.begin();
	//		o != overrides.end(); ++o, ++n, ++d, ++e)
	//	{
	//		std::cout << "    Override " << *o         
	//			<< " with " << *n << std::endl
	//			<< "      described as " << *d << "" << std::endl
	//			<< "      enabled " << *e << std::endl;
	//	}
	//}
	//std::cout << "-----------                -----------" << std::endl;

	////Tets1: 测试是否能够创建版本 TestObject1 的实例
	//
	//Object::Pointer v = Object::New();
	//int status = EXIT_SUCCESS;
	//if (!TestNewObject(v, "TestObject1"))  //Success
	//{
	//	status = EXIT_FAILURE;    
	//}//*/
	//std::cout << "-------------------------------------------------------1" << std::endl;


	return 1;
}

} // end namespace htk