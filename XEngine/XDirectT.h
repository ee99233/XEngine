#pragma once
#include "XD3dUtil.h"
#include "GameTimer.h"
//#include <d3d12.h>
//#include <wrl.h>
//#include <dxgi1_4.h>
#include <d3dcommon.h>
#include <string>
#include <vector>
#include <memory>

using namespace std;
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
	void InitVertxIndex();
	void initPSO();
	void BulidShader();
	void CreateCbuff();
	void initRootSingture();
	void BulidPso();
	void BulidCostantBuff();
	Microsoft::WRL::ComPtr<ID3DBlob> ShaderCompile(const  wstring &filename, const  string &pdefine, const   string &ptarget);
	Microsoft::WRL::ComPtr<ID3D12Resource> CreateDefaultBuff(UINT64 bytesize, Microsoft::WRL::ComPtr<ID3D12Resource>& uploadbufff,void *initdata);
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
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> mcbvheap;

	GameTimer gameitimer;
	//shaderCompile
	Microsoft::WRL::ComPtr<ID3DBlob> vsshader;
	Microsoft::WRL::ComPtr<ID3DBlob> psshafer;
	//
	Microsoft::WRL::ComPtr<ID3D12RootSignature> RootSignature;
	vector<D3D12_INPUT_ELEMENT_DESC> dinputeles;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mpso;

	D3D12_VIEWPORT mScreenViewport;
	D3D12_RECT mScissorRect;

	unique_ptr<UploadBuff<class Matrix>> WorldtoviewbuffPtr;
	
	
};

