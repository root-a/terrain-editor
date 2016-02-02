//------------------------------------------------------------------------------
//  billboardnodeinstance.cc
//  (C) 2013-2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "terrainnodeinstance.h"
#include "models/modelnode.h"
#include "models/nodes/transformnode.h"
#include "models/nodes/statenodeinstance.h"
#include "models/modelnodeinstance.h"
#include "coregraphics/vertexbuffer.h"
#include "coregraphics/memoryvertexbufferloader.h"
#include "coregraphics/vertexcomponent.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/vertexlayoutserver.h"
#include "coregraphics/memoryindexbufferloader.h"
#include "resources/resourceloader.h"
#include "coregraphics/renderdevice.h"

using namespace CoreGraphics;
using namespace Util;
using namespace Resources;
using namespace Models;
namespace Terrain
{
	__ImplementClass(Terrain::TerrainNodeInstance, 'TRNI', Models::StateNodeInstance);

	//------------------------------------------------------------------------------
	/**
	*/
	TerrainNodeInstance::TerrainNodeInstance() :
		vbo(0),
		ibo(0),
		vertexLayout(0)
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	TerrainNodeInstance::~TerrainNodeInstance()
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		TerrainNodeInstance::OnVisibilityResolve(IndexT resolveIndex, float distToViewer)
	{
		// check if node is inside lod distances or if no lod is used
		const Ptr<TransformNode>& transformNode = this->modelNode.downcast<TransformNode>();
		if (transformNode->CheckLodDistance(distToViewer))
		{
			this->modelNode->AddVisibleNodeInstance(resolveIndex, this->surfaceInstance->GetCode(), this);
			ModelNodeInstance::OnVisibilityResolve(resolveIndex, distToViewer);
		}
	}
	
