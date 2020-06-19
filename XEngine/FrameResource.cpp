#include "FrameResource.h"

FrameResource::FrameResource(const ComPtr<ID3D12Device>& d3device)
{

	if (FAILED(d3device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandallocator.GetAddressOf()))))
	{
		return ;
	}
	objmatrix = make_unique<UploadBuff<struct Matrix>>(d3device,0,true);
	objmatrixa = make_unique<UploadBuff<struct Matrix1>>(d3device, 0, true);
	fense = 0;


}

FrameResource::~FrameResource()
{

}
