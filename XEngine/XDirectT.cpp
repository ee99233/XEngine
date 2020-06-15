#include "XDirectT.h"
#include "XWindow.h"
#include <vector>
#include <d3dcompiler.h>
#include "d3dx12.h"
#include <DirectXColors.h>
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

void XDirectT::InitVertxIndex()
{
	XVertx4 vertxs[8] = {
		XVertx4({ XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::White) }),
		XVertx4({ XMFLOAT3(-1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Black) }),
		XVertx4({ XMFLOAT3(+1.0f, +1.0f, -1.0f), XMFLOAT4(Colors::Red) }),
		XVertx4({ XMFLOAT3(+1.0f, -1.0f, -1.0f), XMFLOAT4(Colors::Green) }),
		XVertx4({ XMFLOAT3(-1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Blue) }),
		XVertx4({ XMFLOAT3(-1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Yellow) }),
		XVertx4({ XMFLOAT3(+1.0f, +1.0f, +1.0f), XMFLOAT4(Colors::Cyan) }),
		XVertx4({ XMFLOAT3(+1.0f, -1.0f, +1.0f), XMFLOAT4(Colors::Magenta) })
		
	};

	const UINT64 bytesize = 8 * sizeof(XVertx4);
	
	
	VertxGpuBuff = CreateDefaultBuff(bytesize, UploadGpuBuff, vertxs);
	int width1= VertxGpuBuff->GetDesc().Width;
	
	//CommandList->IASetVertexBuffers(0, 1, &dvbv);

	UINT16 index[] = 
	{ 0, 1, 2,
		0, 2, 3,

		// back face
		4, 6, 5,
		4, 7, 6,

		// left face
		4, 5, 1,
		4, 1, 0,

		// right face
		3, 2, 6,
		3, 6, 7,

		// top face
		1, 5, 6,
		1, 6, 2,

		// bottom face
		4, 0, 3,
		4, 3, 7 };
	const UINT16 inbytesize = 36*sizeof(UINT16);
	
	

	indexbuff = CreateDefaultBuff(inbytesize, uploadinbuff,index);
	int width2=indexbuff->GetDesc().Width;
	width1 = VertxGpuBuff->GetDesc().Width;
	//CommandList->IASetIndexBuffer(&dibv);


	
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
	dgpsd.PS = { reinterpret_cast<BYTE*>(psshafer->GetBufferPointer()),psshafer->GetBufferSize()};
	
	if (FAILED(d3ddevice->CreateGraphicsPipelineState(&dgpsd, IID_PPV_ARGS(mpso.GetAddressOf()))))
	{
		return;
	}
}

void XDirectT::BulidShader()
{
	vsshader = ShaderCompile(L"D:\\XEngine\\XEngine\\color.hlsl", "VS", "vs_5_0");
	psshafer = ShaderCompile(L"D:\\XEngine\\XEngine\\color.hlsl", "PS", "ps_5_0");

	dinputeles =
	{
		{"POSITION",0,DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"COLOR",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,12,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
	};


}

void XDirectT::CreateCbuff()
{

}

void XDirectT::initRootSingture()
{

	CD3DX12_ROOT_PARAMETER slotRootpa[1];
	CD3DX12_DESCRIPTOR_RANGE cbvtable;
	cbvtable.Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0);
	slotRootpa[0].InitAsDescriptorTable(1, &cbvtable);
	CD3DX12_ROOT_SIGNATURE_DESC rootsdesc(1, slotRootpa, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);
	Microsoft::WRL::ComPtr<ID3DBlob> serializedRootSig = nullptr;
	Microsoft::WRL::ComPtr<ID3DBlob> errorBlob=nullptr;
	HRESULT hr= D3D12SerializeRootSignature(&rootsdesc, D3D_ROOT_SIGNATURE_VERSION_1, serializedRootSig.GetAddressOf(),errorBlob.GetAddressOf());

	if (FAILED(d3ddevice->CreateRootSignature(0, serializedRootSig->GetBufferPointer(), serializedRootSig->GetBufferSize(), IID_PPV_ARGS(RootSignature.GetAddressOf()))))
	{
		return; 
	}

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
}

void XDirectT::BulidCostantBuff()
{

	WorldtoviewbuffPtr = make_unique<UploadBuff<Matrix>>(d3ddevice,1,true);
	D3D12_GPU_VIRTUAL_ADDRESS dgva=WorldtoviewbuffPtr->Getresource()->GetGPUVirtualAddress();
	D3D12_CONSTANT_BUFFER_VIEW_DESC dcbvd;
	
	dcbvd.BufferLocation = dgva;
	dcbvd.SizeInBytes = Calabuffer(sizeof(Matrix));
	d3ddevice->CreateConstantBufferView(&dcbvd, mcbvheap->GetCPUDescriptorHandleForHeapStart());

	

	

}

