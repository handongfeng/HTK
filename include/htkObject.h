
#ifndef __htkObjectk_h
#define __htkObjectk_h


#include "htkMacro.h"
#include "htkFastMutexLock.h"
#include "htkSmartPointer.h"

#if defined(_WIN32)
  //To get LONG defined
  #include "Windows.h"
#endif

//MyObject 几乎是所有对象的基类, 实现引用计数功能, 
//代码基本来 ITK 中的 LightObject 类, LightObject 在 ITK 中是最顶层的类.

namespace htk
{

class Object
{

public:
	typedef Object					  Self;
	typedef SmartPointer<Self>        Pointer;		//包装为智能指针
	typedef SmartPointer<const Self>  ConstPointer;

	//通过对象工厂创建对象的实例
	static Pointer New();

	//根据已存在的对象创建一新的对象实例，允许用户创建一个完全相同的对象
	//这在需要向上类型转换时非常有用***
	virtual Pointer CreateAnother() const;

	//使用 New() 的对象, 应该使用 Delete()
	//Delete() 会调用 UnRegister 减少引用计数
	//只有当该对象的引用计数减为 0 时才会真正删除对象
	virtual void Delete();

	//运行时类型识别, 返回某对象的具体类名.
	virtual const char *GetNameOfClass() const 
	{return "htk::Object";}

	//Used to avoid dll boundary problems.
	void* operator new(size_t);
	void* operator new[](size_t);
	void operator delete(void*);
	void operator delete[](void*, size_t);

	//增加与减少引用计数
	virtual void Register() const;
	virtual void UnRegister() const;

	//返回该类对象实例的引用计数
	virtual int GetReferenceCount() const 
	{	return static_cast<int>(m_ReferenceCount);	}

	//设置该类对象的引用计数值.
	virtual void SetReferenceCount(int);

protected:
	Object() : m_ReferenceCount(1) {	}
	virtual ~Object(); 
		
	//不同的平台, 需要定义不同的类型, 这里只简单使用 int 型. 引用计数.
#if (defined(WIN32) || defined(_WIN32))
	typedef LONG InternalReferenceCountType;
#else
	typedef int InternalReferenceCountType;
#endif


	//引用计数
	mutable InternalReferenceCountType m_ReferenceCount;
	//mutable int	m_ReferenceCount;	

	//临界区锁, 用于对临界区变量加锁
	mutable FastMutexLock			m_ReferenceCountLock;


private:
	Object(const Self&);		         
	void operator=(const Self&); 
};


} // end namespace htk

#endif //__htkObjectk_h
