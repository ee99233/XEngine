#pragma once
#include <d3d12.h>
#include <vector>
#include "XMath.h"
using namespace std;
using namespace XMath;
class MeshBulid
{
public:
	MeshBulid();
	~MeshBulid();
	static MeshBulid* GetMeshBulid();
	
	void CreateCyliner(float topr, float bottomr, UINT xrcount, UINT statckcount, float High, OUT vector<XVertx4> & vertxs,OUT vector<UINT16>& indess);
private:
	static MeshBulid* meshbulid;

};

