#pragma once
//------------------------------------------------------------------------------
/**
@class Models::BillboardNodeInstance

A billboard node instance is represented as an individual renderable instance.

(C) 2013-2015 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "models/nodes/statenodeinstance.h"
#include "coregraphics/indexbuffer.h"
#include "coregraphics/vertexbuffer.h"
#include "coregraphics/vertexlayout.h"
#include "coregraphics/primitivegroup.h"
namespace Terrain
{
	#pragma pack(1)
	struct VertexData
	{
		float x, y, z, w;
		float nx, ny, nz;
		float u, v;
		VertexData(){}
		VertexData(float x1, float z1, float u1, float v1) : x(x1), y(0), z(z1), w(1), nx(0), ny(1), nz(0), u(u1), v(v1)
		{}
	};

	class TerrainNodeInstance : public Models::StateNodeInstance
	{
		__DeclareClass(TerrainNodeInstance);
	public:
		/// constructor
		TerrainNodeInstance();
		/// destructor
		virtual ~TerrainNodeInstance();

		/// called when visibility resolves
		virtual void OnVisibilityResolve(IndexT resolveIndex, float distToViewer);
		/// called when we render the billboard node
		virtual void Render();
		void GenerateTerrainBasedOnResolution(int width, int height);

	protected:
		/// called when the node sets up
		virtual void Setup(const Ptr<Models::ModelInstance>& inst, const Ptr<Models::ModelNode>& node, const Ptr<Models::ModelNodeInstance>& parentNodeInst);
		/// called when removed
		virtual void Discard();
		
		Util::Array<int> indices;
		//Util::Array<VertexData> vertexData;
		Util::Array<VertexData> vertexData;

		Ptr<CoreGraphics::VertexBuffer> vbo;
		Ptr<CoreGraphics::IndexBuffer> ibo;
		CoreGraphics::PrimitiveGroup primGroup;
		Ptr<CoreGraphics::VertexLayout> vertexLayout;
	};
} // namespace Models
//------------------------------------------------------------------------------