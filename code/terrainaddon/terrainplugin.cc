//------------------------------------------------------------------------------
//  gridrtplugin.cc
//  (C) 2012-2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "terrainplugin.h"
#include "coregraphics/shaderserver.h"
#include "coregraphics/memoryvertexbufferloader.h"
#include "coregraphics/memoryindexbufferloader.h"
#include "coregraphics/transformdevice.h"
#include "coregraphics/displaydevice.h"
#include "resources/resourcemanager.h"

using namespace Math;
using namespace CoreGraphics;
using namespace Resources;
namespace Terrain
{
	__ImplementClass(Terrain::TerrainRTPlugin, 'TRRT', RenderModules::RTPlugin);

	//------------------------------------------------------------------------------
	/**
	*/
	TerrainRTPlugin::TerrainRTPlugin() :
		visible(true),
		gridSize(1)
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	TerrainRTPlugin::~TerrainRTPlugin()
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		TerrainRTPlugin::OnRegister()
	{
		TerrainInit();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		TerrainRTPlugin::OnUnregister()
	{
		this->tex = 0;
		this->ibo->Unload();
		this->ibo = 0;
		this->vbo->Unload();
		this->vbo = 0;

		this->HeightMultiplier = 0;
		this->gridTexVar = 0;
		this->shader = 0;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		TerrainRTPlugin::OnRenderFrameBatch(const Ptr<Frame::FrameBatch>& frameBatch)
	{
		if (FrameBatchType::Shapes == frameBatch->GetType() && this->visible)
		{
			device = RenderDevice::Instance();
			trans = TransformDevice::Instance();
			DrawTerrain();
		}
	}

	void TerrainRTPlugin::TerrainInit()
	{
		LoadShader();

		textureWidth = 2047;
		textureHeight = 2047;
		GenerateTerrainBasedOnResolution(textureWidth, textureHeight);

		SetUpVBO();
	}

	void TerrainRTPlugin::LoadShader()
	{
		// create new shader
		this->shader = ShaderServer::Instance()->GetShader("shd:my_simple");
		this->HeightMultiplier = this->shader->GetVariableByName("HeightMultiplier");
		this->gridTexVar = this->shader->GetVariableByName("HeightMap");
		this->transform = this->shader->GetVariableByName("ShapeModel");


		// load texture
		this->tex = ResourceManager::Instance()->CreateManagedResource(Texture::RTTI, "tex:examples/heighMap.dds").downcast<ManagedTexture>();
		 
	}

	void TerrainRTPlugin::GenerateTerrainBasedOnResolution(int width, int height)
	{
		int vertexCount = width * height;
		int squares = (width - 1) * (height -1);
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
		for (int col = 0; col < width; col++)
		{
			for (int row = 0; row < height; row++)
			{
				//since i store the points column wise the next column starts at index = current column * height
				int currentColumn = height * col;
				vertexData.Append(VertexData((float)col, (float)row, ((float)col / (float)height), ((float)row / (float)width)));
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

	void TerrainRTPlugin::SetUpVBO()
	{
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

		// setup ibo
		this->vertexLayout = this->vbo->GetVertexLayout();
		this->vertexLayout->SetIndexBuffer(this->ibo);

		this->primitive.SetBaseIndex(0);
		this->primitive.SetNumVertices(vertCount);
		this->primitive.SetBaseIndex(0);
		this->primitive.SetNumIndices(indicesCount);
		this->primitive.SetPrimitiveTopology(PrimitiveTopology::TriangleList);
	}

	void TerrainRTPlugin::EnableShader()
	{
		// start pass
		this->shader->Apply();

		// set variables
		this->shader->BeginUpdate();
		this->gridTexVar->SetTexture(this->tex->GetTexture());
		this->HeightMultiplier->SetFloat(200.0f);
		this->transform->SetMatrix(matrix44::translation(Math::float4(-textureWidth/2.f,0,-textureHeight/2.f,0)));
		this->shader->EndUpdate();
		this->shader->Commit();
	}

	void TerrainRTPlugin::EnableTerrain()
	{
		device->SetStreamSource(0, this->vbo, 0);
		device->SetVertexLayout(this->vertexLayout);
		device->SetIndexBuffer(this->ibo);
		device->SetPrimitiveGroup(this->primitive);
	}

	void TerrainRTPlugin::DrawTerrain()
	{
		EnableShader();
		EnableTerrain();
		device->Draw();
	}

} // namespace Grid