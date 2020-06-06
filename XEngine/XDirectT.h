#pragma once

#include "XD3dUtil.h"



class XDirectT
{
	SingletonX(XDirectT, xdirectx)
public:
	XDirectT();
	~XDirectT();

	void LoadApater();
	void CreateGpuCommand();
	void FlushCommand();
private:
	Microsoft::WRL::ComPtr<IDXGIFactory4> dxfactory;
	Microsoft::WRL::ComPtr<ID3D12Device> d3ddevice;
	//CommandObject
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandqueue;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CommandList;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CommandAlloctor;
	Microsoft::WRL::ComPtr<ID3D12Fence> commandfence;
	UINT64 FenseCount=0;

};

