#include "StaticMesh.h"


StaticMesh::StaticMesh()
{

	



}

StaticMesh::~StaticMesh()
{

}

D3D12_VERTEX_BUFFER_VIEW StaticMesh::getVertxView()
{

	D3D12_VERTEX_BUFFER_VIEW dvbv;
	dvbv.BufferLocation = Vertxbuff->GetGPUVirtualAddress();
	dvbv.SizeInBytes = VSizeInBytes;
	dvbv.StrideInBytes = VStrideInBytes;
	return  dvbv;
	

}

D3D12_INDEX_BUFFER_VIEW StaticMesh::GetIndexView()
{
	D3D12_INDEX_BUFFER_VIEW didv;
	didv.BufferLocation = Indexbuff->GetGPUVirtualAddress();
	didv.Format = DXGI_FORMAT_R16_UINT;
	didv.SizeInBytes = ISizeInBytes;
	return didv;
}
