#pragma once
//------------------------------------------------------------------------------
/**
    @class Tools::TestViewer
    
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


//------------------------------------------------------------------------------
namespace Tools
{
class TestViewerApplication : public App::ViewerApplication
{
public:
    /// constructor
    TestViewerApplication();
    /// destructor
    virtual ~TestViewerApplication();
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
};

} // namespace Test
//------------------------------------------------------------------------------
