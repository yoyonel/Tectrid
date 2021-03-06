﻿#include "GenPyramid.h"
#include "Mesh.h"
#include "SculptEngine.h"

Mesh* GenPyramid::Generate(float width, float height, float depth)
{
	GenVertices(width, height, depth);
	Gentriangles();
	Mesh* mesh = new Mesh(_triangles, _vertices, -1, true, false, true, true, true);
	return mesh;
}

void GenPyramid::GenVertices(float width, float height, float depth)
{
	float sideLength = 1;
	int nbVtxPerFace = _nbVertexPerLine * _nbVertexPerLine;
	_vertices.resize(nbVtxPerFace * 5);
	float absLimit = sideLength / 2.0f;
	float step = sideLength / (float) (_nbVertexPerLine - 1);

	for(int i = 0; i < _nbVertexPerLine; ++i)
	{
		for(int j = 0; j < _nbVertexPerLine; ++j)
		{
			int index = i * _nbVertexPerLine + j;
			float coo1 = -absLimit + step * i;
			float coo2 = -absLimit + step * j;
			Vector3 vtx(coo1, coo2, absLimit);
			// Face 1
			_vertices[index].x = vtx.x;
			_vertices[index].y = -vtx.z;
			_vertices[index].z = vtx.y;
			// Face 2
			index += nbVtxPerFace;
			_vertices[index].x = vtx.x;
			_vertices[index].y = vtx.y;
			_vertices[index].z = -vtx.z;
			// Face 3
			index += nbVtxPerFace;
			_vertices[index].x = vtx.x;
			_vertices[index].y = vtx.y;
			_vertices[index].z = vtx.z;
			// Face 4
			index += nbVtxPerFace;
			_vertices[index].x = -vtx.z;
			_vertices[index].y = vtx.x;
			_vertices[index].z = vtx.y;
			// Face 5
			index += nbVtxPerFace;
			_vertices[index].x = vtx.z;
			_vertices[index].y = vtx.x;
			_vertices[index].z = vtx.y;
		}
	}
	Vector3 scale(width, height, depth);
	for(Vector3& vertex : _vertices)
	{
		vertex *= scale;
		float factor = lerp(1.0f, 0.0f, (vertex.y + (height * 0.5f)) / height);
		vertex.x *= factor;
		vertex.z *= factor;
	}
}

void GenPyramid::Gentriangles()
{
	int nbVtxPerFace = _nbVertexPerLine * _nbVertexPerLine;
	int nbTriPerFace = 3 * 2 * ((_nbVertexPerLine - 1) * (_nbVertexPerLine - 1));
	_triangles.resize(5 * nbTriPerFace);
	int flipEvenOdd = SculptEngine::IsTriangleOrientationInverted() ? 0 : 1;
	for(int i = 0; i < _nbVertexPerLine - 1; ++i)
	{
		for(int j = 0; j < _nbVertexPerLine - 1; ++j)
		{
			for(int k = 0; k < 5; ++k)
			{
				int index = (3 * 2 * (i * (_nbVertexPerLine - 1) + j)) + k * nbTriPerFace;
				int vtx1 = i * _nbVertexPerLine + j + k * nbVtxPerFace;
				int vtx2 = vtx1 + 1;
				int vtx3 = (i + 1) * _nbVertexPerLine + j + k * nbVtxPerFace;
				int vtx4 = vtx3 + 1;
				if(k % 2 != flipEvenOdd)
				{   // Flip triangles one over two face
					++vtx1; --vtx2; ++vtx3; --vtx4;
				}
				// First quad's triangle
				_triangles[index++] = vtx1;
				_triangles[index++] = vtx2;
				_triangles[index++] = vtx3;
				// Second quad's triangle
				_triangles[index++] = vtx3;
				_triangles[index++] = vtx2;
				_triangles[index++] = vtx4;
			}
		}
	}
}

#ifdef __EMSCRIPTEN__ 
#include <emscripten/bind.h>
using namespace emscripten;

EMSCRIPTEN_BINDINGS(GenPyramid)
{
	class_<GenPyramid>("GenPyramid")
		.constructor<>()
		.function("Generate", &GenPyramid::Generate, allow_raw_pointers());
}
#endif // __EMSCRIPTEN__
