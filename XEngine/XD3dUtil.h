#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <dxgi1_4.h>
#include "d3dx12.h"
#include <string>
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

using namespace Microsoft::WRL;

inline UINT16  Calabuffer(UINT16 bytesize)
{

	bytesize += 255;
	bytesize = bytesize & ~(0X00FF);


	return bytesize;
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
			&CD3DX12_RESOURCE_DESC::Buffer(elemmentsize*elementcount)
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

	ID3D12Resource* Getresource() 
	{
		return uploadresource.Get();
	}

	void CopyData(int elementindex,const T &data)
	{
		memcpy(&Mapdata[elementindex*elemmentsize],&data,sizeof(T));
	}

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> uploadresource;
	byte* elememt;
	UINT16 elemmentsize;
	byte* Mapdata;

};

class D3d12Util
{
public:
	static D3d12Util* GetUtil()
	{
		if (until == nullptr)
		{
			 until=new D3d12Util();
		}
		return until;
	}

	D3d12Util()
	{
		
	}
	~D3d12Util()
	{
		if (until != nullptr)
		{
			delete until;
			until = nullptr;
		}
	}
	void  GetDefaultBuff(IN  ComPtr<ID3D12GraphicsCommandList> &CommList, IN ComPtr<ID3D12Device> d3device, OUT ComPtr<ID3D12Resource>& uploadbuff, OUT ComPtr<ID3D12Resource>& defaultbuff,UINT bytesize,void * indata)
	{
		if (FAILED(d3device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),D3D12_HEAP_FLAG_NONE,&CD3DX12_RESOURCE_DESC::Buffer(bytesize),
			D3D12_RESOURCE_STATE_COMMON,nullptr,IID_PPV_ARGS(defaultbuff.GetAddressOf()))))
		{
			return;
		}

		if (FAILED(d3device->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(bytesize),
			D3D12_RESOURCE_STATE_GENERIC_READ,nullptr,IID_PPV_ARGS(uploadbuff.GetAddressOf()))))
		{
			return;
		}

		D3D12_SUBRESOURCE_DATA dsd;
		dsd.pData = indata;
		dsd.RowPitch = bytesize;
		dsd.SlicePitch = dsd.RowPitch;

		CommList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultbuff.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
		UpdateSubresources<1>(CommList.Get(), defaultbuff.Get(), uploadbuff.Get(), 0, 0, 1, &dsd);
		CommList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(defaultbuff.Get(),D3D12_RESOURCE_STATE_COPY_DEST,D3D12_RESOURCE_STATE_GENERIC_READ));
	

	}

private:
	static D3d12Util* until;
};

__declspec(selectany) D3d12Util* D3d12Util::until=nullptr;


struct Tecture
{
	std::wstring filename;
	std::string name;
	ComPtr<ID3D12Resource> Resource;
	ComPtr<ID3D12Resource> UploadReource;


};
