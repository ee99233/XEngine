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
	void CreateGrid(UINT width, UINT height,UINT m,UINT n,OUT vector<XVertx4> &invertxs,OUT vector<UINT16>& inindex);
	void CreateSphere(UINT r,UINT m, UINT n, OUT vector<XVertx4> &invertxs, OUT vector<UINT16>& inindex);
	void CreateBox(float w, float h, float d, OUT vector<XVertx4> &invertxs, OUT vector<UINT16>& inindex);
private:
	static MeshBulid* meshbulid;

};

