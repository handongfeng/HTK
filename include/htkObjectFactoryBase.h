
#ifndef __htkObjectFactoryBase_h
#define __htkObjectFactoryBase_h


#include "htkCreateObjectFunction.h"

#include <list>
#include <vector>
#include <string>

//
//关于对象工厂,参考上面给出的文章,写得很不错,关于泛化对象工厂的原理阐述的很清晰.
//泛化对象工厂: http://blog.csdn.net/azure1987/archive/2009/10/27/4732355.aspx
//

//一个 multimap, 用来存储识别对象工厂的信息,及其可以创建的对象的版本信息
class OverRideMap;

//
//MyObjectFactoryBase: 工厂基类, ITK 中几乎所有对象都是通过相应的对象工厂创建
//MyObjectFactoryBase 中包含一个静态方法 CreateInstance(), 该方法通过已注册的对象工厂创建对象
//第一次调用 CreateInstance(), ITK 环境变量路径中所有的 dll 或者 静态库都被加载到当前进程
//为了加载环境变量中的动态或静态库到当前进程, ITK 中又使用了其它库, 我将这部分省略掉
//
//1.对象工厂能够在运行时进行版本替换
//  在程序中通过变量 m_EnabledFlag 进行控制是否可能创建特定版本的对象实例.
//2.使用 multimap 存储识别对象工厂的信息, 这样一个对象工厂中可以有相同 key 的对象
//  我们在程序中演示了如何通过设置 m_EnabledFlag 创建不同版本的对象
//3.代码中的一些方法如 GetDescription 是用来输出对象工厂信息的
//  SetEnableFlag 等是用来控制创建对象的哪一个版本
//

namespace htk
{

class ObjectFactoryBase: public Object
{

public:
	typedef ObjectFactoryBase	        Self;
	typedef Object                      Superclass
	typedef SmartPointer<Self>          Pointer;		
	typedef SmartPointer<const Self>    ConstPointer;

	//virtual const char* GetNameOfClass() const;
	htkTypeMacro(ObjectFactoryBase, Object);
	//返回一个名为 classname 的类对象实例
	//每个被加载的 MyObjectFactoryBase 都会按环境变量中的顺序被询问
	//一旦有一个工厂成功创建对象并返回, 其余工厂便不再被询问.
	static MyObject::Pointer CreateInstance(const char* classname);

	/////////////////////////////
	//注册/注销 对象工厂
	static void RegisterFactory(ObjectFactoryBase* );
	static void UnRegisterFactory(ObjectFactoryBase*);
	static void UnRegisterAllFactories();

	////////////////////////////
	//返回包含所有已注册的对象工厂的链表
	static std::list<ObjectFactoryBase*> GetRegisteredFactories();
	virtual const char* GetDescription(void) const = 0; //描述该对象工厂

	//描述对象工厂及其能够创建的对象
	virtual std::list<std::string> GetClassOverrideNames();
	virtual std::list<std::string> GetClassOverrideWithNames();
	virtual std::list<std::string> GetClassOverrideDescriptions();

	/////////////////////////////
	//m_EnabledFlag:用来控制使用哪一个版本的类来创建对象, 以达到动态替换的目标
	virtual std::list<bool> GetEnableFlags();
	//设置/取得 指定版本类的 m_EnabledFlag 标志.
	virtual void SetEnableFlag(bool flag, const char* className, const char* subclassName);
	virtual bool GetEnableFlag(const char* className, const char* subclassName);
	//Disable: 将使对象工厂不能创建该类的任何版本实例
	virtual void Disable(const char* className);

	//OverrideInformation: 工厂及对象的版本信息, 及是否可以创建
	struct OverrideInformation
	{
		std::string m_Description;			//类的描述
		std::string m_OverrideWithName;		//重载的版本
		bool m_EnabledFlag;					//指示是否可以创建该版本的对象实例.
		CreateObjectFunctionBase::Pointer m_CreateObject;
	};
protected:
	ObjectFactoryBase();
	virtual ~ObjectFactoryBase();

	//注册对象工厂, 以及其可以创建的对象版本的信息
	void RegisterOverride(
		const char* classOverride,
		const char* overrideClassName,
		const char* description,
		bool enableFlag,
		CreateObjectFunctionBase* createFunction);  //

	//对象工厂的子类实现 CreateObject 并返回创建的对象的指针，若不支持则返回 0
	virtual Object::Pointer CreateObject(const char* classname );
private:
	ObjectFactoryBase(const Self&);
	void operator=(const Self&);

	static void Initialize();			//初始化

	//一个 multimap: 存储对象工厂及其可以创建的对象的版本信息
	OverRideMap* m_OverrideMap;

	//对象工厂列表, 包含指向已经注册的对象工厂的指针
	static std::list<ObjectFactoryBase*>* m_RegisteredFactories; 
};

} // end namespace htk

#endif //__htkCreateObjectFunction_h
