#include "MeshBulid.h"
#include <corecrt_math.h>
#include <DirectXColors.h>
#define PI 3.1415926
using namespace DirectX;
MeshBulid::MeshBulid()
{

}

MeshBulid::~MeshBulid()
{
	if (meshbulid != nullptr)
	{
		delete meshbulid;
		meshbulid = nullptr;
	}

}

MeshBulid* MeshBulid::GetMeshBulid()
{

	if (meshbulid != nullptr)
	{
		meshbulid = new MeshBulid();
	}
	return meshbulid;


}

void MeshBulid::CreateCyliner(float topr, float bottomr, UINT xrcount, UINT statckcount, float High,OUT vector<XVertx4> & vertxs,OUT vector<UINT16>& indess)
{

	float dr = (topr-bottomr)/statckcount;
	float dh = High / statckcount;
	float dw = (2 * PI) / float(xrcount);
	for (UINT i = 0; i < statckcount+1; ++i)
	{
		float z = -0.5f * High + i * dh;
		float r = bottomr + dr * i;
		for (UINT j = 0; j < xrcount; ++j)
		{
			float mTheta = dw * j;
			float x = r * cosf(mTheta);
			float y = r * sinf(mTheta);
			//vertxs.push_back(std::move(XVertx4({ XMFLOAT3(x, y, z), XMFLOAT4(Colors::Cyan) })));
		}
	}
	UINT cylinerindex = vertxs.size();
	for (UINT y = 0; y < statckcount+1 ; y++)
	{
		for (UINT x = 0; x < xrcount; x++)
		{
			if (x < xrcount - 1)
			{
				indess.push_back(xrcount*y + x);
				indess.push_back(xrcount*y + x + 1);
				indess.push_back(xrcount*(y + 1) + (x + 1));
				indess.push_back(xrcount*y + x);
				indess.push_back(xrcount*(y + 1) + (x + 1));
				indess.push_back(xrcount*(y + 1) + x);
			}
			else
			{
				indess.push_back(xrcount*y + x);
				indess.push_back(xrcount*y);
				indess.push_back(xrcount*(y + 1));
				indess.push_back(xrcount*y + x);
				indess.push_back(xrcount*(y + 1));
				indess.push_back(xrcount*(y + 1) + x);
			}
			
		}
	}

	float maxz = -0.5f * High + statckcount * dh;
	float minz = -0.5f * High;
	//vertxs.push_back(std::move(XVertx4({ XMFLOAT3(0.f, 0.f,minz), XMFLOAT4(Colors::Cyan) })));
	//vertxs.push_back(std::move(XVertx4({ XMFLOAT3(0.f, 0.f,maxz), XMFLOAT4(Colors::Cyan) })));
	for (int i = 0; i < xrcount;i++)
	{
		
		if (i < xrcount - 1)
		{
			indess.push_back(statckcount*xrcount + i);
			indess.push_back(statckcount*xrcount + i + 1);
			indess.push_back(vertxs.size() - 1);

			indess.push_back(0 * xrcount + i);
			indess.push_back(0 * xrcount + i + 1);
			indess.push_back(vertxs.size() - 2);
		}
		else
		{
			indess.push_back(statckcount*xrcount + i);
			indess.push_back(statckcount*xrcount);
			indess.push_back(vertxs.size() - 1);

			indess.push_back(0 * xrcount + i);
			indess.push_back(0);
			indess.push_back(vertxs.size() - 2);
		}

	}
	


	
}

void MeshBulid::CreateGrid(UINT width, UINT height, UINT m, UINT n, OUT vector<XVertx4> &invertxs, OUT vector<UINT16>& inindex)
{

	float dx = (float)width / (float)(m);
	float dy = (float)height / float(n);
	float centerx = (float)width * -0.5f;
	float centery = (float)height * -0.5f;
	for (int j = 0; j < n; ++j)
	{
		for (int i = 0; i < m; ++i)
		{

			float x = centerx + dx * i;
			float y = centery + dy * j;
			float z = 2.0f;
			invertxs.push_back(std::move(XVertx4({ XMFLOAT3(x, y, z), XMFLOAT4(Colors::Cyan)})));
		}
	}

	for (int j = n - 1; j > 0; --j)
	{
		for (int i = 0; i < m - 1;++i)
		{
			inindex.push_back(j*m + i);
			inindex.push_back((j - 1)*m + i);
			inindex.push_back((j-1)*m+i+1);

			inindex.push_back(j*m + i);
			inindex.push_back((j - 1)*m + i + 1);
			inindex.push_back(j*m+i+1);

		}
	}



}

