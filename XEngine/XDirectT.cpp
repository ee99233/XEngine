#include "XDirectT.h"
#include "XWindow.h"
#include <algorithm>
#include <vector>
#include <d3dcompiler.h>
#include "d3dx12.h"
#include <DirectXColors.h>
#include "XD3dUtil.h"
#include "MeshBulid.h"
#include "FrameResource.h"
#include "XMath.h"
#include "DDSTextureLoader.h"
#include <fstream>
#pragma comment(lib,"D3D12.lib")
#pragma comment(lib,"DXGI.lib")
#pragma comment(lib,"d3dcompiler.lib")

XDirectT* XDirectT::xdirectx = nullptr;
XDirectT::XDirectT()
{
	

	
}


XDirectT::~XDirectT()
{
	if (xdirectx != nullptr)
	{
		delete xdirectx;
	}
	
}

XDirectT * XDirectT::Getdirectx()
{
	if (xdirectx == nullptr)
	{
		xdirectx = new XDirectT();
	}
	return xdirectx;
}

void XDirectT::LoadApater()
{
	IDXGIAdapter *adpter;
	std::vector<IDXGIAdapter *> adpteList;
	int i = 0;
	while (dxfactory->EnumAdapters(i, &adpter) != DXGI_ERROR_NOT_FOUND)
	{
		DXGI_ADAPTER_DESC desc;
		adpter->GetDesc(&desc);
		std::wstring text;
		text += desc.Description;
		text += L"\n";
		adpteList.push_back(adpter);
		++i;
	}





}

void XDirectT::CreateGpuCommand()
{

	D3D12_COMMAND_QUEUE_DESC commandqueuedesc = {};
	commandqueuedesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	commandqueuedesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	if (FAILED(d3ddevice->CreateCommandQueue(&commandqueuedesc, IID_PPV_ARGS(mCommandqueue.GetAddressOf()))))
	{
		return;
	}

	if (FAILED(d3ddevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(CommandAlloctor.GetAddressOf()))))
	{
		return ;
	}

	if (FAILED(d3ddevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAlloctor.Get(), nullptr, IID_PPV_ARGS(CommandList.GetAddressOf()))))
	{
		return;
	}

	CommandList->Close();
	if (FAILED(d3ddevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(commandfence.GetAddressOf()))))
	{
		return;
	}



}

void XDirectT::FlushCommand()
{

	++FenseCount;
	if (FAILED(mCommandqueue->Signal(commandfence.Get(), FenseCount)))
	{
		return;
	}

	if (commandfence->GetCompletedValue() < FenseCount)
	{
		HANDLE eventhandel = CreateEventEx(nullptr,false,false,EVENT_ALL_ACCESS);
		if (FAILED(commandfence->SetEventOnCompletion(FenseCount, eventhandel)))
		{
			return;
		}
		WaitForSingleObject(eventhandel, INFINITE);
		CloseHandle(eventhandel);
	}


}

void XDirectT::InitD3d()
{

#if defined(DEBUG)||defined(_DEBUG)
	{
		Microsoft::WRL::ComPtr<ID3D12Debug> debugcontrol;
		if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(debugcontrol.GetAddressOf()))))
		{
			return;
		}
		
	
		
		debugcontrol->EnableDebugLayer();
	}
#endif

	if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(&dxfactory))))
	{
		return;
	}

	HRESULT hardware = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(&d3ddevice));
	if (FAILED(hardware))
	{
		Microsoft::WRL::ComPtr<IDXGIAdapter> wrapAdapter;
		if (FAILED(dxfactory->EnumWarpAdapter(IID_PPV_ARGS(wrapAdapter.GetAddressOf()))))
		{
			return;
		}
		if (FAILED(D3D12CreateDevice(wrapAdapter.Get(), D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(d3ddevice.GetAddressOf()))))
		{
			return;
		}

	}
	D3D12_FEATURE_DATA_MULTISAMPLE_QUALITY_LEVELS msQualityLevels;
	msQualityLevels.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	msQualityLevels.SampleCount = 4;
	msQualityLevels.Flags = D3D12_MULTISAMPLE_QUALITY_LEVELS_FLAG_NONE;
	msQualityLevels.NumQualityLevels = 0;
	d3ddevice->CheckFeatureSupport(
		D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS,
		&msQualityLevels,
		sizeof(msQualityLevels));
	
	CreateGpuCommand();
	CreateRerousce();
	CreateSwapchain();
	CreateD3dview();
	
	BulidPso();
	initpbr();
}


