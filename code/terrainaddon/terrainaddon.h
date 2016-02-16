#pragma once
//------------------------------------------------------------------------------
/**
@class Terrain::TerrainAddon

Renders terrain with specified resolution based on height-map

(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"
#include "core/singleton.h"
#include "graphics/modelentity.h"
#include "models/nodes/shapenodeinstance.h"
#include "models/nodes/shapenode.h"
#include "brush.h"

namespace Terrain
{
	#pragma pack(1)
	struct VertexData
	{
		float x, z;
		float u, v;
		VertexData(){}
		VertexData(float x1, float z1, float u1, float v1) : x(x1), z(z1), u(u1), v(v1)
		{}
	};

	class TerrainAddon : public Core::RefCounted
	{
		__DeclareClass(TerrainAddon);
		__DeclareSingleton(TerrainAddon);
	public:
		/// constructor
		TerrainAddon();
		/// destructor
		virtual ~TerrainAddon();

		/// setup
		void Setup(Ptr<Graphics::Stage> stage);
		/// discard
		void Discard();

		void LoadShader();

		void GenerateTerrainBasedOnResolution();
		void UpdateTerrainMesh();
		void SetUpVBO();

		void InitializeTexture();
		void UpdateTexture(void* data, SizeT size, SizeT width, SizeT height, IndexT left, IndexT top, IndexT mip);
		void CreateTerrainEntity();

		Ptr<Graphics::ModelEntity> GetTerrainEntity();
		Ptr<CoreGraphics::Mesh> GetTerrainMesh();

		void SetHeightMultiplier(int multiplier);

		void UpdateTerrainWithNewSize(int width, int height);
		void UpdateWorldSize();

		void UpdateTerrainAtPos(const Math::float4& pos, const KeyMod modifier);
		bool IsMouseOnTerrain(const Math::float4& pos);
	private:
		Ptr<Graphics::Stage> stage;

		int width, height;
		int heightMultiplier;
		float maxHeight = 1024;

		// mesh
		Util::Array<int> indices;
		Util::Array<VertexData> vertexData;

		Util::Array<CoreGraphics::PrimitiveGroup> primitiveGroups;
		Ptr<CoreGraphics::VertexBuffer> vbo;
		Ptr<CoreGraphics::IndexBuffer> ibo;
		Ptr<CoreGraphics::VertexLayout> vertexLayout;

		// shader variables
		Ptr<Materials::SurfaceConstant> heightMultiplierHandle;
		Ptr<Materials::SurfaceConstant> samplerHeightMapHandle;
		Ptr<Materials::SurfaceConstant> transformHandle;

		Ptr<CoreGraphics::Texture> memoryHeightTexture;
		float *rHeightBuffer;

		Ptr<Graphics::ModelEntity> terrainModelEnt;
		Ptr<CoreGraphics::Mesh> terrainMesh;
		Ptr<Models::ShapeNodeInstance> terrainShapeNodeInstance;
		Ptr<Models::ShapeNode> terrainShapeNode;

		Ptr<Terrain::Brush> currentBrush;
	};
} // namespace Grid