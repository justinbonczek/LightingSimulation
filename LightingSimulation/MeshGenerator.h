//
// Class with static methods to generate primitive meshes
// Credit to Frank D Luna
//

#ifndef MESHGENERATOR_H
#define MESHGENERATOR_H

#include "Mesh.h"

class MeshGenerator
{
public:
	static void CreateSphere(float radius, UINT numSubdivisions, MeshData& data);
	static void CreatePlane(float width, float depth, UINT n, UINT m, MeshData& data);
private:
	static void Subdivide(MeshData& mesh);
};

#endif