	//------------------------------------------------------------------------------
	/**
	*/
	void
		TerrainNodeInstance::Setup(const Ptr<ModelInstance>& inst, const Ptr<ModelNode>& node, const Ptr<ModelNodeInstance>& parentNodeInst)
	{
		n_assert(!this->vbo.isvalid());
		n_assert(!this->ibo.isvalid());
		n_assert(!this->vertexLayout.isvalid());

		// up to parent class
		StateNodeInstance::Setup(inst, node, parentNodeInst);
	
		GenerateTerrainBasedOnResolution(100, 100);

		// setup VBO
		Util::Array<VertexComponent> components;
		components.Append(VertexComponent(VertexComponent::Position, 0, VertexComponent::Float4, 0));
		components.Append(VertexComponent(VertexComponent::Normal, 0, VertexComponent::Float3, 0));
		components.Append(VertexComponent(VertexComponent::TexCoord1, 0, VertexComponent::Float2, 0));
		Ptr<MemoryVertexBufferLoader> vboLoader = MemoryVertexBufferLoader::Create();
		int vertCount = vertexData.Size();
		int sizeofstruct = sizeof(VertexData);
		vboLoader->Setup(components, vertCount, vertexData.Begin(), vertCount*sizeof(VertexData), VertexBuffer::UsageImmutable, VertexBuffer::AccessNone);

		this->vbo = VertexBuffer::Create();
		this->vbo->SetLoader(vboLoader.upcast<ResourceLoader>());
		this->vbo->SetAsyncEnabled(false);
		this->vbo->Load();
		n_assert(this->vbo->IsLoaded());
		this->vbo->SetLoader(NULL);

		Ptr<MemoryIndexBufferLoader> iboLoader = MemoryIndexBufferLoader::Create();
		int indicesCount = indices.Size();
		iboLoader->Setup(IndexType::Index32, indicesCount, indices.Begin(), indicesCount*sizeof(int));

		this->ibo = IndexBuffer::Create();
		this->ibo->SetLoader(iboLoader.upcast<ResourceLoader>());
		this->ibo->SetAsyncEnabled(false);
		this->ibo->Load();
		n_assert(this->ibo->IsLoaded());
		this->ibo->SetLoader(NULL);

		// setup the primitive group
		this->primGroup.SetBaseIndex(0);
		this->primGroup.SetNumVertices(vertCount);
		this->primGroup.SetBaseIndex(0);
		this->primGroup.SetNumIndices(indicesCount);
		this->primGroup.SetPrimitiveTopology(PrimitiveTopology::TriangleList);

		// setup ibo
		this->vertexLayout = this->vbo->GetVertexLayout();
		this->vertexLayout->SetIndexBuffer(this->ibo);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		TerrainNodeInstance::Discard()
	{
		n_assert(this->vbo->IsLoaded());
		n_assert(this->ibo->IsLoaded());
		n_assert(this->vertexLayout.isvalid());

		this->vertexLayout = 0;
		this->vbo->Unload();
		this->vbo = 0;
		this->ibo->Unload();
		this->ibo = 0;

		StateNodeInstance::Discard();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		TerrainNodeInstance::Render()
	{
		StateNodeInstance::Render();
		RenderDevice* renderDevice = RenderDevice::Instance();

		// setup rendering
		renderDevice->SetStreamVertexBuffer(0, this->vbo, 0);
		renderDevice->SetVertexLayout(this->vertexLayout);
		renderDevice->SetIndexBuffer(this->ibo);
		renderDevice->SetPrimitiveGroup(this->primGroup);

		// draw geometry
		renderDevice->Draw();
	}

	void TerrainNodeInstance::GenerateTerrainBasedOnResolution(int width, int height)
	{
		int vertexCount = width * height;
		int squares = (width - 1) * (height - 1);
		int triangles = squares * 2;
		int indicesCount = triangles * 3;
		// Create the structure to hold the mesh data.
		vertexData.Reserve(vertexCount);
		//vertexData.Reserve(vertexCount);// = new Math::point[vertexCount]; //3 floats per vertex
		indices.Reserve(indicesCount);// = new int[indicesCount];

		// Generate indices for specified resolution
		//we store columns in the array
		//current column is 0
		//current row is 0

		//since i store the points column wise the next column starts at index = current column * height

		//so how do we traverse?
		//we traverse with squares

		//we can traverse column wise
		//so first loop loops columns
		//second loop loops rows
		//first is column 0
		//first row is 0
		//then row is 1
		//we never do the last row nor last column, we don't do that with borders since they are already a part of pre-border edges

		//face 1
		//vertex 0 is current column and current row
		//vertex 1 is current column and current row + 1
		//vertex 2 is current column + 1 and current row + 1

		//face 2
		//vertex 0 is current column + 1 and current row + 1 i.e. same as face 1 vertex 2
		//vertex 1 is current column + 1 and current row
		//vertex 2 is current column and current row

		//indices are 0 1 4, 5 4 0
		//let's walk around the square
		//must traverse triangle vertices in same direction for all triangles f.ex. all face vertices are traversed counter-clockwise
		int index = 0;
		for (int col = 0; col < width; col++)
		{
			for (int row = 0; row < height; row++)
			{
				//since i store the points column wise the next column starts at index = current column * height
				int currentColumn = height * col;
				index = (currentColumn)+row;
				vertexData.Append(VertexData((float)col, (float)row, (float)(col / height), (float)(row / width)));
				//we never do the last row nor last column, we don't do that with borders since they are already a part border faces that were build in previous loop
				if (col == width - 1 || row == height - 1) continue; //this might be more expensive than writing another for loop set just for indices

				//face 1
				//vertex 0 is current column and current row
				//vertex 1 is current column and current row + 1
				//vertex 2 is current column + 1 and current row + 1
				int nextColumn = height * (col + 1); //or currentColumn + height //will use that later
				indices.Append(currentColumn + row);
				indices.Append(currentColumn + row + 1);
				indices.Append(nextColumn + row + 1); //we need to calculate the next column here
				//face 2
				//vertex 0 is current column + 1 and current row + 1 i.e. same as face 1 vertex 2
				//vertex 1 is current column + 1 and current row
				//vertex 2 is current column and current row i.e. same as face 1 vertex 1
				indices.Append(nextColumn + row + 1);
				indices.Append(nextColumn + row);
				indices.Append(currentColumn + row);
			}
		}


	}
} // namespace Models