void XDirectT::BulidPso()
{

	if (FAILED(CommandList->Reset(CommandAlloctor.Get(), nullptr)))
	{
		return;
	}

	BulidCostantBuff();
	initRootSingture();
	BulidShader();
	InitVertxIndex();
	initPSO();
	CommandList->Close();
	ID3D12CommandList* cmdsLists[] = { CommandList.Get() };
	mCommandqueue->ExecuteCommandLists(_countof(cmdsLists), cmdsLists);

	// Wait until initialization is complete.
	FlushCommand();
	//CommandAlloctor->Release();
}



void XDirectT::initpbr()
{
	objConstants1.light[0].LightColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
	objConstants1.light[0].LightPos = XMFLOAT3(0.f, 0.f, 20.f);
	objConstants1.light[1].LightColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
	objConstants1.light[1].LightPos = XMFLOAT3(30.f, 0.f, 0.f);
	objConstants1.light[2].LightColor = XMFLOAT3(1.0f, 1.0f, 1.0f);
	objConstants1.light[2].LightPos = XMFLOAT3(-30.f, 0.f, 0.f);

	objConstants1.mat.BaseColor = XMFLOAT3(1.0f, 0, 0);
	objConstants1.mat.metallic = 0.02;
	objConstants1.mat.Rougress = 0.1;
	objConstants1.ao = 0.2;
	objConstants1.albedo = 0.3;
	objConstants1.gworld._11 = 1.0f;
	objConstants1.gworld._22 = 1.0f;
	objConstants1.gworld._33 = 1.0f;
	objConstants1.gworld._44 = 1.0f;


}

void XDirectT::CreateRerousce()
{

	MrtvDescriptionsize=d3ddevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	MdsvDescriptionsize = d3ddevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	MCbvUavDescriptionsize = d3ddevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);


	D3D12_DESCRIPTOR_HEAP_DESC rtvheapdesc;
	rtvheapdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvheapdesc.NumDescriptors = 2;
	rtvheapdesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	rtvheapdesc.NodeMask = 0;
	if (FAILED(d3ddevice->CreateDescriptorHeap(&rtvheapdesc, IID_PPV_ARGS(mrtvheap.GetAddressOf()))))
	{
		return;
	}
	D3D12_DESCRIPTOR_HEAP_DESC dsvheapdesc;
	dsvheapdesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	dsvheapdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvheapdesc.NodeMask = 0;
	dsvheapdesc.NumDescriptors = 1;
	if (FAILED(d3ddevice->CreateDescriptorHeap(&dsvheapdesc, IID_PPV_ARGS(mdsvheap.GetAddressOf()))))
	{
		return;
	}

	D3D12_DESCRIPTOR_HEAP_DESC cbvheapdesc;
	cbvheapdesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	cbvheapdesc.NodeMask = 0;
	cbvheapdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvheapdesc.NumDescriptors = 1;
	if (FAILED(d3ddevice->CreateDescriptorHeap(&cbvheapdesc, IID_PPV_ARGS(mcbvheap.GetAddressOf()))))
	{
		return;
	}

	D3D12_DESCRIPTOR_HEAP_DESC srvdesc;
	srvdesc.NumDescriptors = 1;
	srvdesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvdesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvdesc.NodeMask = 0;
	if (FAILED(d3ddevice->CreateDescriptorHeap(&srvdesc, IID_PPV_ARGS(srvheap.GetAddressOf()))))
	{
		return ;
	}


}

void XDirectT::CreateSwapchain()
{


	swapchain.Reset();
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = XWindow::GetXwindow()->Width;
	sd.BufferDesc.Height = XWindow::GetXwindow()->Height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.OutputWindow = XWindow::GetXwindow()->ghMainWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;


	
	



	
	
	//sd.Windowed = true;
	//sd.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	//sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	
	if (FAILED(dxfactory->CreateSwapChain(mCommandqueue.Get(), &sd, swapchain.GetAddressOf())))
	{
		return;
	}




}

