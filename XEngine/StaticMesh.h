#pragma once
#include <d3d12.h>
#include <wrl.h>
#include <string>
#include <DirectXMath.h>
using namespace DirectX;
using std::string;

struct XMaterail
{
	XMFLOAT3 BaseColor;
	float Rougress;
	XMFLOAT3 Norml;
	float metallic;
};


class StaticMesh
{
public:

	StaticMesh();
	~StaticMesh();

	D3D12_VERTEX_BUFFER_VIEW getVertxView();

	D3D12_INDEX_BUFFER_VIEW GetIndexView();

	Microsoft::WRL::ComPtr<ID3D12Resource> Vertxbuff;
	Microsoft::WRL::ComPtr<ID3D12Resource> Indexbuff;
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadVertx;
	Microsoft::WRL::ComPtr<ID3D12Resource> UploadIndex;
	string filename;
	UINT VSizeInBytes;
	UINT VStrideInBytes;
	UINT ISizeInBytes;
	UINT indexcount;
	XMaterail xmat;
private:
	

};

