#pragma once
#include <d3d12.h>
#include <wrl.h>
class XDirectT
{
public:
	XDirectT();
	~XDirectT();
private:
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandqueue;
	IID_PPV_ARGS()
};