void XDirectT::CreateD3dview()
{

	FlushCommand();
	CommandList->Reset(CommandAlloctor.Get(),nullptr);
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvhandle(mrtvheap->GetCPUDescriptorHandleForHeapStart());

	CreateTexture();


	for (int i = 0; i < buffcout; ++i)
	{
		HRESULT hresult = swapchain->GetBuffer(i, IID_PPV_ARGS(SwpainChianBuff[i].GetAddressOf()));
		if (FAILED(hresult))
		{
			return;
		}

		d3ddevice->CreateRenderTargetView(SwpainChianBuff[i].Get(), nullptr, rtvhandle);
		
		rtvhandle.Offset(1, MrtvDescriptionsize);

	}
	D3D12_RESOURCE_DESC dd;
	dd.Alignment = 0;
	dd.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	dd.DepthOrArraySize = 1;
	dd.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	dd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dd.Height = XWindow::GetXwindow()->Height;
	dd.Width = XWindow::GetXwindow()->Width;
	dd.MipLevels = 1;
	dd.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	dd.SampleDesc.Count = 1;
	dd.SampleDesc.Quality = 0;
	

	D3D12_CLEAR_VALUE dv;
	dv.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dv.DepthStencil.Depth = 1.0f;
	dv.DepthStencil.Stencil = 0;
	
	D3D12_HEAP_PROPERTIES dp;
	dp.Type = D3D12_HEAP_TYPE_DEFAULT;
	dp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	dp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	dp.VisibleNodeMask = 1;
	dp.CreationNodeMask = 1;

	if (FAILED(d3ddevice->CreateCommittedResource(&dp,
		D3D12_HEAP_FLAG_NONE,
		&dd,
		D3D12_RESOURCE_STATE_COMMON,
		&dv,
		IID_PPV_ARGS(DepthStencilView.GetAddressOf())
	)))
	{
		return;
	}
	CD3DX12_CPU_DESCRIPTOR_HANDLE dehandel(mdsvheap->GetCPUDescriptorHandleForHeapStart());
	d3ddevice->CreateDepthStencilView(DepthStencilView.Get(), nullptr, dehandel);

	CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(DepthStencilView.Get(), D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_DEPTH_WRITE));
	CommandList->Close();
	ID3D12CommandList* cmdslist[] = { CommandList.Get() };
	mCommandqueue->ExecuteCommandLists(_countof(cmdslist), cmdslist);
	FlushCommand();

	mScreenViewport.TopLeftX = 0;
	mScreenViewport.TopLeftY = 0;
	mScreenViewport.Width = static_cast<float>(XWindow::GetXwindow()->Width);
	mScreenViewport.Height = static_cast<float>(XWindow::GetXwindow()->Height);
	mScreenViewport.MinDepth = 0.0f;
	mScreenViewport.MaxDepth = 1.0f;

	mScissorRect = { 0, 0, XWindow::GetXwindow()->Width, XWindow::GetXwindow()->Height };


	CD3DX12_CPU_DESCRIPTOR_HANDLE hde(srvheap->GetCPUDescriptorHandleForHeapStart());
	D3D12_SHADER_RESOURCE_VIEW_DESC srcdesc = {};
	srcdesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srcdesc.Format = text2d->Resource->GetDesc().Format;
	srcdesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srcdesc.Texture2D.MostDetailedMip = 0;
	srcdesc.Texture2D.MipLevels = text2d->Resource->GetDesc().MipLevels;
	srcdesc.Texture2D.ResourceMinLODClamp = 0.0f;
	d3ddevice->CreateShaderResourceView(text2d->Resource.Get(), &srcdesc, hde);
}

void XDirectT::CalcFrame()
{
	static int frameCnt=0;
	static float timeElaped = 0.0f;
	++frameCnt;
	if ((gameitimer.Totaltime() - timeElaped) >= 1.0f)
	{
		int fps = frameCnt;
		wstring wintext = to_wstring(fps);
		SetWindowText(XWindow::GetXwindow()->ghMainWnd, wintext.c_str());

		frameCnt = 0;
		timeElaped += 1.0f;
	}
}

void XDirectT::UpdateTime()
{
	gameitimer.Tick();
}



void XDirectT::BulidCostantBuff()
{

	WorldtoviewbuffPtr = make_unique<UploadBuff<Matrix>>(d3ddevice, 1, true);
	/*D3D12_GPU_VIRTUAL_ADDRESS dgva=WorldtoviewbuffPtr->Getresource()->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC dcbvd;

	dcbvd.BufferLocation = dgva;
	dcbvd.SizeInBytes = Calabuffer(sizeof(Matrix));
	d3ddevice->CreateConstantBufferView(&dcbvd, mcbvheap->GetCPUDescriptorHandleForHeapStart());*/

	WorldtoviewbuffPtr1 = make_unique<UploadBuff<Matrix1>>(d3ddevice, 1, true);

	for (int i = 0; i < 3; ++i)
	{
		
		frameResource[i] = make_unique<FrameResource >(d3ddevice);
		CurrentBuffnum = 0;
	}


}


void XDirectT::initRootSingture()
{

	CD3DX12_ROOT_PARAMETER slotRootpa[3];
	CD3DX12_DESCRIPTOR_RANGE srvtable;
	srvtable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0);
	
	slotRootpa[0].InitAsConstantBufferView(0);
	slotRootpa[1].InitAsConstantBufferView(1);
	slotRootpa[2].InitAsDescriptorTable(1,&srvtable, D3D12_SHADER_VISIBILITY_PIXEL);

	auto sampleas = GetStaticSamplers();
	CD3DX12_ROOT_SIGNATURE_DESC rootsdesc(3, slotRootpa, sampleas.size(), sampleas.data(), D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);



	Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSig = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob=nullptr;
	HRESULT hr= D3D12SerializeRootSignature(&rootsdesc, D3D_ROOT_SIGNATURE_VERSION_1, serializedRootSig.GetAddressOf(),errorBlob.GetAddressOf());

	if (FAILED(d3ddevice->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(RootSignature.GetAddressOf()))))
	{
		return; 
	}

}

