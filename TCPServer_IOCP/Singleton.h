#pragma once
#ifndef SINGLETON_H
#define SINGLETON_H

template <class T>
class Singleton {
private:
	static T* instance_;   //静态类成员实例延迟创建
	Singleton(void);		   //私有构造函数


public:
	static T* getInstance(void);
	static void close(void);
};

//模板类static变量 类内声明，类外定义
template <class T>
T* Singleton<T>::instance_ = nullptr;

//static T* getInstance()方法实现
template <class T>
T* Singleton<T>::getInstance(void)
{
	static std::once_flag oc;  //用于call_once的局部静态变量
	std::call_once(oc, 
									[&](void)
									{
										instance_ = new T();
									});
	return instance_;
}


//static void close()方法实现
template<class T>
void Singleton<T>::close(void)
{
	if (instance_)
	{
		delete instance_;
	}
}

#endif // !SINGLETON
