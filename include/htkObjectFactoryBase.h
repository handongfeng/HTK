
#ifndef __htkObjectFactoryBase_h
#define __htkObjectFactoryBase_h


#include "htkCreateObjectFunction.h"

#include <list>
#include <vector>
#include <string>

//
//���ڶ��󹤳�,�ο��������������,д�úܲ���,���ڷ������󹤳���ԭ������ĺ�����.
//�������󹤳�: http://blog.csdn.net/azure1987/archive/2009/10/27/4732355.aspx
//

//һ�� multimap, �����洢ʶ����󹤳�����Ϣ,������Դ����Ķ���İ汾��Ϣ
class OverRideMap;

//
//MyObjectFactoryBase: ��������, ITK �м������ж�����ͨ����Ӧ�Ķ��󹤳�����
//MyObjectFactoryBase �а���һ����̬���� CreateInstance(), �÷���ͨ����ע��Ķ��󹤳���������
//��һ�ε��� CreateInstance(), ITK ��������·�������е� dll ���� ��̬�ⶼ�����ص���ǰ����
//Ϊ�˼��ػ��������еĶ�̬��̬�⵽��ǰ����, ITK ����ʹ����������, �ҽ��ⲿ��ʡ�Ե�
//
//1.���󹤳��ܹ�������ʱ���а汾�滻
//  �ڳ�����ͨ������ m_EnabledFlag ���п����Ƿ���ܴ����ض��汾�Ķ���ʵ��.
//2.ʹ�� multimap �洢ʶ����󹤳�����Ϣ, ����һ�����󹤳��п�������ͬ key �Ķ���
//  �����ڳ�������ʾ�����ͨ������ m_EnabledFlag ������ͬ�汾�Ķ���
//3.�����е�һЩ������ GetDescription ������������󹤳���Ϣ��
//  SetEnableFlag �����������ƴ����������һ���汾
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
	//����һ����Ϊ classname �������ʵ��
	//ÿ�������ص� MyObjectFactoryBase ���ᰴ���������е�˳��ѯ��
	//һ����һ�������ɹ��������󲢷���, ���๤���㲻�ٱ�ѯ��.
	static MyObject::Pointer CreateInstance(const char* classname);

	/////////////////////////////
	//ע��/ע�� ���󹤳�
	static void RegisterFactory(ObjectFactoryBase* );
	static void UnRegisterFactory(ObjectFactoryBase*);
	static void UnRegisterAllFactories();

	////////////////////////////
	//���ذ���������ע��Ķ��󹤳�������
	static std::list<ObjectFactoryBase*> GetRegisteredFactories();
	virtual const char* GetDescription(void) const = 0; //�����ö��󹤳�

	//�������󹤳������ܹ������Ķ���
	virtual std::list<std::string> GetClassOverrideNames();
	virtual std::list<std::string> GetClassOverrideWithNames();
	virtual std::list<std::string> GetClassOverrideDescriptions();

	/////////////////////////////
	//m_EnabledFlag:��������ʹ����һ���汾��������������, �Դﵽ��̬�滻��Ŀ��
	virtual std::list<bool> GetEnableFlags();
	//����/ȡ�� ָ���汾��� m_EnabledFlag ��־.
	virtual void SetEnableFlag(bool flag, const char* className, const char* subclassName);
	virtual bool GetEnableFlag(const char* className, const char* subclassName);
	//Disable: ��ʹ���󹤳����ܴ���������κΰ汾ʵ��
	virtual void Disable(const char* className);

	//OverrideInformation: ����������İ汾��Ϣ, ���Ƿ���Դ���
	struct OverrideInformation
	{
		std::string m_Description;			//�������
		std::string m_OverrideWithName;		//���صİ汾
		bool m_EnabledFlag;					//ָʾ�Ƿ���Դ����ð汾�Ķ���ʵ��.
		CreateObjectFunctionBase::Pointer m_CreateObject;
	};
protected:
	ObjectFactoryBase();
	virtual ~ObjectFactoryBase();

	//ע����󹤳�, �Լ�����Դ����Ķ���汾����Ϣ
	void RegisterOverride(
		const char* classOverride,
		const char* overrideClassName,
		const char* description,
		bool enableFlag,
		CreateObjectFunctionBase* createFunction);  //

	//���󹤳�������ʵ�� CreateObject �����ش����Ķ����ָ�룬����֧���򷵻� 0
	virtual Object::Pointer CreateObject(const char* classname );
private:
	ObjectFactoryBase(const Self&);
	void operator=(const Self&);

	static void Initialize();			//��ʼ��

	//һ�� multimap: �洢���󹤳�������Դ����Ķ���İ汾��Ϣ
	OverRideMap* m_OverrideMap;

	//���󹤳��б�, ����ָ���Ѿ�ע��Ķ��󹤳���ָ��
	static std::list<ObjectFactoryBase*>* m_RegisteredFactories; 
};

} // end namespace htk

#endif //__htkCreateObjectFunction_h
