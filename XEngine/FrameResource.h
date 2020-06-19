#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "XD3dUtil.h"
#include <memory>
using namespace std;
using namespace Microsoft::WRL;
class FrameResource
{
	FrameResource(const ComPtr<ID3D12Device>& d3device);
	~FrameResource();

	unique_ptr< UploadBuff<struct Matrix1>>  objmatrixa;
	unique_ptr < UploadBuff<struct Matrix>> objmatrix;

private:
	UINT fense;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandallocator;

	
};

