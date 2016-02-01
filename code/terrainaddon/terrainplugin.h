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
	struct VertexData
	{
		Math::point pos;
		Math::float2 uv;
		VertexData(){}
		VertexData(float x, float y, float z, float u, float v)
		{
			pos.set(x, y, z);
			uv.set(u, v);
		}
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
		Util::Array<VertexData> vertexData;
		Ptr<CoreGraphics::RenderDevice> device;
		Ptr<CoreGraphics::TransformDevice> trans;

		void TerrainInit();
		void LoadShader();
		void GenerateTerrainBasedOnResolution(int width, int height);
		void SetUpVBO(Util::Array<VertexData>& terrainMesh, Util::Array<int>& indices);
		
		void DrawTerrain();
		void EnableShader();
		void EnableTerrain();

	private:
		bool visible;
		float gridSize;

		// mesh
		CoreGraphics::PrimitiveGroup primitive;
		Ptr<CoreGraphics::VertexBuffer> vbo;
		Ptr<CoreGraphics::IndexBuffer> ibo;
		Ptr<CoreGraphics::VertexLayout> vertexLayout;
		Ptr<Resources::ManagedTexture> tex;

		// shader
		Ptr<CoreGraphics::Shader> shader;
		Ptr<CoreGraphics::ShaderVariable> gridSizeVar;
		Ptr<CoreGraphics::ShaderVariable> gridTexVar;
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