void MeshBulid::CreateSphere(INT r, UINT m, UINT n, OUT vector<XVertx4> &invertxs, OUT vector<UINT>& inindex)
{

	XVertx4 TopVertx(0.0, 0.0, r, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	XVertx4 BottomVertx(0.0, 0.0, -r, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f);
	invertxs.push_back(TopVertx);

	float thetastep = 2.0 * XM_PI / m;
	float phistep = XM_PI / n;

	for (int i = 1; i <= n - 1; i++)
	{
		float phi = phistep * i;
		for (int j = 0; j <= m; j++)
		{
			float theta = thetastep * j;

			XVertx4 v;
			v.Pos.x = r * sinf(phi) * cosf(theta);
			v.Pos.y = r * sinf(phi) * sinf(theta);
			v.Pos.z = r * cosf(phi);

			v.Tangent.x = -r * sinf(phi) * sinf(theta);
			v.Tangent.y = r * sinf(phi) * cosf(theta);
			v.Tangent.z = 0;
			XMVECTOR T = XMLoadFloat3(&v.Tangent);
			XMStoreFloat3(&v.Tangent, XMVector3Normalize(T));

			XMVECTOR p = XMLoadFloat3(&v.Pos);
			XMStoreFloat3(&v.Normal, XMVector3Normalize(p));

			v.TextCord.x = theta / XM_2PI;
			v.TextCord.y = phi / XM_PI;
			invertxs.push_back(v);	
		}
	}
	invertxs.push_back(BottomVertx);
	for (UINT i = 1; i <= m; ++i)
	{
		inindex.push_back(0);
		inindex.push_back(i + 1);
		inindex.push_back(i);
	}
	UINT baseindex = 1;
	UINT Ringcount = m + 1;
	for (UINT i = 0; i < n-2; ++i)
	{
		for (UINT j = 0; j < m; ++j)
		{
			inindex.push_back(baseindex + i * Ringcount + j);
			inindex.push_back(baseindex + i * Ringcount + j+1);
			inindex.push_back(baseindex + (i+1) * (Ringcount) + j);

			inindex.push_back(baseindex + (i+1) * Ringcount + j);
			inindex.push_back(baseindex + i * Ringcount + j + 1);
			inindex.push_back(baseindex + (i+1) * (Ringcount) + j+1);
		}
	}
	UINT SourcePoleinex = invertxs.size() - 1;
	baseindex = SourcePoleinex - Ringcount;
	for (UINT i=0; i < m; ++i)
	{
		inindex.push_back(SourcePoleinex);
		inindex.push_back(baseindex + i);
		inindex.push_back(baseindex + i + 1);
	}
}

void MeshBulid::CreateBox(float w, float h, float d, OUT vector<XVertx4> &invertxs, OUT vector<UINT>& inindex)
{

	float w2 = w / 2.0f;
	float h2 = h / 2.0f;
	float d2 = h / 2.0f;

	invertxs.reserve(24);
	invertxs.push_back(XVertx4(-w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f));
	invertxs.push_back(XVertx4(-w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	invertxs.push_back(XVertx4(+w2, +h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f));
	invertxs.push_back(XVertx4(+w2, -h2, -d2, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f));

	invertxs.push_back(XVertx4(-w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f));
	invertxs.push_back(XVertx4(+w2, -h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f));
	invertxs.push_back(XVertx4(+w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	invertxs.push_back(XVertx4(-w2, +h2, +d2, 0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f));

	invertxs.push_back(XVertx4(-w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f));
	invertxs.push_back(XVertx4(-w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	invertxs.push_back(XVertx4(+w2, +h2, +d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f));
	invertxs.push_back(XVertx4(+w2, +h2, -d2, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f));

	invertxs.push_back(XVertx4(-w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f));
	invertxs.push_back(XVertx4(+w2, -h2, -d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f));
	invertxs.push_back(XVertx4(+w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f));
	invertxs.push_back(XVertx4(-w2, -h2, +d2, 0.0f, -1.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f));

	invertxs.push_back(XVertx4(-w2, -h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f));
	invertxs.push_back(XVertx4(-w2, +h2, +d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f));
	invertxs.push_back(XVertx4(-w2, +h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f));
	invertxs.push_back(XVertx4(-w2, -h2, -d2, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f));

	invertxs.push_back(XVertx4(+w2, -h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f));
	invertxs.push_back(XVertx4(+w2, +h2, -d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f));
	invertxs.push_back(XVertx4(+w2, +h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f));
	invertxs.push_back(XVertx4(+w2, -h2, +d2, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f));

	inindex.reserve(36);
	inindex.push_back(0); inindex.push_back(1); inindex.push_back(2);
	inindex.push_back(0); inindex.push_back(2); inindex.push_back(3);
	inindex.push_back(4); inindex.push_back(5); inindex.push_back(6);
	inindex.push_back(4); inindex.push_back(6); inindex.push_back(7);
	inindex.push_back(8); inindex.push_back(9); inindex.push_back(10);
	inindex.push_back(8); inindex.push_back(10); inindex.push_back(11);
	inindex.push_back(12); inindex.push_back(13); inindex.push_back(14);
	inindex.push_back(12); inindex.push_back(14); inindex.push_back(15);
	inindex.push_back(16); inindex.push_back(17); inindex.push_back(18);
	inindex.push_back(16); inindex.push_back(18); inindex.push_back(19);
	inindex.push_back(20); inindex.push_back(21); inindex.push_back(22);
	inindex.push_back(20); inindex.push_back(22); inindex.push_back(23);


	


}

MeshBulid* MeshBulid::meshbulid=nullptr;

