#include "XDirectT.h"
#include "XWindow.h"
#include <vector>
#include <string>
#pragma comment(lib,"D3D12.lib")
#pragma comment(lib,"DXGI.lib")

using namespace std;
XDirectT* XDirectT::xdirectx = nullptr;
XDirectT::XDirectT()
{
	/*#if defined(DEBUG)||defined(_DEBUG)
	{
		Microsoft::WRL::ComPtr<ID3D12Debug> debugcontrol;
		if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(debugcontrol.GetAddressOf()))))
		{
			return;
		}
		debugcontrol->EnableDebugLayer();
	}
	#endif

	if (FAILED(CreateDXGIFactory1(IID_PPV_ARGS(dxfactory.GetAddressOf()))))
	{
		return;
	}

	HRESULT hardware = D3D12CreateDevice(nullptr, D3D_FEATURE_LEVEL_11_0, IID_PPV_ARGS(d3ddevice.GetAddressOf()));
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

	}*/

	
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


}

void XDirectT::CreateSwapchain()
{


	swapchain.Reset();
	DXGI_SWAP_CHAIN_DESC sd;
	sd.BufferDesc.Width = XWindow::GetXwindow()->Width;
	sd.BufferDesc.Height = XWindow::GetXwindow()->Height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Numerator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 2;
	sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	sd.OutputWindow = XWindow::GetXwindow()->ghMainWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	if (FAILED(dxfactory->CreateSwapChain(d3ddevice.Get(), &sd, swapchain.GetAddressOf())))
	{
		return;
	}




}

void XDirectT::CreateD3dview()
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvhandle(mrtvheap->GetCPUDescriptorHandleForHeapStart());


	for (int i = 0; i < buffcout; ++i)
	{
		HRESULT hresult = swapchain->GetBuffer(i, IID_PPV_ARGS(SwpainChianBuff[i].GetAddressOf()));
		if (FAILED(hresult))
		{
			return;
		}

		d3ddevice->CreateRenderTargetView(SwpainChianBuff[i].Get(), nullptr, rtvhandle);
		

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
	dd.SampleDesc.Count = 0;
	dd.SampleDesc.Quality = 1;

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
	D3D12_CPU_DESCRIPTOR_HANDLE dehandel(mdsvheap->GetCPUDescriptorHandleForHeapStart());
	d3ddevice->CreateDepthStencilView(DepthStencilView.Get(), nullptr, dehandel);

}

void XDirectT::CalcFrame()
{
	static int frameCnt = 0;
	static float timeElaped = 0.0f;
	++frameCnt;
	if (gameitimer.Totaltime() - timeElaped >= 1.0f)
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

void XDirectT::CreateDefaultBuff()
{
	D3D12_INPUT_ELEMENT_DESC died[] =
	{
		{"Postion",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},
		{"Color",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,16,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0},

	};

}
