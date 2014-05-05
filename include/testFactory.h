//TestMyObjectFactory.h
#pragma once

#if defined(_MSC_VER)
#pragma warning ( disable : 4786 )
#endif

#include "htkObjectFactory.h"
#include <string>
#include <list>

//���Զ��󹤳�:
//1.���󹤳�����������ʱ���ж�̬�滻
//2.���Ǵ� MyObject �̳еõ������汾����: TestObject1, TestObject2
//Ȼ�󴴽�һ�������������Ķ��󹤳�, ���ڿ�ʼʱ���ô����汾 TestObject1
//����ʱ, ͨ������ m_EnabledFlag����, ���Խ��䶯̬�滻Ϊ�汾 TestObject2
//


//�汾1



namespace htk
{

class TestObject1 : public Object
{
public:
	typedef TestObject1					Self;
	typedef Object					    Superclass;
	typedef SmartPointer<Self>		    Pointer;
	typedef SmartPointer<const Self>	ConstPointer;

	//ͨ�����󹤳���������ʵ��,����ָ�����ʵ��������ָ��
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

//�汾2
class TestObject2 : public Object
{
public:
	typedef TestObject2					Self;
	typedef Object					Superclass;
	typedef SmartPointer<Self>		Pointer;
	typedef SmartPointer<const Self>	ConstPointer;

	//ͨ�����󹤳���������ʵ��,����ָ�����ʵ��������ָ��
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

//���ڴ��� TestObject1, TestObject2 �Ķ��󹤳�.
class TestFactory : public ObjectFactoryBase
{
public:
	typedef TestFactory                Self;
	typedef ObjectFactoryBase        Superclass;
	typedef SmartPointer<Self>       Pointer;
	typedef SmartPointer<const Self> ConstPointer;

	//�����ö��󹤳�...
	const char* GetDescription() const { 
		return "A Test Factory";
	}

	//ʹ�� new ������ʵ�������󹤳�
	htkFactorylessNewMacro(Self);
	htkTypeMacro(TestFactory, ObjectFactoryBase);

	//ע��ö��󹤳�.
	static void RegisterOneFactory(void)
	{
		TestFactory::Pointer factory = TestFactory::New();
		//���ö��󹤳�����ľ�̬����
		ObjectFactoryBase::RegisterFactory(factory);    
	}

private:
	TestFactory(const Self&);    
	void operator=(const Self&);

	//���캯��
	//ע�����ܴ����Ķ���,����汾��Ϣ������
	TestFactory()
	{
		//TestFactory ���Դ������ֶ���: TestObject1, TestObject2;
		this->RegisterOverride(
			typeid(Object).name(),						//����
			typeid(TestObject1).name(),						//�Լ�
			"Test Object factory override 1",				//�Թ���������
			true,											//true, Ĭ�ϴ����ð汾
			CreateObjectFunction< TestObject1 >::New() ); //���ڴ����ö���Ļص�����

		//this->RegisterOverride(
		//	typeid(Object).name(),
		//	typeid(TestObject2).name(),
		//	"Test Object factory override 2",
		//	false,											//false, �������ð汾
		//	CreateObjectFunction< TestObject2 >::New());
	}
};

//����ָ�� v ��ָ�Ķ����Ƿ��� expectedClassName ��Ķ���ʵ��.
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
////���Զ��󹤳�
//int MyObjectFactoryTest(int argc, char* argv[])
//{
//	TestFactory::Pointer factory = TestFactory::New();
//	ObjectFactoryBase::RegisterFactory(factory);
//
//	//�г������Ѿ�ע��Ķ��󹤳�
//	std::list<ObjectFactoryBase *> factories =
//		ObjectFactoryBase::GetRegisteredFactories();
//
//	//��ӡ�Ѿ�ע��Ķ��󹤳�����Ϣ
//	//���δ�ӡ���󹤳������ϵĸ������󹤳�����Ϣ, ��Ȼ����ֻע����һ�����󹤳�
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
//		//ÿ�����󹤳������ܹ�����ͬһ����Ķ��ֲ�ͬ�汾, �ֱ��ӡ
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
//	//Tets1: �����Ƿ��ܹ������汾 TestObject1 ��ʵ��
//	Object::Pointer v = Object::New();
//	int status = EXIT_SUCCESS;
//	if (!TestNewObject(v, "TestObject1"))  //Success
//	{
//		status = EXIT_FAILURE;    
//	}//*/
//	std::cout << "-------------------------------------------------------1" << std::endl;
//
//	//Test2: �ر� ID Ϊ MyObject �Ķ��󹤳�, ��ʱ�޷����� TestObject1 �� TestObject2
//	factory->Disable(typeid(Object).name());
//	v = Object::New();				//���Ը����õ��ľ��� MyObject
//	if (!TestNewObject(v, "Object"))
//	{
//		status = EXIT_FAILURE;    
//	}
//	std::cout << "-------------------------------------------------------2" << std::endl;
//
//	//Test3: �� ID Ϊ MyObject �Ķ��󹤳�, ������������Ϣ������֤
//	factory->SetEnableFlag(true,
//		typeid(Object).name(),
//		typeid(TestObject2).name());		//ʹ���ܹ������汾 TestObject2
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
//	//Test4: ���ù������Դ����汾 TestObject1, ���ܴ����汾 TestObject2
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
//	//Test5: �����еĶ��󹤳�ע��
//	ObjectFactoryBase::UnRegisterFactory(factory);
//
//	v = Object::New();				//���Ը����õ��ľ��� MyObject
//	if (!TestNewObject(v, "Object"))
//	{
//		status = EXIT_FAILURE;    
//	}
//	std::cout << "-------------------------------------------------------5" << std::endl;
//
//	//Test6: ע�����󹤳���, �޷������汾 TestObject1, TestObject2
//	v = Object::New();
//	if (!TestNewObject(v, "TestObject1")) //ע�����󹤳��� Test Failed!
//	{
//		status = EXIT_FAILURE;             
//	}
//	std::cout << "-------------------------------------------------------6" << std::endl;
//
//	//Test7: ע����󹤳�, �ָ��� Test1 ��״̬.
//	TestFactory::RegisterOneFactory();	  //�ٴ�ע����󹤳�
//	v = Object::New();
//	if (!TestNewObject(v, "TestObject1"))
//	{
//		status = EXIT_FAILURE;    
//	}
//	std::cout << "/nTest MyObjectFactory End...." << std::endl;
//
//	return status;
//}











//���Զ��󹤳�
int MyHDFObjectFactoryTest(int argc, char* argv[])
{
	TestFactory::Pointer factory = TestFactory::New();
	ObjectFactoryBase::RegisterFactory(factory);

	//�г������Ѿ�ע��Ķ��󹤳�
	std::list<ObjectFactoryBase *> factories =
		ObjectFactoryBase::GetRegisteredFactories();


	TestObject1::Pointer p = TestObject1::New();



	////��ӡ�Ѿ�ע��Ķ��󹤳�����Ϣ
	////���δ�ӡ���󹤳������ϵĸ������󹤳�����Ϣ, ��Ȼ����ֻע����һ�����󹤳�
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
	//	//ÿ�����󹤳������ܹ�����ͬһ����Ķ��ֲ�ͬ�汾, �ֱ��ӡ
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

	////Tets1: �����Ƿ��ܹ������汾 TestObject1 ��ʵ��
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