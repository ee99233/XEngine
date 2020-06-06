#include "XDirectT.h"
#include <vector>

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
	if (!FAILED(d3ddevice->CreateCommandQueue(&commandqueuedesc, IID_PPV_ARGS(mCommandqueue.GetAddressOf()))))
	{
		return;
	}

	if (!FAILED(d3ddevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(CommandAlloctor.GetAddressOf()))))
	{
		return ;
	}

	if (!FAILED(d3ddevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, CommandAlloctor.Get(), nullptr, IID_PPV_ARGS(CommandList.GetAddressOf()))))
	{
		return;
	}

	CommandList->Close();
	if (!FAILED(d3ddevice->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(commandfence.GetAddressOf()))))
	{
		return;
	}



}

void XDirectT::FlushCommand()
{

	++FenseCount;
	if (!FAILED(mCommandqueue->Signal(commandfence.Get(), FenseCount)))
	{
		return;
	}

	if (commandfence->GetCompletedValue() < FenseCount)
	{
		HANDLE eventhandel = CreateEventEx(nullptr,false,false,EVENT_ALL_ACCESS);
		if (!FAILED(commandfence->SetEventOnCompletion(FenseCount, eventhandel)))
		{
			return;
		}
		WaitForSingleObject(eventhandel, INFINITE);
		CloseHandle(eventhandel);
	}


}