void XDirectT::InitVertxIndex()
{
	//
	boxMesh = make_unique<StaticMesh>();

	//vector<XVertx4> vertxs;
	//vector<UINT16> index;
	//MeshBulid::GetMeshBulid()->CreateSphere(2.0f,50,10, vertxs,index);

	/*UINT bytesize = vertxs.size() * sizeof(XVertx4);
	UINT inbytesize = index.size() * sizeof(UINT16);*/


	//for (int i = 0; i < vertxs.size(); ++i)
	//{
	//	float z = HillHight(vertxs[i].Pos.x, vertxs[i].Pos.y);
	//	vertxs[i].Pos.z = z;
	//	if (vertxs[i].Pos.z < -10.f)
	//	{
	//		vertxs[i].Color = XMFLOAT4(1.0f, 0.96f, 0.62f, 1.0f);

	//	}
	//	else if (vertxs[i].Pos.z < 5.0f)
	//	{
	//		vertxs[i].Color = XMFLOAT4(0.48f,0.77f,0.46f,1.0f);
	//	}
	//	else if (vertxs[i].Pos.z<12.0f)
	//	{
	//		vertxs[i].Color = XMFLOAT4(0.1f,0.48f,0.19f,1.0f);
	//	}
	//	else if (vertxs[i].Pos.z<20.0f)
	//	{
	//		vertxs[i].Color = XMFLOAT4(0.45f,0.39f,0.34f,1.0f);
	//	}
	//	else
	//	{
	//		vertxs[i].Color = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	//	}

	//}
	vector<XVertx4> vertxs;
	vector<UINT> index;
	MeshBulid::GetMeshBulid()->CreateSphere(1.0f, 40.0f, 40.0f, vertxs, index);
	UINT bytesize = vertxs.size() * sizeof(XVertx4);
	UINT inbytesize = index.size() * sizeof(UINT);
	boxMesh->indexcount = index.size();

	D3d12Util::GetUtil()->GetDefaultBuff(CommandList, d3ddevice, boxMesh->UploadVertx, boxMesh->Vertxbuff, bytesize, vertxs.data());
	D3d12Util::GetUtil()->GetDefaultBuff(CommandList, d3ddevice, boxMesh->UploadIndex, boxMesh->Indexbuff, inbytesize, index.data());
	boxMesh->filename = "BOX";
	boxMesh->ISizeInBytes = inbytesize;
	boxMesh->VSizeInBytes = bytesize;
	boxMesh->VStrideInBytes = sizeof(XVertx4);
	boxMesh->xmat.BaseColor = XMFLOAT3(1.0f, 0.0f, 0.0f);
	boxMesh->xmat.metallic = 0.2f;
	boxMesh->xmat.Rougress = 0.5f;


	//unique_ptr<StaticMesh> floormesh = make_unique<StaticMesh>();
	//std::array<XVertx4, 20> vertices =
	//{
	//	// Floor: Observe we tile texture coordinates.
	//	XVertx4(-3.5f, 0.0f, -10.0f,0.0f, 1.0f, 0.0f, 0.0f, 4.0f), // 0 
	//	XVertx4(-3.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f),
	//	XVertx4(7.5f, 0.0f,   0.0f, 0.0f, 1.0f, 0.0f, 4.0f, 0.0f),
	//	XVertx4(7.5f, 0.0f, -10.0f, 0.0f, 1.0f, 0.0f, 4.0f, 4.0f),

	//	// Wall: Observe we tile texture coordinates, and that we
	//	// leave a gap in the middle for the mirror.
	//	XVertx4(-3.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f), // 4
	//	XVertx4(-3.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
	//	XVertx4(-2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 0.0f),
	//	XVertx4(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.5f, 2.0f),

	//	XVertx4(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 2.0f), // 8 
	//	XVertx4(2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
	//	XVertx4(7.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 0.0f),
	//	XVertx4(7.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 2.0f, 2.0f),

	//	XVertx4(-3.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f), // 12
	//	XVertx4(-3.5f, 6.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
	//	XVertx4(7.5f, 6.0f, 0.0f, 0.0f, 0.0f, -1.0f, 6.0f, 0.0f),
	//	XVertx4(7.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 6.0f, 1.0f),

	//	// Mirror
	//	XVertx4(-2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f), // 16
	//	XVertx4(-2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
	//	XVertx4(2.5f, 4.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f),
	//	XVertx4(2.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f)
	//};

	//std::array<UINT, 30> indices =
	//{
	//	// Floor
	//	0, 1, 2,
	//	0, 2, 3,

	//	// Walls
	//	4, 5, 6,
	//	4, 6, 7,

	//	8, 9, 10,
	//	8, 10, 11,

	//	12, 13, 14,
	//	12, 14, 15,

	//	// Mirror
	//	16, 17, 18,
	//	16, 18, 19
	//};

	//UINT bytesize = vertices.size() * sizeof(XVertx4);
	//UINT ibytesize = indices.size() * sizeof(UINT);

	//SubMesh floor;
	//floor.IndexCount = 6;
	//floor.StartIndexLocation = 0;
	//floor.StartIndexLocation = 0;

	//SubMesh wall;
	//wall.IndexCount=18;
	//wall.BaseVertexLocation = 0;
	//wall.StartIndexLocation = 6;

	//SubMesh mirror;
	//mirror.BaseVertexLocation = 0;
	//mirror.StartIndexLocation = 24;
	//mirror.IndexCount=6;

	//D3d12Util::GetUtil()->GetDefaultBuff(CommandList, d3ddevice, floormesh->UploadVertx, floormesh->Vertxbuff, bytesize, vertices.data());
	//D3d12Util::GetUtil()->GetDefaultBuff(CommandList, d3ddevice, floormesh->UploadIndex, floormesh->Indexbuff, ibytesize, indices.data());

	//floormesh->filename = "floor";
	//floormesh->indexcount = indices.size();
	//floormesh->ISizeInBytes = ibytesize;
	//floormesh->VSizeInBytes = bytesize;
	//floormesh->VStrideInBytes = sizeof(XVertx4);
	//floormesh->xmat.BaseColor = XMFLOAT3(0.2, 0.2, 0.2);
	//floormesh->xmat.metallic = 0.2f;
	//floormesh->xmat.Rougress = 0.5f;


	



	/*skullmesh = make_unique<StaticMesh>();
	ifstream fin("F:/XEngine/Models/skull.txt");
	if (!fin)
	{
		return;
	}
	UINT vcount = 0;
	UINT tcount = 0;
	std::string ignore;
	fin >> ignore >> vcount;
	fin >> ignore >> tcount;
	fin >> ignore >> ignore >> ignore >> ignore;
	vector<XVertx4>	Vertex(vcount);
	for (UINT i = 0; i < vcount; ++i)
	{
		fin >> Vertex[i].Pos.x >> Vertex[i].Pos.y >> Vertex[i].Pos.z
			>> Vertex[i].Normal.x >> Vertex[i].Normal.y >> Vertex[i].Normal.z;
		Vertex[i].TextCord.x = 0.0f;
		Vertex[i].TextCord.y = 0.0f;
	}

	fin >> ignore >> ignore >> ignore;
	vector<UINT> Indexs(tcount*3);
	for (UINT i = 0; i < tcount;++i)
	{
		fin >> Indexs[i * 3] >> Indexs[i * 3 + 1] >> Indexs[i * 3 + 2];
	}

	UINT bytesizes = Vertex.size() * sizeof(XVertx4);
	UINT inbytesizes = Indexs.size() * sizeof(UINT);

	skullmesh = make_unique<StaticMesh>();
	skullmesh->filename = "skull";
	skullmesh->indexcount = Indexs.size();
	D3d12Util::GetUtil()->GetDefaultBuff(CommandList, d3ddevice, skullmesh->UploadVertx, skullmesh->Vertxbuff, bytesizes, Vertex.data());
	D3d12Util::GetUtil()->GetDefaultBuff(CommandList, d3ddevice, skullmesh->UploadIndex, skullmesh->Indexbuff, inbytesizes, Indexs.data());
	skullmesh->VStrideInBytes = sizeof(XVertx4);
	skullmesh->VSizeInBytes = bytesizes;
	skullmesh->ISizeInBytes = inbytesizes;
	skullmesh->xmat.BaseColor = XMFLOAT3(0.2, 0.2, 0.2);
	skullmesh->xmat.metallic = 0.2f;
	skullmesh->xmat.Rougress = 0.5f;
	fin.close();*/

}

