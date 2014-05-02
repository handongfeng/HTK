
#ifndef __htkObjectk_h
#define __htkObjectk_h


#include "htkMacro.h"
#include "htkFastMutexLock.h"
#include "htkSmartPointer.h"

#if defined(_WIN32)
  //To get LONG defined
  #include "Windows.h"
#endif

//MyObject ���������ж���Ļ���, ʵ�����ü�������, 
//��������� ITK �е� LightObject ��, LightObject �� ITK ����������.

namespace htk
{

class Object
{

public:
	typedef Object					  Self;
	typedef SmartPointer<Self>        Pointer;		//��װΪ����ָ��
	typedef SmartPointer<const Self>  ConstPointer;

	//ͨ�����󹤳����������ʵ��
	static Pointer New();

	//�����Ѵ��ڵĶ��󴴽�һ�µĶ���ʵ���������û�����һ����ȫ��ͬ�Ķ���
	//������Ҫ��������ת��ʱ�ǳ�����***
	virtual Pointer CreateAnother() const;

	//ʹ�� New() �Ķ���, Ӧ��ʹ�� Delete()
	//Delete() ����� UnRegister �������ü���
	//ֻ�е��ö�������ü�����Ϊ 0 ʱ�Ż�����ɾ������
	virtual void Delete();

	//����ʱ����ʶ��, ����ĳ����ľ�������.
	virtual const char *GetNameOfClass() const 
	{return "htk::Object";}

	//Used to avoid dll boundary problems.
	void* operator new(size_t);
	void* operator new[](size_t);
	void operator delete(void*);
	void operator delete[](void*, size_t);

	//������������ü���
	virtual void Register() const;
	virtual void UnRegister() const;

	//���ظ������ʵ�������ü���
	virtual int GetReferenceCount() const 
	{	return static_cast<int>(m_ReferenceCount);	}

	//���ø����������ü���ֵ.
	virtual void SetReferenceCount(int);

protected:
	Object() : m_ReferenceCount(1) {	}
	virtual ~Object(); 
		
	//��ͬ��ƽ̨, ��Ҫ���岻ͬ������, ����ֻ��ʹ�� int ��. ���ü���.
#if (defined(WIN32) || defined(_WIN32))
	typedef LONG InternalReferenceCountType;
#else
	typedef int InternalReferenceCountType;
#endif


	//���ü���
	mutable InternalReferenceCountType m_ReferenceCount;
	//mutable int	m_ReferenceCount;	

	//�ٽ�����, ���ڶ��ٽ�����������
	mutable FastMutexLock			m_ReferenceCountLock;


private:
	Object(const Self&);		         
	void operator=(const Self&); 
};


} // end namespace htk

#endif //__htkObjectk_h
