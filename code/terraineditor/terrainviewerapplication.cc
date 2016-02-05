//------------------------------------------------------------------------------
//  testviewerapplication.cc
//  (C) 2006 Radon Labs GmbH
//  (C) 2013-2014 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "terraineditor/terrainviewerapplication.h"
#include "debugrender/debugrender.h"
#include "debugrender/debugshaperenderer.h"
#include "math/quaternion.h"
#include "input/keyboard.h"
#include "input/gamepad.h"
#include "framecapture/framecapturerendermodule.h"
#include "framecaptureprotocol.h"
#include "imgui/imgui.h"
#include "coregraphics/memoryvertexbufferloader.h"
#include "coregraphics/memoryindexbufferloader.h"
#include "renderutil/nodelookuputil.h"
#include "models/nodes/shapenodeinstance.h"
#include "models/nodes/shapenode.h"

namespace Tools
{
using namespace CoreGraphics;
using namespace Graphics;
using namespace Math;
using namespace Util;
using namespace Resources;
using namespace Timing;
using namespace Debug;
using namespace Input;
using namespace FrameCapture;

//------------------------------------------------------------------------------
/**
*/
TerrainViewerApplication::TerrainViewerApplication() :
    shadowConstants(100.0f, 20.0f, 0.003f, 1024.0f),
    direction(1.0),
    avgFPS(0.0f),
    benchmarkmode(false),
    renderDebug(false),
    rotX(-225),
    capturing(false),
	fullscreen(false)
{
    // empty
}

//------------------------------------------------------------------------------
/**
*/
TerrainViewerApplication::~TerrainViewerApplication()
{
    if (this->IsOpen())
    {
        this->Close();
    }
}

//------------------------------------------------------------------------------
/**
*/
bool
TerrainViewerApplication::Open()
{
    n_assert(!this->IsOpen());
    if (ViewerApplication::Open())
    {

		// setup terrain
		this->terrain = Terrain::TerrainAddon::Create();
		this->terrain->Setup();

        // setup lights
		matrix44 lightTransform = matrix44::rotationx(n_deg2rad(-45.0f));
        this->globalLight = GlobalLightEntity::Create();
        this->globalLight->SetTransform(lightTransform);
        this->globalLight->SetColor(float4(0.4f, 0.4f, 0.4f, 0.1f)*3);
        this->globalLight->SetBackLightColor(float4(0.0f, 0.0f, 0.15f, 0.1f));
        this->globalLight->SetAmbientLightColor(float4(0.2f, 0.2f, 0.2f, 1.0f));
        this->globalLight->SetCastShadows(false);
		this->globalLight->SetShadowIntensity(0.2f);
		this->globalLight->SetVolumetric(true);
        this->stage->AttachEntity(this->globalLight.cast<GraphicsEntity>());

		// setup the camera util object
		this->mayaCameraUtil.Setup(point(0.0f, 0.0f, 0.0f), point(200.0f, 1000.0f, 200.0f), vector(0.0f, 1.0f, 0.0f));
		this->mayaCameraUtil.Update();
		this->camera->SetTransform(this->mayaCameraUtil.GetCameraTransform());
		
        // create one shadow casting light 
        matrix44 transform = matrix44::rotationx(n_deg2rad(-45.0f));        
		transform.scale(point(1,1,1));
		transform.set_position(point(0,10,5));
        this->testSpotLight = SpotLightEntity::Create();
        this->testSpotLight->SetTransform(transform);
        this->testSpotLight->SetCastShadows(false);
        this->testSpotLight->SetColor(float4(1,0.7f,1,0.1));
        this->stage->AttachEntity(this->testSpotLight.cast<GraphicsEntity>());

        // setup models        
		this->ground = ModelEntity::Create();
		this->ground->SetResourceId(ResourceId("mdl:examples/dummyground.n3"));
		transform = matrix44::translation(0,-15,0);
		this->ground->SetTransform(transform);// matrix44::multiply(transform, matrix44::translation(0, n_deg2rad(90), 0)));
		//this->stage->AttachEntity(ground.cast<GraphicsEntity>());


        // wait for animated stuff to load
        GraphicsInterface::Instance()->WaitForPendingResources();

        // setup frame capture render module
        Ptr<FrameCaptureRenderModule> module = FrameCaptureRenderModule::Create();
        module->Setup();

		
		this->terrainModelEnt = ModelEntity::Create();
		this->terrainModelEnt->SetResourceId(ResourceId("mdl:examples/dummyground.n3"));
		transform = matrix44::translation(2047 / 2.f, 0, 2047 / 2.f);
		this->terrainModelEnt->SetTransform(transform);
		this->terrainModelEnt->SetLoadSynced(true);
		this->stage->AttachEntity(terrainModelEnt.cast<GraphicsEntity>());

		
		//terrainMesh->SetVertexBuffer()
		// setup models
		//this->terrainentity = Graphics::TerrainEntity::Create();
		//this->terrainentity.cast<ModelEntity>()->SetResourceId(ResourceId("mdl:examples/dummyground.n3"));
		//transform = matrix44::translation(0, 0, 0);
		//this->terrainentity->SetTransform(transform);// matrix44::multiply(transform, matrix44::translation(0, n_deg2rad(90), 0)));
		//this->stage->AttachEntity(terrainentity.cast<GraphicsEntity>());

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
TerrainViewerApplication::Close()
{

	// close terrain
	this->terrain->Discard();
	this->terrain = 0;

    this->stage->RemoveEntity(this->globalLight.cast<GraphicsEntity>());
	//this->stage->RemoveEntity(this->ground.cast<GraphicsEntity>());   
	//this->stage->RemoveEntity(this->terrainentity.cast<GraphicsEntity>());
	
    this->globalLight = 0;
    this->ground = 0;   
	this->terrainentity = 0;
                         
    IndexT i;
    for (i = 0; i < this->pointLights.Size(); i++)
    {
        this->stage->RemoveEntity(this->pointLights[i].cast<GraphicsEntity>());
    }
    this->pointLights.Clear();
    for (i = 0; i < this->spotLights.Size(); i++)
    {
        this->stage->RemoveEntity(this->spotLights[i].cast<GraphicsEntity>());
    }
    this->spotLights.Clear();

    this->stage->RemoveEntity(this->testSpotLight.cast<GraphicsEntity>());
    this->testSpotLight = 0;

    for (i = 0; i < this->models.Size(); i++)
    {
        this->stage->RemoveEntity(this->models[i].cast<GraphicsEntity>());    	
    }
    this->models.Clear();

    FrameCaptureRenderModule::Instance()->Discard();
    ViewerApplication::Close();
}

//------------------------------------------------------------------------------
/**
*/
void
TerrainViewerApplication::OnConfigureDisplay()
{
    ViewerApplication::OnConfigureDisplay();
    this->display->Settings().SetVerticalSyncEnabled(true);
}

//------------------------------------------------------------------------------
/**
*/
void
TerrainViewerApplication::OnProcessInput()
{
    const Ptr<Keyboard>& kbd = InputServer::Instance()->GetDefaultKeyboard();
    const Ptr<GamePad>& gamePad = InputServer::Instance()->GetDefaultGamePad(0);
    
    
    ViewerApplication::OnProcessInput();
}

//------------------------------------------------------------------------------
/**
*/
void
TerrainViewerApplication::OnUpdateFrame()
{	  
	/*
	if (terrainModelEnt->IsActive())
	{
		Ptr<ModelEntity> modelEntity = terrainModelEnt.cast<ModelEntity>();
		// is model entity deleted, and msg out-of-date, return handled = true to remove msg from list
		if (modelEntity->IsActive())
		{
			// check resource state if set to
			if (modelEntity->GetModelResourceState() == Resources::Resource::Loaded)
			{
				if (modelEntity->GetModelInstance().isvalid())
				{
					Ptr<Models::ShapeNodeInstance> terrainShapeNodeInstance = RenderUtil::NodeLookupUtil::LookupStateNodeInstance(terrainModelEnt, "pCube").cast<Models::ShapeNodeInstance>();
					Ptr<Models::ShapeNode> terrainShapeNode = terrainShapeNodeInstance->GetModelNode().cast<Models::ShapeNode>();
					Ptr<Resources::ManagedMesh> terrainManagedMesh = terrainShapeNode->GetManagedMesh();
					Ptr<CoreGraphics::Mesh> terrainMesh = terrainManagedMesh->GetMesh();
				}
			}
		}
	}
	*/
    ViewerApplication::OnUpdateFrame();
}

//------------------------------------------------------------------------------
/**
*/
void
TerrainViewerApplication::AppendTestModel()
{              
}

} // namespace Tools