void XDirectT::BulidShader()
{
	vsshader = ShaderCompile(L"G:\\xe\\XEngine\\Xone.hlsl", "VS", "vs_5_0");
	psshafer = ShaderCompile(L"G:\\xe\\XEngine\\Xone.hlsl", "PS", "ps_5_0");

	dinputeles =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,12,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"TANGENT",0,DXGI_FORMAT_R32G32B32_FLOAT,0,24,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,36,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,44,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
	};


}

void XDirectT::initPSO()
{
	//D3D12_RASTERIZER_DESC drd;
	//drd.FillMode = D3D12_FILL_MODE_SOLID;
	//drd.CullMode = D3D12_CULL_MODE_NONE;
	//drd.DepthBias = 0;//同深度优先级，阴影在墙上但是深度一样，所以要使阴影深度小于墙深度
	//drd.ForcedSampleCount = false;
	//drd.AntialiasedLineEnable = false;
	//drd.MultisampleEnable = false;
	//drd.ConservativeRaster = D3D12_CONSERVATIVE_RASTERIZATION_MODE_ON;
	//drd.DepthBiasClamp = 0;
	//drd.DepthClipEnable = true;
	//drd.ForcedSampleCount = 0;
	//drd.SlopeScaledDepthBias = 0;
	//drd.FrontCounterClockwise = false;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC dgpsd;
	ZeroMemory(&dgpsd, sizeof(D3D12_GRAPHICS_PIPELINE_STATE_DESC));

	
	D3D12_RENDER_TARGET_BLEND_DESC drtbd;
	drtbd.BlendEnable = true;
	drtbd.LogicOpEnable = false;
	drtbd.BlendOp = D3D12_BLEND_OP_ADD;
	drtbd.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	drtbd.DestBlendAlpha = D3D12_BLEND_ZERO;
	drtbd.LogicOp = D3D12_LOGIC_OP_NOOP;
	drtbd.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	drtbd.SrcBlend = D3D12_BLEND_SRC_ALPHA;
	drtbd.SrcBlendAlpha = D3D12_BLEND_ONE;

	dgpsd.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	dgpsd.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	dgpsd.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dgpsd.InputLayout = { dinputeles.data(),(UINT)dinputeles.size() };
	dgpsd.NumRenderTargets = 1;
	dgpsd.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	dgpsd.pRootSignature = RootSignature.Get();
	dgpsd.RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	dgpsd.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	dgpsd.SampleDesc.Count = 1;
	dgpsd.SampleDesc.Quality = 0;
	dgpsd.SampleMask = UINT_MAX;
	dgpsd.VS = { reinterpret_cast<BYTE*>(vsshader->GetBufferPointer()),vsshader->GetBufferSize() };
	dgpsd.PS = { reinterpret_cast<BYTE*>(psshafer->GetBufferPointer()),psshafer->GetBufferSize() };

	if (FAILED(d3ddevice->CreateGraphicsPipelineState(&dgpsd, IID_PPV_ARGS(mpso.GetAddressOf()))))
	{
		return;
	}
	PsoMap.insert(make_pair("default", mpso));
	 
	Microsoft::WRL::ComPtr<ID3D12PipelineState> mirror;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> reflect;
	Microsoft::WRL::ComPtr<ID3D12PipelineState> transparetn;
	PsoMap.insert(make_pair("mirrorpso", mirror));
	PsoMap.insert(make_pair("reflectpso", reflect));
	PsoMap.insert(make_pair("transparetn", transparetn));

	D3D12_DEPTH_STENCIL_DESC mirrorDss;
	mirrorDss.DepthEnable = true;
	mirrorDss.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	mirrorDss.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	mirrorDss.StencilEnable = true;
	mirrorDss.StencilReadMask = 0xff;
	mirrorDss.StencilWriteMask = 0xff;

	mirrorDss.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	mirrorDss.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	mirrorDss.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	mirrorDss.FrontFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE;

	mirrorDss.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	mirrorDss.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	mirrorDss.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	mirrorDss.BackFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE;

	dgpsd.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	dgpsd.DepthStencilState = mirrorDss;

	if (FAILED(d3ddevice->CreateGraphicsPipelineState(&dgpsd, IID_PPV_ARGS(PsoMap["mirrorpso"].GetAddressOf()))))
	{
		return;
	}


	D3D12_DEPTH_STENCIL_DESC reflrctDSS;
	reflrctDSS.DepthEnable = true;
	reflrctDSS.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
	reflrctDSS.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	reflrctDSS.StencilEnable = true;
	reflrctDSS.StencilReadMask = 0xff;
	reflrctDSS.StencilWriteMask = 0xff;

	reflrctDSS.FrontFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	reflrctDSS.FrontFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	reflrctDSS.FrontFace.StencilFunc = D3D12_COMPARISON_FUNC_EQUAL;
	reflrctDSS.FrontFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE;

	reflrctDSS.BackFace.StencilDepthFailOp = D3D12_STENCIL_OP_KEEP;
	reflrctDSS.BackFace.StencilFailOp = D3D12_STENCIL_OP_KEEP;
	reflrctDSS.BackFace.StencilFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	reflrctDSS.BackFace.StencilPassOp = D3D12_STENCIL_OP_REPLACE;

	
	dgpsd.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	dgpsd.DepthStencilState = reflrctDSS;
	dgpsd.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	dgpsd.RasterizerState.FrontCounterClockwise = true;
	if (FAILED(d3ddevice->CreateGraphicsPipelineState(&dgpsd, IID_PPV_ARGS(PsoMap["reflectpso"].GetAddressOf()))))
	{
		return;
	}
	dgpsd.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	dgpsd.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	dgpsd.BlendState.RenderTarget[0] = drtbd;
	if (FAILED(d3ddevice->CreateGraphicsPipelineState(&dgpsd, IID_PPV_ARGS(PsoMap["transparetn"].GetAddressOf()))))
	{
		return;
	}

}



