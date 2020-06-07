#pragma once
//#include <d3d12.h>
//#include <wrl.h>
//#include <dxgi1_4.h>
#define SingletonX(ClassType,ClassName)\
private:                             \
	static ClassType*  ClassName; \
public:                            \
	static ClassType* Get##ClassName() \
	{                                  \
		if (ClassName == nullptr)         \
		{                                 \
		ClassName = new ClassType(); \
		}                             \
		return ClassName; \
	}

