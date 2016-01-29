#pragma once
//------------------------------------------------------------------------------
/**
    @class Tools::TerrainViewer
    
    Nebula3 test viewer app.

    (C) 2006 Radon Labs GmbH
    (C) 2013-2014 Individual contributors, see AUTHORS file
*/
#include "apprender/viewerapplication.h"
#include "graphics/spotlightentity.h"
#include "graphics/modelentity.h"
#include "graphics/globallightentity.h"
#include "graphics/pointlightentity.h"
#include "graphics/billboardentity.h"
#include "dynui/imguiaddon.h"

#include "rendermodules/rt/rtplugin.h"
#include "coregraphics/vertexbuffer.h"
#include "coregraphics/indexbuffer.h"
#include "coregraphics/shaderinstance.h"
#include "coregraphics/texture.h"
#include "resources/managedtexture.h"

#include "coregraphics/shaderserver.h"
#include "coregraphics/transformdevice.h"
#include "coregraphics/displaydevice.h"
#include "resources/resourcemanager.h"

#include "coregraphics/renderdevice.h"

//------------------------------------------------------------------------------
namespace Tools
{
class TerrainViewerApplication : public App::ViewerApplication
{
public:
    /// constructor
	TerrainViewerApplication();
    /// destructor
	virtual ~TerrainViewerApplication();
    /// open the application
    virtual bool Open();
    /// close the application
    virtual void Close();

private:
    /// process input (called before rendering)
    virtual void OnProcessInput();
    /// render current frame
    virtual void OnUpdateFrame();
    /// called to configure display
    virtual void OnConfigureDisplay();

	/// append a test model
    virtual void AppendTestModel();

	Ptr<Dynui::ImguiAddon> ui;
    Ptr<Graphics::ModelEntity> ground;
	Ptr<Graphics::BillboardEntity> billboard;
    Util::Array<Ptr<Graphics::ModelEntity> > models;
    Ptr<Graphics::GlobalLightEntity> globalLight;
    Util::Array<Ptr<Graphics::PointLightEntity> > pointLights;
    Ptr<Graphics::SpotLightEntity> testSpotLight;
    Util::Array<Ptr<Graphics::SpotLightEntity> > spotLights;
    Util::Array<Math::matrix44> lightTransforms; 
    Math::float4 shadowConstants;
    float direction;
    Util::Array<Timing::Time> frameTimes;
    Timing::Time avgFPS;
    bool benchmarkmode;
    bool renderDebug;
	bool fullscreen;
    float rotX;
    bool capturing;
	//create memory texture
	//use the size of that texture to generate the terrain plane
	void GenerateTerrainBasedOnResolution(int width, int height);

	int* indices;
	Math::vector* vertices;
	Ptr<CoreGraphics::RenderDevice> device;
	Ptr<CoreGraphics::TransformDevice> trans;

	void SetUpVBO(Math::vector* terrainMesh, Math::vector* indices, int width, int height);
	void LoadShader();
	void EnableShader();
	void EnableTerrain();
	void DrawTerrain();

	void Unload();
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

} // namespace Test
//------------------------------------------------------------------------------