void XDirectT::Update()
{
	float x = mRadius * sinf(mPhi)*cosf(mTheta);
	float z = mRadius * sinf(mPhi)*sinf(mTheta);
	float y = mRadius * cosf(mPhi);

	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*3.1415926535f, 1280.f / 720.f, 1.0f, 1000.0f);
	XMStoreFloat4x4(&mProj, P);
	// Build the view matrix.
	XMVECTOR pos = XMVectorSet(x, y, z, 1.0f);
	XMVECTOR target = XMVectorZero();
	XMVECTOR up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	XMMATRIX view = XMMatrixLookAtLH(pos, target, up);
	XMStoreFloat4x4(&mView, view);

	XMMATRIX world = XMLoadFloat4x4(&mWorld);
	XMMATRIX proj = XMLoadFloat4x4(&mProj);
	XMMATRIX worldViewProj = world * view*proj;
	//XMMATRIX worldViewProj = world;

	// Update the constant buffer with the latest worldViewProj matrix.
	Matrix objConstants;
	XMStoreFloat4x4(&objConstants.WorldtoviewMatrix, XMMatrixTranspose(worldViewProj));
	
	objConstants1.Campos = XMFLOAT3(x, y, z);
	
	framresourcenum = framresourcenum%3;
	currentframeresource = frameResource[framresourcenum].get();

	if (currentframeresource->fense != 0 && commandfence->GetCompletedValue() < currentframeresource->fense)
	{
		HANDLE eventhandel = CreateEventEx(nullptr, false, false, EVENT_ALL_ACCESS);
		if (FAILED(commandfence->SetEventOnCompletion(FenseCount, eventhandel)))
		{
			return;
		}
		WaitForSingleObject(eventhandel, INFINITE);
		CloseHandle(eventhandel);

	}


	currentframeresource->objmatrix->CopyData(0, objConstants);
	currentframeresource->objmatrixa->CopyData(0, objConstants1);
}


