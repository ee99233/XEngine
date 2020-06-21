#pragma once
#include <d3d12.h>
#include <wrl.h>
#include "XD3dUtil.h"
#include <memory>
#include "XMath.h"
using namespace XMath;
using namespace std;
using namespace Microsoft::WRL;
class FrameResource
{
public:
	FrameResource(ComPtr<ID3D12Device>& d3device);
	~FrameResource();

	unique_ptr< UploadBuff< Matrix1>>  objmatrixa;
	unique_ptr <UploadBuff< Matrix>> objmatrix;
	UINT64 fense;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> commandallocator;
private:
	
	

	
};

