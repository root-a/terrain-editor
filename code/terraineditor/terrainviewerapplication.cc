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
#include "input/mouse.h"
#include "framecapture/framecapturerendermodule.h"
#include "framecaptureprotocol.h"
#include "imgui/imgui.h"
#include "picking/pickingserver.h"
#include "brush.h"

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
    avgFPS(0.0f),
    renderDebug(false)
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
		this->mayaCameraUtil.Setup(point(0.0f, 0.0f, 0.0f), point(200.0f, 10.f, 200.0f), vector(0.0f, 1.0f, 0.0f));
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


		this->ground = ModelEntity::Create();
		this->ground->SetResourceId(ResourceId("mdl:examples/placeholder.n3"));
		transform = matrix44::translation(0, 0, 0);
		this->ground->SetTransform(transform);
		this->stage->AttachEntity(ground.cast<GraphicsEntity>());

		ground2 = ModelEntity::Create();
		this->ground2->SetResourceId(ResourceId("mdl:examples/placeholder.n3"));
		transform = matrix44::translation(39, 0, 39);
		this->ground2->SetTransform(transform);
		this->stage->AttachEntity(ground2.cast<GraphicsEntity>());

		ground3 = ModelEntity::Create();
		this->ground3->SetResourceId(ResourceId("mdl:examples/placeholder.n3"));
		transform = matrix44::translation(19, 0, 19);
		this->ground3->SetTransform(transform);
		this->stage->AttachEntity(ground3.cast<GraphicsEntity>());

        // wait for animated stuff to load
        GraphicsInterface::Instance()->WaitForPendingResources();

        // setup frame capture render module
        Ptr<FrameCaptureRenderModule> module = FrameCaptureRenderModule::Create();
        module->Setup();

		// setup terrain
		this->terrainAddon = Terrain::TerrainAddon::Create();
		this->terrainAddon->Setup(stage);

		Picking::PickingServer::Instance()->SetEnabled(true);

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
	this->terrainAddon->Discard();
	this->terrainAddon = 0;

    this->stage->RemoveEntity(this->globalLight.cast<GraphicsEntity>());
	this->stage->RemoveEntity(this->ground.cast<GraphicsEntity>());
	this->stage->RemoveEntity(this->ground2.cast<GraphicsEntity>());
	this->stage->RemoveEntity(this->ground3.cast<GraphicsEntity>());
    this->globalLight = 0;
	this->ground = 0;
	this->ground2 = 0;
	this->ground3 = 0;
                         
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
	const Ptr<Keyboard>& keyboard = InputServer::Instance()->GetDefaultKeyboard();
	const Ptr<Mouse>& mouse = InputServer::Instance()->GetDefaultMouse();

	if (keyboard->KeyDown(Key::F4))
	{
		// turn on debug rendering
		Ptr<Debug::RenderDebugView> renderDebugMsg = Debug::RenderDebugView::Create();
		renderDebugMsg->SetThreadId(Threading::Thread::GetMyThreadId());
		renderDebugMsg->SetEnableDebugRendering(!this->renderDebug);
		Graphics::GraphicsInterface::Instance()->Send(renderDebugMsg.cast<Messaging::Message>());
		this->renderDebug = !this->renderDebug;
	}
	
	if (!keyboard->KeyPressed(Key::LeftMenu))
	{
		if (mouse->ButtonDown(MouseButton::LeftButton))
		{
			Terrain::KeyMod mod = Terrain::KeyMod::None;
			if (keyboard->KeyPressed(Key::LeftControl)) mod = Terrain::KeyMod::Ctrl;
			if (keyboard->KeyPressed(Key::LeftShift)) mod = Terrain::KeyMod::Shift;
			float4 worldPos = CalculateWorldPosFromMouseAndDepth(mouse);
			terrainAddon->UpdateTerrainAtPos(worldPos, mod);
		}
		else if (mouse->ButtonPressed(MouseButton::LeftButton))
		{
			if (mouse->GetMovement().length() > 1.f) // don't paint when mouse is stationary
			{
				Terrain::KeyMod mod = Terrain::KeyMod::None;
				if (keyboard->KeyPressed(Key::LeftControl)) mod = Terrain::KeyMod::Ctrl;
				if (keyboard->KeyPressed(Key::LeftShift)) mod = Terrain::KeyMod::Shift;
				float4 worldPos = CalculateWorldPosFromMouseAndDepth(mouse);
				terrainAddon->UpdateTerrainAtPos(worldPos, mod);
			}
		}
	}
	//if we change the height-map res, let's recenter camera?
	//this->mayaCameraUtil.Setup(point((127 / 2.0f), 0, (127 / 2.0f)), point(200.0f, 10.f, 200.0f), vector(0.0f, 1.0f, 0.0f));
	//this->mayaCameraUtil.Update();
	
	OnInputUpdateCamera();
}

//------------------------------------------------------------------------------
/**
*/
void
TerrainViewerApplication::OnUpdateFrame()
{
    ViewerApplication::OnUpdateFrame();
}

//------------------------------------------------------------------------------
/**
*/
void
TerrainViewerApplication::AppendTestModel()
{              
}