void XDirectT::Draw()
{
	D3D12_VERTEX_BUFFER_VIEW dvbv;
	dvbv.BufferLocation = VertxGpuBuff->GetGPUVirtualAddress();
	dvbv.StrideInBytes = sizeof(XVertx4);
	dvbv.SizeInBytes = 8* sizeof(XVertx4);

	int width1 = VertxGpuBuff->GetDesc().Width;

	D3D12_INDEX_BUFFER_VIEW dibv;
	dibv.BufferLocation = indexbuff->GetGPUVirtualAddress();
	dibv.SizeInBytes = 36 * sizeof(UINT16);
	dibv.Format = DXGI_FORMAT_R16_UINT;


	if (FAILED(CommandAlloctor->Reset()))
	{
		return;
	}

	if (FAILED(CommandList->Reset(CommandAlloctor.Get(), mpso.Get())))
	{
		return;
	}
	
	CommandList->RSSetViewports(1, &mScreenViewport);
	CommandList->RSSetScissorRects(1, &mScissorRect);
	CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(SwpainChianBuff[CurrentBuffnum].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));


	CommandList->ClearRenderTargetView(CD3DX12_CPU_DESCRIPTOR_HANDLE(mrtvheap->GetCPUDescriptorHandleForHeapStart(), CurrentBuffnum, MrtvDescriptionsize), Colors::Blue, 0, nullptr);
	CommandList->ClearDepthStencilView(mdsvheap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH | D3D12_CLEAR_FLAG_STENCIL, 1.0f, 0, 0, nullptr);

	CommandList->OMSetRenderTargets(1, &CD3DX12_CPU_DESCRIPTOR_HANDLE(mrtvheap->GetCPUDescriptorHandleForHeapStart(), CurrentBuffnum, MrtvDescriptionsize), true, &(mdsvheap->GetCPUDescriptorHandleForHeapStart()));

	ID3D12DescriptorHeap* descriptorheap[] = {mcbvheap.Get()};
	CommandList->SetDescriptorHeaps(_countof(descriptorheap), descriptorheap);

	CommandList->SetGraphicsRootSignature(RootSignature.Get());
	CommandList->IASetVertexBuffers(0, 1, &dvbv);
	CommandList->IASetIndexBuffer(&dibv);
	CommandList->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	CommandList->SetGraphicsRootDescriptorTable(0, mcbvheap->GetGPUDescriptorHandleForHeapStart());

	CommandList->DrawIndexedInstanced(36, 1, 0, 0, 0);

	CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(SwpainChianBuff[CurrentBuffnum].Get(),
		D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	CommandList->Close();

	ID3D12CommandList* cmdlists[] = { CommandList.Get() };
	mCommandqueue->ExecuteCommandLists(_countof(cmdlists), cmdlists);
	
	HRESULT hr= swapchain->Present(0, 0);
	
	CurrentBuffnum = (CurrentBuffnum + 1) % 2;
	FlushCommand();


}

void XDirectT::Update()
{
	float x = mRadius * sinf(mPhi)*cosf(mTheta);
	float z = mRadius * sinf(mPhi)*sinf(mTheta);
	float y = mRadius * cosf(mPhi);

	XMMATRIX P = XMMatrixPerspectiveFovLH(0.25f*3.1415926535f, 1280.f/720.f, 1.0f, 1000.0f);
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

	WorldtoviewbuffPtr->CopyData(0, objConstants);
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

Microsoft::WRL::ComPtr<ID3D12Resource> XDirectT::CreateDefaultBuff(UINT64 bytesize, Microsoft::WRL::ComPtr<ID3D12Resource>& uploadbufff,void* initData)
{



	
	Microsoft::WRL::ComPtr<ID3D12Resource> Default;
	D3D12_HEAP_PROPERTIES dhp;
	dhp.Type = D3D12_HEAP_TYPE_DEFAULT;
	dhp.VisibleNodeMask = 1;
	dhp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	dhp.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	dhp.CreationNodeMask = 1;
	D3D12_RESOURCE_DESC drd;
	drd.Alignment = 0;
	drd.DepthOrArraySize = 1;
	drd.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	drd.Flags = D3D12_RESOURCE_FLAG_NONE;
	drd.Format = DXGI_FORMAT_UNKNOWN;
	drd.Height = 1;
	drd.Width = bytesize;
	drd.MipLevels = 1;
	drd.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	drd.SampleDesc.Count = 1;
	drd.SampleDesc.Quality = 0;

	if (FAILED(d3ddevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT), D3D12_HEAP_FLAG_NONE,&CD3DX12_RESOURCE_DESC::Buffer(bytesize), D3D12_RESOURCE_STATE_COMMON, nullptr, IID_PPV_ARGS(Default.GetAddressOf()))))
	{
		return NULL;
	}
	D3D12_HEAP_PROPERTIES dhpd;
	dhpd.Type = D3D12_HEAP_TYPE_UPLOAD;
	dhpd.VisibleNodeMask = 1;
	dhpd.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	dhpd.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	dhpd.CreationNodeMask = 1;
	if (FAILED(d3ddevice->CreateCommittedResource(&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD), D3D12_HEAP_FLAG_NONE, &CD3DX12_RESOURCE_DESC::Buffer(bytesize), D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(uploadbufff.GetAddressOf()))))
	{
		return NULL;
	}
	D3D12_RESOURCE_BARRIER brd;
	brd.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
	brd.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_DEST;
	brd.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	brd.Transition.pResource = Default.Get();
	brd.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	brd.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;

	D3D12_SUBRESOURCE_DATA subreourceData = {};
	subreourceData.pData = initData;
	subreourceData.RowPitch = bytesize;
	subreourceData.SlicePitch = subreourceData.RowPitch;
	CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(Default.Get(),
		D3D12_RESOURCE_STATE_COMMON, D3D12_RESOURCE_STATE_COPY_DEST));
	UpdateSubresources<1>(CommandList.Get(), Default.Get(), uploadbufff.Get(), 0, 0, 1, &subreourceData);
	CommandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(Default.Get(), D3D12_RESOURCE_STATE_COPY_DEST, D3D12_RESOURCE_STATE_GENERIC_READ));
	return Default;
	
	
}
