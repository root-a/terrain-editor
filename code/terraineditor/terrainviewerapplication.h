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

#include "terrainaddon/terrainaddon.h"

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

	Math::float4 CalculateWorldPosFromMouseAndDepth(const Ptr<Input::Mouse> mouse);

	/// render current frame
    virtual void OnUpdateFrame();
    /// called to configure display
    virtual void OnConfigureDisplay();

	/// append a test model
    virtual void AppendTestModel();
	void OnInputUpdateCamera();
    Util::Array<Ptr<Graphics::ModelEntity>> models;
    Ptr<Graphics::GlobalLightEntity> globalLight;
    Util::Array<Ptr<Graphics::PointLightEntity>> pointLights;
    Ptr<Graphics::SpotLightEntity> testSpotLight;
    Util::Array<Ptr<Graphics::SpotLightEntity>> spotLights;
	bool renderDebug;

	Ptr<Terrain::TerrainAddon> terrainAddon;
	Ptr<Graphics::ModelEntity> terrainEnt;
	Ptr<Picking::PickingServer> pickingServer;
};

} // namespace Test
//------------------------------------------------------------------------------