void 
TerrainViewerApplication::OnInputUpdateCamera()
{
	// update the camera from input
	InputServer* inputServer = InputServer::Instance();
	const Ptr<Keyboard>& keyboard = inputServer->GetDefaultKeyboard();
	const Ptr<Mouse>& mouse = inputServer->GetDefaultMouse();

#ifndef FREECAM
	// standard input handling: manipulate camera
	bool pan = mouse->ButtonPressed(MouseButton::MiddleButton) && keyboard->KeyPressed(Key::LeftMenu);
	bool zoom = mouse->ButtonPressed(MouseButton::RightButton) && keyboard->KeyPressed(Key::LeftMenu);
	
	this->mayaCameraUtil.SetOrbitButton(mouse->ButtonPressed(MouseButton::LeftButton) && keyboard->KeyPressed(Key::LeftMenu));
	this->mayaCameraUtil.SetPanButton(pan);
	this->mayaCameraUtil.SetZoomButton(zoom);
	this->mayaCameraUtil.SetZoomInButton(mouse->WheelForward());
	this->mayaCameraUtil.SetZoomOutButton(mouse->WheelBackward());
	this->mayaCameraUtil.SetMouseMovement(mouse->GetMovement());

	// process gamepad input
	float zoomIn = 0.0f;
	float zoomOut = 0.0f;
	float2 panning(0.0f, 0.0f);
	//float2 orbiting(0.0f, 0.0f);
	float zoomSpeed = 50.f;
	float panSpeed = 50.f;

	float frameTime = (float) this->GetFrameTime();
	if (zoom)
	{
		zoomIn += mouse->GetMovement().y() * frameTime * zoomSpeed;
		zoomOut += mouse->GetMovement().y() * frameTime * zoomSpeed;
	}
	if (pan)
	{
		panning.x() -= mouse->GetMovement().x() * frameTime * panSpeed;
		panning.y() -= mouse->GetMovement().y() * frameTime * panSpeed;
	}
	
	// process keyboard input
	if (keyboard->KeyDown(Key::Escape))
	{
		this->SetQuitRequested(true);
	}
	if (keyboard->KeyDown(Key::Space))
	{
		this->mayaCameraUtil.Reset();
	}
	if (keyboard->KeyPressed(Key::Left))
	{
		panning.x() -= 1.1f;
	}
	if (keyboard->KeyPressed(Key::Right))
	{
		panning.x() += 1.1f;
	}
	if (keyboard->KeyPressed(Key::Up))
	{
		panning.y() -= 1.1f;
	}
	if (keyboard->KeyPressed(Key::Down))
	{
		panning.y() += 1.1f;
	}

	this->mayaCameraUtil.SetPanning(panning);
	//this->mayaCameraUtil.SetOrbiting(orbiting);
	this->mayaCameraUtil.SetZoomIn(zoomIn);
	this->mayaCameraUtil.SetZoomOut(zoomOut);
	this->mayaCameraUtil.Update();
	this->camera->SetTransform(this->mayaCameraUtil.GetCameraTransform());
#else
	this->freeCameraUtil.SetRotateButton(mouse->ButtonPressed(MouseButton::LeftButton));
	this->freeCameraUtil.SetAccelerateButton(keyboard->KeyPressed(Key::Shift));
	this->freeCameraUtil.SetForwardsKey(keyboard->KeyPressed(Key::W));
	this->freeCameraUtil.SetBackwardsKey(keyboard->KeyPressed(Key::S));
	this->freeCameraUtil.SetLeftStrafeKey(keyboard->KeyPressed(Key::A));
	this->freeCameraUtil.SetRightStrafeKey(keyboard->KeyPressed(Key::D));
	this->freeCameraUtil.SetUpKey(keyboard->KeyPressed(Key::Space));
	this->freeCameraUtil.SetDownKey(keyboard->KeyPressed(Key::C));
	this->freeCameraUtil.SetMouseMovement(mouse->GetMovement());
	this->freeCameraUtil.Update();
	this->camera->SetTransform(this->freeCameraUtil.GetTransform());
#endif
}

Math::float4 TerrainViewerApplication::CalculateWorldPosFromMouseAndDepth(const Ptr<Mouse> mouse)
{
	float depth = Picking::PickingServer::Instance()->FetchDepth(mouse->GetPixelPosition());

	//n_printf("\ndepth distance %f\n", depth);
	Math::float2 screenPos = mouse->GetScreenPosition();

	float2 focalLength = camera->GetCameraSettings().GetFocalLength();
	float2 mousePos((screenPos.x()*2.f - 1.f), -(screenPos.y()*2.f - 1.f));
	//n_printf("\nmousePos %f %f\n", mousePos.x(), mousePos.y());
	float2 viewSpace = float2::multiply(mousePos, focalLength);
	vector viewSpacePos(viewSpace.x(), viewSpace.y(), -1);

	viewSpacePos = float4::normalize3(viewSpacePos);
	point surfaceSpacePos = point(viewSpacePos*depth);

	float4 worldPos = matrix44::transform(surfaceSpacePos, TransformDevice::Instance()->GetInvViewTransform());

	return worldPos;
}


} // namespace Tools
