#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <string>
using std::string;
class StaticMesh
{
public:

	D3D12_VERTEX_BUFFER_VIEW getVertxView();

	D3D12_INDEX_BUFFER_VIEW GetIndexView();

private:
	Microsoft::WRL::ComPtr<ID3D12Resource> Vertxbuff;
	Microsoft::WRL::ComPtr<ID3D12Resource> Indexbuff;
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadVertx;
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadIndex;
	string filename;
	UINT VSizeInBytes;
	UINT VStrideInBytes;
	UINT ISizeInBytes;

};