void XDirectT::CreateTexture()
{
	text2d = make_unique<Tecture>();
	text2d->name = "text2d";
	text2d->filename= L"D:/bricks.dds";
	if (FAILED(CreateDDSTextureFromFile12(d3ddevice.Get(), CommandList.Get(), text2d->filename.c_str(), text2d->Resource, text2d->UploadReource)))
	{
		return ;
	}


}

std::array<const CD3DX12_STATIC_SAMPLER_DESC, 6> XDirectT::GetStaticSamplers()
{

	const CD3DX12_STATIC_SAMPLER_DESC pointWrap(
		0, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC pointClamp(
		1, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_POINT, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearWrap(
		2, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC linearClamp(
		3, // shaderRegister
		D3D12_FILTER_MIN_MAG_MIP_LINEAR, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP); // addressW

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicWrap(
		4, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_WRAP,  // addressW
		0.0f,                             // mipLODBias
		8);                               // maxAnisotropy

	const CD3DX12_STATIC_SAMPLER_DESC anisotropicClamp(
		5, // shaderRegister
		D3D12_FILTER_ANISOTROPIC, // filter
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressU
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressV
		D3D12_TEXTURE_ADDRESS_MODE_CLAMP,  // addressW
		0.0f,                              // mipLODBias
		8);                                // maxAnisotropy

	return {
		pointWrap, pointClamp,
		linearWrap, linearClamp,
		anisotropicWrap, anisotropicClamp };

}

void XDirectT::OnMouseMove(WPARAM btnState, int x, int y)
{

	if ((btnState & MK_LBUTTON) != 0)
	{
		// Make each pixel correspond to a quarter of a degree.
		float dx = XMConvertToRadians(0.25f*static_cast<float>(x - mLastMousePos.x));
		float dy = XMConvertToRadians(0.25f*static_cast<float>(y - mLastMousePos.y));

		// Update angles based on input to orbit camera around box.
		mTheta += dx;
		mPhi += dy;

		// Restrict the angle mPhi.
		mPhi = XMath::Clamp<float>(mPhi, 0.1f, 3.1415926535f - 0.1f);
		

	}
	else if ((btnState & MK_RBUTTON) != 0)
	{
		// Make each pixel correspond to 0.005 unit in the scene.
		float dx = 0.005f*static_cast<float>(x - mLastMousePos.x);
		float dy = 0.005f*static_cast<float>(y - mLastMousePos.y);

		// Update the camera radius based on input.
		mRadius += dx - dy;

		// Restrict the radius.
		mRadius = XMath::Clamp<float>(mRadius, 3.0f, 150.0f);
	}
	mLastMousePos.x = x;
	mLastMousePos.y = y;
}

void XDirectT::Draw()
{
	

	 

	if (FAILED(currentframeresource->commandallocator->Reset()))
	{
		return;
	}

	if (FAILED(CommandList->Reset(currentframeresource->commandallocator.Get(), mpso.Get())))
	{
		return;
	}
	
	CommandList->RSSetViewports(1, &mScreenViewport);
	CommandList->RSSetScissorRects(1, &mScissorRect);
	CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(SwpainChianBuff[CurrentBuffnum].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));


	CommandList->ClearRenderTargetView(CD3DX12_CPU_DESCRIPTOR_HANDLE(mrtvheap->GetCPUDescriptorHandleForHeapStart(), CurrentBuffnum, MrtvDescriptionsize), Colors::White, 0, nullptr);
	CommandList->ClearDepthStencilView(mdsvheap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	CommandList->OMSetRenderTargets(1, &CD3DX12_CPU_DESCRIPTOR_HANDLE(mrtvheap->GetCPUDescriptorHandleForHeapStart(), CurrentBuffnum, MrtvDescriptionsize), true, &(mdsvheap->GetCPUDescriptorHandleForHeapStart()));
	//CommandList->OMSetBlendFactor(D3D12_BLEND_INV_SRC_ALPHA);
	ID3D12DescriptorHeap* descriptorheap[] = { srvheap.Get()};
	CommandList->SetDescriptorHeaps(_countof(descriptorheap), descriptorheap);
	CD3DX12_GPU_DESCRIPTOR_HANDLE tex(srvheap->GetGPUDescriptorHandleForHeapStart());

	CommandList->SetGraphicsRootSignature(RootSignature.Get());
	CommandList->IASetVertexBuffers(0, 1, &boxMesh->getVertxView());
	CommandList->IASetIndexBuffer(&boxMesh->GetIndexView());
	CommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	CommandList->SetGraphicsRootConstantBufferView(0, currentframeresource->objmatrix->Getresource()->GetGPUVirtualAddress());
	CommandList->SetGraphicsRootConstantBufferView(1, currentframeresource->objmatrixa->Getresource()->GetGPUVirtualAddress());
	CommandList->SetGraphicsRootDescriptorTable(2, tex);
	CommandList->DrawIndexedInstanced(boxMesh->indexcount, 1, 0, 0, 0);
	CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(SwpainChianBuff[CurrentBuffnum].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	CommandList->Close();

	ID3D12CommandList* cmdlists[] = { CommandList.Get() };
	mCommandqueue->ExecuteCommandLists(_countof(cmdlists), cmdlists);


	HRESULT hr = swapchain->Present(0, 0);
	if (FAILED(hr))
	{
		return;
	}
	CurrentBuffnum = (CurrentBuffnum + 1) % 2;
	currentframeresource->fense = ++FenseCount;
	++framresourcenum;
	mCommandqueue->Signal(commandfence.Get(), FenseCount);
	
	//FlushCommand();


}



Microsoft::WRL::ComPtr<ID3DBlob> XDirectT::ShaderCompile(const wstring &filename,const string &pdefine,const string &ptarget)
{
	UINT16 compileFlags = 0;
	#if defined(DEBUG)||defined(_DEBUG)
		compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	#endif
		Microsoft::WRL::ComPtr<ID3DBlob> errormessage;
		Microsoft::WRL::ComPtr<ID3DBlob> bytecode;
		HRESULT hr = S_OK;
		hr= D3DCompileFromFile(filename.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, pdefine.c_str(), ptarget.c_str(), compileFlags, 0, bytecode.GetAddressOf(), errormessage.GetAddressOf());
		if (errormessage != nullptr)
		{
			OutputDebugStringA((char*)errormessage->GetBufferPointer());
		}
		if (FAILED(hr))
		{
			return NULL; 
		}

		return bytecode;


}

