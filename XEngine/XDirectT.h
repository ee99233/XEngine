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
#include <array>
#include "XMath.h"
#include "StaticMesh.h"
#include "FrameResource.h"
using namespace std;
using namespace XMath;
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
	void initRootSingture();
	void BulidPso();
	void BulidCostantBuff();
	void Draw();
	void Update();
	Microsoft::WRL::ComPtr<ID3DBlob> ShaderCompile(const  wstring &filename, const  string &pdefine, const   string &ptarget);
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
	UINT CurrentBuffnum=0;
	//ResouceDep
	UINT MrtvDescriptionsize;
	UINT MdsvDescriptionsize;
	UINT MCbvUavDescriptionsize;
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

	unique_ptr<UploadBuff< Matrix>> WorldtoviewbuffPtr;
	unique_ptr<UploadBuff< Matrix1>> WorldtoviewbuffPtr1;

	
	XMFLOAT4X4 mWorld = Identity4x4();
	XMFLOAT4X4 mView = Identity4x4();
	XMFLOAT4X4 mProj = Identity4x4();

	float mTheta = 1.5f*XM_PI;
	float mPhi = XM_PIDIV4;
	float mRadius = 5.0f;

	Microsoft::WRL::ComPtr<ID3D12Resource> VertxGpuBuff;
	Microsoft::WRL::ComPtr<ID3D12Resource> indexbuff;
	Microsoft::WRL::ComPtr<ID3D12Resource> uploadinbuff;
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadGpuBuff;

	unique_ptr<StaticMesh> boxMesh;

	unique_ptr<FrameResource> frameResource[3];
	UINT16 framresourcenum;
	FrameResource* currentframeresource;

};

