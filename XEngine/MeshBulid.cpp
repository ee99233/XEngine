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
			vertxs.push_back(std::move(XVertx4({ XMFLOAT3(x, y, z), XMFLOAT4(Colors::Cyan) })));
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
	vertxs.push_back(std::move(XVertx4({ XMFLOAT3(0.f, 0.f,minz), XMFLOAT4(Colors::Cyan) })));
	vertxs.push_back(std::move(XVertx4({ XMFLOAT3(0.f, 0.f,maxz), XMFLOAT4(Colors::Cyan) })));
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

void MeshBulid::CreateSphere(UINT r, UINT m, UINT n, OUT vector<XVertx4> &invertxs, OUT vector<UINT16>& inindex)
{

	float dphi = PI / n;
	float dth = 2 * PI / m;
	float du = 1.0f / m;
	float dv = 1.0f / n;
	for (int h = 1;h< n; ++h)
	{
		float v =1- dv * h;
 		float phi = PI-dphi * h;
		float sinr = sinf(phi)*r;
		for (int j = 0; j <= m; ++j)
		{
			float th = 2*PI-dth * j;
			float x = cosf(th) * sinr;
			float y = sinf(th)*sinr;
			float z = cosf(phi)*r;
			float u = du * j;
			invertxs.push_back(std::move(XVertx4({ XMFLOAT3(x, y, z), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(u,v) }))); 
		}
	}
	int rcount = m + 1;
	for (int h = 0; h < n-2; ++h)
	{
		for (int i = 0; i < m; ++i)
		{
			inindex.push_back(h*(rcount)+i);
			inindex.push_back((h+1)*rcount+i);
			inindex.push_back((h+1)*rcount+i+1);


			inindex.push_back(h*rcount+i);
			inindex.push_back((h+1)*rcount+i+1);
			inindex.push_back(h*rcount+i+1);

		}

	}
	float xr = r;


	

	invertxs.push_back(std::move(XVertx4({ XMFLOAT3(0, 0, -xr), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(0,0) })));
	invertxs.push_back(std::move(XVertx4({ XMFLOAT3(0, 0, xr), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(0,0) })));


	int index = invertxs.size();
	for (int i = 0; i < m; ++i)
	{
		
		inindex.push_back((n - 2)*rcount + i);
		inindex.push_back(index - 1);
		inindex.push_back((n - 2)*rcount + i+1);
		
	
		
		inindex.push_back(i+1);
		inindex.push_back(index - 2);
		inindex.push_back(i);
		
		
	}

	UINT Trinum = inindex.size();

	for (int i = 0; i < Trinum; i += 3)
	{
		
		XMFLOAT3 p1 = invertxs[inindex[i]].Pos;
		XMFLOAT3 p2 = invertxs[inindex[i+1]].Pos;
		XMFLOAT3 p3 = invertxs[inindex[i + 2]].Pos;

		XMFLOAT3 v1(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
		XMFLOAT3 v2(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z);

		XMFLOAT3 n = CrossTS(v1, v2);

		invertxs[inindex[i]].Normal.x += n.x;
		invertxs[inindex[i]].Normal.y += n.y;
		invertxs[inindex[i]].Normal.z += n.z;

		invertxs[inindex[i+1]].Normal.x += n.x;
		invertxs[inindex[i+1]].Normal.y += n.y;
		invertxs[inindex[i+1]].Normal.z += n.z;

		invertxs[inindex[i+2]].Normal.x += n.x;
		invertxs[inindex[i+2]].Normal.y += n.y;
		invertxs[inindex[i+2]].Normal.z += n.z;
	}
	

}

void MeshBulid::CreateBox(float w, float h, float d, OUT vector<XVertx4> &invertxs, OUT vector<UINT16>& inindex)
{

	float w2 = w / 2.0f;
	float h2 = h / 2.0f;
	float d2 = h / 2.0f;

	invertxs.reserve(24);
	invertxs.push_back(XVertx4({ XMFLOAT3(-w2, -h2, -d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(0.0f,1.0f) }));
	invertxs.push_back(XVertx4({ XMFLOAT3(-w2, h2, -d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(0.0f,0.0f) }));
	invertxs.push_back(XVertx4({ XMFLOAT3(w2, h2, -d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(1.0f,0.0f) }));
	invertxs.push_back(XVertx4({ XMFLOAT3(w2, -h2, -d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(1.0f,0.0f) }));

	invertxs.push_back(XVertx4({ XMFLOAT3(-w2, -h2, d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(1.0f,1.0f) }));
	invertxs.push_back(XVertx4({ XMFLOAT3(w2, -h2, d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(0.0f,1.0f) }));
	invertxs.push_back(XVertx4({ XMFLOAT3(w2, h2, d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(0.0f,0.0f) }));
	invertxs.push_back(XVertx4({ XMFLOAT3(-w2,h2,d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(1.0f,0.0f) }));

	invertxs.push_back(XVertx4({ XMFLOAT3(-w2, +h2, -d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(0.0f,1.0f) }));
	invertxs.push_back(XVertx4({ XMFLOAT3(-w2, h2, d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(0.0f,0.0f) }));
	invertxs.push_back(XVertx4({ XMFLOAT3(w2, h2, d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(1.0f,0.0f) }));
	invertxs.push_back(XVertx4({ XMFLOAT3(w2, h2, -d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(1.0f,1.0f) }));

	invertxs.push_back(XVertx4({ XMFLOAT3(-w2, -h2, -d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(1.0f,1.0f) }));
	invertxs.push_back(XVertx4({ XMFLOAT3(w2, -h2,-d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(0.0f,1.0f) }));
	invertxs.push_back(XVertx4({ XMFLOAT3(w2, -h2, +d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(0.0f,0.0f) }));
	invertxs.push_back(XVertx4({ XMFLOAT3(-w2, -h2, d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(1.0f,0.0f) }));

	invertxs.push_back(XVertx4({ XMFLOAT3(-w2, -h2, d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(0.0f,1.0f) }));
	invertxs.push_back(XVertx4({ XMFLOAT3(-w2, h2, d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(0.0f,0.0f) }));
	invertxs.push_back(XVertx4({ XMFLOAT3(-w2, h2, -d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(1.0f,0.0f) }));
	invertxs.push_back(XVertx4({ XMFLOAT3(-w2,-h2,-d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(1.0f,1.0f) }));

	invertxs.push_back(XVertx4({ XMFLOAT3(+w2, -h2, -d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(0.0f,1.0f) }));
	invertxs.push_back(XVertx4({ XMFLOAT3(+w2, h2, -d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(0.0f,0.0f) }));
	invertxs.push_back(XVertx4({ XMFLOAT3(w2, h2, d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(1.0f,0.0f) }));
	invertxs.push_back(XVertx4({ XMFLOAT3(w2, -h2, d2), XMFLOAT4(Colors::Cyan),XMFLOAT3(0,0,0),XMFLOAT2(1.0f,1.0f) }));

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

	for (int i = 0; i < inindex.size(); i += 3)
	{
		XMFLOAT3 p1 = invertxs[inindex[i]].Pos;
		XMFLOAT3 p2 = invertxs[inindex[i+1]].Pos;
		XMFLOAT3 p3 = invertxs[inindex[i+2]].Pos;
		XMFLOAT3 v1(p2.x - p1.x, p2.y - p1.y, p2.z - p1.z);
		XMFLOAT3 v2(p3.x - p1.x, p3.y - p1.y, p3.z - p1.z);
		XMFLOAT3 Normal = CrossTS(v1, v2);;
		invertxs[inindex[i]].Normal.x+= Normal.x;
		invertxs[inindex[i]].Normal.y += Normal.y;
		invertxs[inindex[i]].Normal.z += Normal.z;

		invertxs[inindex[i+1]].Normal.x += Normal.x;
		invertxs[inindex[i+1]].Normal.y += Normal.y;
		invertxs[inindex[i+1]].Normal.z += Normal.z;


		invertxs[inindex[i+2]].Normal.x += Normal.x;
		invertxs[inindex[i+2]].Normal.y += Normal.y;
		invertxs[inindex[i+2]].Normal.z += Normal.z;
	}

	


}

MeshBulid* MeshBulid::meshbulid=nullptr;

