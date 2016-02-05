#pragma once
//------------------------------------------------------------------------------
/**
@class Terrain::TerrainRTPlugin

The TerrainRTPlugin injects itself into the main frameshader and renders the terrain in the shape-pass.

(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "rendermodules/rt/rtplugin.h"
#include "coregraphics/vertexbuffer.h"
#include "coregraphics/indexbuffer.h"
#include "coregraphics/shaderinstance.h"
#include "coregraphics/texture.h"
#include "resources/managedtexture.h"
#include "coregraphics/renderdevice.h"
#include "coregraphics/transformdevice.h"



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

	class TerrainRTPlugin : public RenderModules::RTPlugin
	{
		__DeclareClass(TerrainRTPlugin);
	public:

		/// constructor
		TerrainRTPlugin();
		/// destructor
		virtual ~TerrainRTPlugin();

		/// called when plugin is registered on the render-thread side
		virtual void OnRegister();
		/// called when plugin is unregistered on the render-thread side
		virtual void OnUnregister();

		/// called when rendering a frame batch
		virtual void OnRenderFrameBatch(const Ptr<Frame::FrameBatch>& frameBatch);

		/// set if grid should be visible
		void SetVisible(bool b);
		/// set grid tile size
		void SetGridSize(float size);
		
		Util::Array<int> indices;
		//Util::Array<VertexData> vertexData;
		Util::Array<VertexData> vertexData;

		Ptr<CoreGraphics::RenderDevice> device;
		Ptr<CoreGraphics::TransformDevice> trans;

		void TerrainInit();
		void LoadShader();
		void GenerateTerrainBasedOnResolution(int width, int height);
		void SetUpVBO();
		
		void DrawTerrain();
		void EnableShader();
		void EnableTerrain();
		void UpdateTexture(void* data, SizeT size, SizeT width, SizeT height, IndexT left, IndexT top, IndexT mip);

	private:
		bool visible;
		float gridSize;
		int textureHeight;
		int textureWidth;

		// mesh
		CoreGraphics::PrimitiveGroup primitive;
		Ptr<CoreGraphics::VertexBuffer> vbo;
		Ptr<CoreGraphics::IndexBuffer> ibo;
		Ptr<CoreGraphics::VertexLayout> vertexLayout;
		Ptr<Resources::ManagedTexture> tex;

		// shader
		Ptr<CoreGraphics::Shader> shader;
		Ptr<CoreGraphics::ShaderVariable> HeightMultiplier;
		Ptr<CoreGraphics::ShaderVariable> gridTexVar;
		Ptr<CoreGraphics::ShaderVariable> transform;
	};

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
		TerrainRTPlugin::SetVisible(bool b)
	{
		this->visible = b;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	inline void
		TerrainRTPlugin::SetGridSize(float size)
	{
		this->gridSize = size;
	}

} // namespace Grid