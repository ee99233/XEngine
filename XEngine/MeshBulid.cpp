#include "MeshBulid.h"
#include <corecrt_math.h>
#include <DirectXColors.h>

#define PI 3.1415926

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

MeshBulid* MeshBulid::meshbulid=nullptr;

