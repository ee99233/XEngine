#pragma once
#include "XD3dUtil.h"
#include "GameTimer.h"
#include <d3d12.h>
#include <wrl.h>
#include <dxgi1_4.h>
class XDirectT
{
	//SingletonX(XDirectT, xdirectx)
public:
	XDirectT();
	~XDirectT();
	static XDirectT* Getdirectx();
	void LoadApater();
	void CreateGpuCommand();
	void FlushCommand();
	void InitD3d();
	void CreateRerousce();
	void CreateSwapchain();
	void CreateD3dview();
	void CalcFrame();
	void UpdateTime();
	void CreateDefaultBuff();
private:
	static XDirectT* xdirectx;
	Microsoft::WRL::ComPtr<IDXGIFactory4> dxfactory;
	Microsoft::WRL::ComPtr<ID3D12Device> d3ddevice;
	Microsoft::WRL::ComPtr<IDXGISwapChain> swapchain;

	//CommandObject
	Microsoft::WRL::ComPtr<ID3D12CommandQueue> mCommandqueue;
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> CommandList;
	Microsoft::WRL::ComPtr<ID3D12CommandAllocator> CommandAlloctor;
	Microsoft::WRL::ComPtr<ID3D12Fence> commandfence;
	UINT64 FenseCount=0;
	static const UINT8 buffcout=2;
	//ResouceDep
	UINT64 MrtvDescriptionsize;
	UINT64 MdsvDescriptionsize;
	UINT64 MCbvUavDescriptionsize;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mrtvheap;
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mdsvheap;
	Microsoft::WRL::ComPtr<ID3D12Resource> SwpainChianBuff[buffcout];
	Microsoft::WRL::ComPtr<ID3D12Resource> DepthStencilView;
	GameTimer gameitimer;


};

