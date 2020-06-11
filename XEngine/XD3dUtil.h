#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <dxgi1_4.h>
#include "d3dx12.h"
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



inline UINT16  Calabuffer(UINT16 bytesize)
{

	bytesize += 255;
	bytesize = bytesize & (0X00FF);


	return 1;
}


template<typename T>
class UploadBuff
{

public:
	UploadBuff(Microsoft::WRL::ComPtr<ID3D12Device> &device,const UINT16 &elementcount,bool IsContbuff)
	{
		elemmentsize = sizeof(T);
		
		if (IsContbuff)
		{
			elemmentsize = Calabuffer(sizeof(T));
		}
		
		HRESULT hr = device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE,
			CD3DX12_RESOURCE_DESC::Buffer(elemmentsize*elementcount)
			, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(uploadresource.GetAddressOf()));

		if (FAILED(hr))
		{
			return;
		}
		if (FAILED(uploadresource->Map(0, nullptr,reinterpret_cast<void **>(&Mapdata))))
		{
			return;
		}
	}
	~UploadBuff()
	{
		if (uploadresource != nullptr)
		{

			uploadresource->Unmap(0, nullptr);
		}

		Mapdata = nullptr;
	}
	

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> uploadresource;
	byte* elememt;
	UINT16 elemmentsize;
	byte* Mapdata;

};