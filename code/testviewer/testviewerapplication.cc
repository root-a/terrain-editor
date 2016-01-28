//------------------------------------------------------------------------------
//  testviewerapplication.cc
//  (C) 2006 Radon Labs GmbH
//  (C) 2013-2014 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "testviewer/testviewerapplication.h"
#include "debugrender/debugrender.h"
#include "debugrender/debugshaperenderer.h"
#include "math/quaternion.h"
#include "input/keyboard.h"
#include "input/gamepad.h"
#include "framecapture/framecapturerendermodule.h"
#include "framecaptureprotocol.h"
#include "imgui/imgui.h"


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
TestViewerApplication::TestViewerApplication() : 
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
TestViewerApplication::~TestViewerApplication()
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
TestViewerApplication::Open()
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
		this->mayaCameraUtil.Setup(point(0.0f, 0.0f, 0.0f), point(200.0f, 200.0f, 200.0f), vector(0.0f, 1.0f, 0.0f));
		this->mayaCameraUtil.Update();
		this->camera->SetTransform(this->mayaCameraUtil.GetCameraTransform());

        // create a few point lights
        IndexT lightIndex;
        const SizeT numLights = 1;
        const float lightRange = 25;
        for (lightIndex = 0; lightIndex < numLights; lightIndex++)
        {   
			float x = n_rand() * 10.0f - 5.0f;
			float y = 10;
			float z = n_rand() * 10.0f - 5.0f;
			float r = n_rand();
			float g = n_rand();
			float b = n_rand();
                
            Ptr<PointLightEntity> pointLight = PointLightEntity::Create();            
            pointLight->SetTransformFromPosDirAndRange(point(x, y, z), -vector::upvec(), lightRange);
            pointLight->SetColor(float4(r, g, b, 1.0f));
            pointLight->SetCastShadows(false);
			//pointLight->SetVolumetric(true);
            //pointLight->SetVolumetricIntensity(1);
            //pointLight->SetVolumetricScale(5);
            this->pointLights.Append(pointLight);
            this->lightTransforms.Append(pointLight->GetTransform());
            this->stage->AttachEntity(pointLight.cast<GraphicsEntity>());
        }

        const SizeT numSpotLights = 0;
        for (lightIndex = 0; lightIndex < numSpotLights; lightIndex++)
        {
            float x = n_rand() * 10.0f - 5.0f;
            float y = 10;
            float z = n_rand() * 10.0f - 5.0f;
            float r = n_rand() + 0.1f;
            float g = n_rand() + 0.1f;
            float b = n_rand() + 0.1f;
            matrix44 spotLightTransform = matrix44::multiply(matrix44::rotationy(n_deg2rad(90)), matrix44::rotationx(n_deg2rad(-65.0f)));
            spotLightTransform.scale(point(7,7,7));
            spotLightTransform.set_position(point(x,y,z));
            Ptr<SpotLightEntity> spotLight = SpotLightEntity::Create();
            spotLight->SetTransform(spotLightTransform);
            spotLight->SetColor(float4(r, g, b, 1.0f));
            spotLight->SetCastShadows(false);
            this->spotLights.Append(spotLight);
            this->stage->AttachEntity(spotLight.cast<GraphicsEntity>());
        } 

		
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
		transform = matrix44::translation(0,-10,0);
		this->ground->SetTransform(matrix44::multiply(transform, matrix44::translation(0, n_deg2rad(90), 0)));
		this->stage->AttachEntity(ground.cast<GraphicsEntity>());


		this->billboard = BillboardEntity::Create();
		this->billboard->SetViewAligned(true);
		this->billboard->SetTexture("tex:system/nebulalogo.dds");
		this->stage->AttachEntity(this->billboard.cast<GraphicsEntity>());

		/*
		{
			Ptr<ModelEntity> entity = ModelEntity::Create();
			entity->SetResourceId("mdl:characters/cpt_g.n3");
			entity->SetTransform(matrix44::translation(10, 0, 10));
			this->stage->AttachEntity(entity.cast<GraphicsEntity>());
			models.Append(entity);

			Ptr<AnimPlayClip> msg = AnimPlayClip::Create();
			msg->SetLoopCount(0);
			msg->SetClipName("idle");
			__Send(entity, msg);

			entity = ModelEntity::Create();
			entity->SetResourceId("mdl:characters/cpt_g.n3");
			entity->SetTransform(matrix44::translation(10, 0, 10));
			this->stage->AttachEntity(entity.cast<GraphicsEntity>());
			models.Append(entity);

			msg = AnimPlayClip::Create();
			msg->SetLoopCount(0);
			msg->SetClipName("walk");
			__Send(entity, msg);
		}
		*/

		IndexT c = 0;
		for (int i = -50; i < 50; i++)
		{
			for (int j = -10; j < 10; j++)
			{
				Ptr<ModelEntity> modelEntity = ModelEntity::Create();
				//modelEntity->SetResourceId("mdl:characters/cpt_g.n3");
				modelEntity->SetResourceId("mdl:examples/ak74.n3");
				modelEntity->SetTransform(matrix44::translation(point(i * 3.0f, 0.0f, j * 15.0f)));
				modelEntity->SetInstanced(true);
				this->stage->AttachEntity(modelEntity.cast<GraphicsEntity>());
				models.Append(modelEntity);

				/*
				Ptr<AnimPlayClip> msg = AnimPlayClip::Create();
				msg->SetClipName("walk");
				//msg->SetStartTime((i+j)*3000);
				msg->SetTimeOffset(n_rand(i*100, j*100));
				msg->SetLoopCount(0);
				__Send(modelEntity, msg);
				*/				
			}
		}

		// create new ui
		this->ui = Dynui::ImguiAddon::Create();
		this->ui->Setup();

        // wait for animated stuff to load
        GraphicsInterface::Instance()->WaitForPendingResources();		

        // setup frame capture render module
        Ptr<FrameCaptureRenderModule> module = FrameCaptureRenderModule::Create();
        module->Setup();

        return true;
    }
    return false;
}

//------------------------------------------------------------------------------
/**
*/
void
TestViewerApplication::Close()
{
	this->ui->Discard();
	this->ui = 0;

    this->stage->RemoveEntity(this->globalLight.cast<GraphicsEntity>());
	this->stage->RemoveEntity(this->ground.cast<GraphicsEntity>());   
	this->stage->RemoveEntity(this->billboard.cast<GraphicsEntity>());
	
	this->billboard = 0;
    this->globalLight = 0;
    this->ground = 0;   
                         
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
TestViewerApplication::OnConfigureDisplay()
{
    ViewerApplication::OnConfigureDisplay();
    this->display->Settings().SetVerticalSyncEnabled(true);
}

//------------------------------------------------------------------------------
/**
*/
void
TestViewerApplication::OnProcessInput()
{
    const Ptr<Keyboard>& kbd = InputServer::Instance()->GetDefaultKeyboard();
    const Ptr<GamePad>& gamePad = InputServer::Instance()->GetDefaultGamePad(0);
    
    // enable/disable debug view
    if (kbd->KeyDown(Key::F4) || gamePad.isvalid() && gamePad->ButtonDown(GamePad::XButton))
    {
        // turn on debug rendering        
        Ptr<Debug::RenderDebugView> renderDebugMsg = Debug::RenderDebugView::Create();
        renderDebugMsg->SetThreadId(Threading::Thread::GetMyThreadId());
        renderDebugMsg->SetEnableDebugRendering(!this->renderDebug);
        Graphics::GraphicsInterface::Instance()->Send(renderDebugMsg.cast<Messaging::Message>());
        this->renderDebug = !this->renderDebug;
    }
    else if (kbd->KeyDown(Key::F3))
    {
        this->benchmarkmode = !this->benchmarkmode;
        // reset to start pos
#ifndef FREECAM
		this->mayaCameraUtil.Reset();
#else
		this->freeCameraUtil.Reset();
#endif
    }
	else if (kbd->KeyDown(Key::F5))
	{
		this->fullscreen = !this->fullscreen;
		Ptr<Graphics::UpdateDisplay> dsp = Graphics::UpdateDisplay::Create();
		dsp->SetDisplayMode(CoreGraphics::DisplayMode(0, 0, 1024, 768));
		dsp->SetFullscreen(this->fullscreen);
		dsp->SetTripleBufferingEnabled(true);
		GraphicsInterface::Instance()->Send(dsp.upcast<Messaging::Message>());
	}
    else if (kbd->KeyDown(Key::F11))
    {
        Ptr<Messaging::Message> msg;
        if (this->capturing)
        {
            Ptr<StopFrameCapture> stopMsg = StopFrameCapture::Create();
            msg = stopMsg.cast<Messaging::Message>();
            this->capturing = false;
        }
        else
        {
            Ptr<StartFrameCapture> startMsg = StartFrameCapture::Create();
            msg = startMsg.cast<Messaging::Message>();
            this->capturing = true;
        }
        Graphics::GraphicsInterface::Instance()->Send(msg);
    }
    else if (kbd->KeyDown(Key::F7))
    {
        Ptr<SaveScreenShot> msg = SaveScreenShot::Create();
        Graphics::GraphicsInterface::Instance()->Send(msg.cast<Messaging::Message>());
    }
    else if (kbd->KeyDown(Key::F6) || gamePad->ButtonDown(GamePad::LeftThumbButton))
    {
        this->direction *= -1;
    }
    
	/*
    bool updateSharedVar = false;
    if (kbd->KeyDown(Key::Q) || gamePad->ButtonDown(GamePad::RightThumbButton))
    {
        if (kbd->KeyPressed(Key::Shift) || gamePad->ButtonPressed(GamePad::RightShoulderButton))
        {
            if (this->shadowConstants.x() >= 1.0f)
            {
                this->shadowConstants.x() -= 1.0f;    
            }
            else
            {
                this->shadowConstants.x() -= 0.1f;    
            }            
        }
        else
        {
            if (this->shadowConstants.x() >= 1.0f)
            {
                this->shadowConstants.x() += 1.0f;    
            }
            else
            {
                this->shadowConstants.x() += 0.1f;    
            }
        }           
        this->shadowConstants.x() = n_clamp(this->shadowConstants.x(), 0.1f, 100.0f);
        updateSharedVar = true;
    }
    else if (kbd->KeyDown(Key::W) || gamePad->ButtonDown(GamePad::YButton))
    {
        if (kbd->KeyPressed(Key::Shift) || gamePad->ButtonPressed(GamePad::RightShoulderButton))
        {
            this->shadowConstants.y() -= 1.0f;    
        }
        else
        {
            this->shadowConstants.y() += 1.0f;    
        }   
        this->shadowConstants.y() = n_clamp(this->shadowConstants.y(), 1.0f, 100.0f);
        updateSharedVar = true;
    } 
    else if (kbd->KeyDown(Key::E) || gamePad->ButtonDown(GamePad::BButton))
    {
        if (kbd->KeyPressed(Key::Shift) || gamePad->ButtonPressed(GamePad::RightShoulderButton))
        {
            this->shadowConstants.z() -= 0.0001f;    
        }
        else
        {
            this->shadowConstants.z() += 0.0001f;    
        }           
        updateSharedVar = true;
    }
    else if (kbd->KeyDown(Key::N))
    {
        this->AppendTestModel();
    }

    Util::String shadowConstantStr(Util::String::FromFloat4(this->shadowConstants));
    _debug_text(shadowConstantStr, Math::float2(0.2,0.0), Math::float4(1,1,1,1));

    if (updateSharedVar)
    {
        Ptr<Graphics::UpdateSharedShaderVariable> msg = Graphics::UpdateSharedShaderVariable::Create();
		msg->SetSemantic("Pixel:ShadowConstants");
        msg->SetValue(this->shadowConstants);
        GraphicsInterface::Instance()->SendBatched(msg.cast<Messaging::Message>());
    }
	*/

        
    if (this->benchmarkmode)
    {
#ifndef FREECAM
		// auto rotate
		this->mayaCameraUtil.SetOrbiting(Math::float2(0.02,0));
		this->mayaCameraUtil.SetOrbitButton(true);
		this->mayaCameraUtil.Update();
		this->camera->SetTransform(this->mayaCameraUtil.GetCameraTransform());
#else
		this->freeCameraUtil.SetMouseMovement(Math::float2(0.02, 0));
		this->freeCameraUtil.SetRotateButton(true);
		this->freeCameraUtil.Update();
		this->camera->SetTransform(this->freeCameraUtil.GetTransform());
#endif
     
    }
    else
    {                                        
        ViewerApplication::OnProcessInput(); 
    }
}

//------------------------------------------------------------------------------
/**
*/
void
TestViewerApplication::OnUpdateFrame()
{
    //DebugShapeRenderer::Instance()->DrawBox(matrix44::identity(), float4(1.0f, 0.0f, 0.0f, 1.0f));
    //DebugShapeRenderer:://Instance()->DrawSphere(matrix44::identity(), float4(1.0f, 0.5f, 0.0f, 0.5f));

    //DebugShapeRenderer::Instance()->DrawBox(matrix44::identity(), float4(1.0f, 0.5f, 0.0f, 0.5f));
    //DebugShapeRenderer::Instance()->DrawSphere(matrix44::translation(-2.0f, 1.0f, 0.0f), float4(0.0f, 0.5f, 0.0f, 0.5f));
    //DebugShapeRenderer::Instance()->DrawCylinder(matrix44::translation(-1.0f, 3.0f, 0.0f), float4(1.0f, 0.5f, 0.2f, 0.5f));
    //DebugShapeRenderer::Instance()->DrawTorus(matrix44::translation(2.0f, 1.0f, 0.0f), float4(0.3f, 0.5f, 0.2f, 0.5f));

	this->ui->BeginFrame();
	Timing::Time frameTime = (float)this->GetFrameTime();
	ImGui::Begin("TestViewer", NULL, ImVec2(200, 200));
	ImGui::TextColored(ImVec4(0.7f, 0.3f, 0.3f, 1.0f), "FPS: %.2f", 1 / frameTime);

    if (this->benchmarkmode)
    {
        // benchmark with avg frames for every 100 frames
        if (this->frameTimes.Size() > 0 || frameTime < 0.08)
        {
            this->frameTimes.Append(frameTime);
        }       
        if (this->frameTimes.Size() > 0 && this->frameTimes.Size() % 50 == 0)
        {
            this->avgFPS = 0;
            IndexT i;
            for (i = 0; i < this->frameTimes.Size(); ++i)
            {
        	    this->avgFPS += this->frameTimes[i];
            }
            this->avgFPS /=  this->frameTimes.Size();
        }   
        if (this->avgFPS > 0)
        {
			ImGui::Text("Benchmarking: Avg Game FPS: %.2f", 1 / this->avgFPS);
        }
        else
        {
			ImGui::Text("Benchmarking: Wait for measuring average framerate");
        }
    }               
	if (ImGui::Button("Benchmark")) { this->benchmarkmode = !this->benchmarkmode; }
	if (ImGui::Button("Render debug")) { this->renderDebug = !this->renderDebug; }
	if (ImGui::TreeNode("Render stats"))
	{
		ImGui::Text("Currently showing %d point lights", this->pointLights.Size());
		ImGui::Text("Currently showing %d spot lights", this->spotLights.Size());
		ImGui::Text("Currently showing %d models", this->models.Size());
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Spot lights"))
	{
		String label;
		for (IndexT i = 0; i < this->spotLights.Size(); i++)
		{
			label.Format("SpotLight%d", i);
			if (ImGui::TreeNode(label.AsCharPtr()))
			{
				float color[3];
				int intensity;
				const Math::float4& c = this->spotLights[i]->GetColor();
				color[0] = c.x() / c.w();
				color[1] = c.y() / c.w();
				color[2] = c.z() / c.w();
				intensity = (int)c.w();
				ImGui::ColorEdit3("Color", color);
				ImGui::SliderInt("Intensity", &intensity, 1, 25);
				this->pointLights[i]->SetColor(Math::float4(color[0] * intensity, color[1] * intensity, color[2] * intensity, (float)intensity));
				ImGui::TreePop();
			}
		}
		if (this->spotLights.Size() == 0) ImGui::Text("No spotlights in the scene!");
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Point lights"))
	{
		String label;
		for (IndexT i = 0; i < this->pointLights.Size(); i++)
		{
			label.Format("PointLight%d", i);
			if (ImGui::TreeNode(label.AsCharPtr()))
			{
				float color[4];
				int intensity;
				const Math::float4& c = this->pointLights[i]->GetColor();
				color[0] = c.x() / c.w();
				color[1] = c.y() / c.w();
				color[2] = c.z() / c.w();
				intensity = (int)c.w();
				ImGui::ColorEdit3("Color", color);
				ImGui::SliderInt("Intensity", &intensity, 1, 25);
				this->pointLights[i]->SetColor(Math::float4(color[0] * intensity, color[1] * intensity, color[2] * intensity, (float)intensity));
				ImGui::TreePop();
			}
		}
		if (this->pointLights.Size() == 0) ImGui::Text("No pointLights in the scene!");
		ImGui::TreePop();
	}
	if (ImGui::TreeNode("Global light"))
	{
		float color[4];
		Math::float4 c = this->globalLight->GetColor();
		color[0] = c.x();
		color[1] = c.y();
		color[2] = c.z();
		color[3] = c.w();
		ImGui::ColorEdit4("Diffuse color", color);
		this->globalLight->SetColor(Math::float4(color[0], color[1], color[2], color[3]));

		c = this->globalLight->GetAmbientLightColor();
		color[0] = c.x();
		color[1] = c.y();
		color[2] = c.z();
		color[3] = c.w();
		ImGui::ColorEdit4("Ambient color", color);
		this->globalLight->SetAmbientLightColor(Math::float4(color[0], color[1], color[2], color[3]));

		c = this->globalLight->GetBackLightColor();
		color[0] = c.x();
		color[1] = c.y();
		color[2] = c.z();
		color[3] = c.w();
		ImGui::ColorEdit4("Backlight color", color);
		this->globalLight->SetBackLightColor(Math::float4(color[0], color[1], color[2], color[3]));

		bool b = this->globalLight->GetCastShadows();
		ImGui::Checkbox("Cast shadows", &b);
		this->globalLight->SetCastShadows(b);

		// pop tree node
		ImGui::TreePop();
	}
	ImGui::End();

	this->ui->EndFrame();
	          
    float time = (float) this->GetTime();
    IndexT i;
    for (i = 0; i < this->pointLights.Size(); i++)
    {
        matrix44 m = this->lightTransforms[i];
        float4 t(5 * n_sin(0.1f * i * time), 0.0f, 5 * n_sin(0.05f * i * time), 0.0f);
        m.translate(t);
        this->pointLights[i]->SetTransform(m);        
    }

    if (this->models.Size() > 0)
    {
        const matrix44& firstOne = this->models[0]->GetTransform();
        if (firstOne.get_position().z() > 30.0f
            || firstOne.get_position().z() < -30.0f)
        {
            direction *= -1;
        }
        const matrix44& lastOne = this->models.Back()->GetTransform();
        // update shadow point of interest
        Ptr<Graphics::SetShadowPointOfInterest> poiMsg = Graphics::SetShadowPointOfInterest::Create();
        poiMsg->SetPoi((firstOne.get_position() + lastOne.get_position()) * 0.5f);
        GraphicsInterface::Instance()->Send(poiMsg.cast<Messaging::Message>());
    }

	/*
	IndexT modelCounter = 0;
	for (int i = -5; i < 5; i++)
	{
		for (int j = -2; j < 2; j++)
		{
			matrix44 trans = matrix44::translation(i * 10 + sin(time*2), sin(time*2), j * 10 + cos(time*2));
			this->models[modelCounter]->SetTransform(trans);
		
			modelCounter++;
		}
	}
	*/
    

    // rotate spotlights
    for (i = 0; i < this->spotLights.Size(); i++)
    {
        matrix44 spotLightTransform = matrix44::rotationyawpitchroll(time * 2 * (i+1) / this->spotLights.Size(), n_deg2rad(-55), 0);        
        spotLightTransform.scale(point(i * 0.2f + 15, 30, i * 0.2f + 15));
        spotLightTransform.set_position(this->spotLights[i]->GetTransform().get_position());
        this->spotLights[i]->SetTransform(spotLightTransform);
    }


	matrix44 trans = matrix44::translation(10 + sin(time*2), 1, 10 + cos(time*2));
	this->billboard->SetTransform(trans);	
	


    // render a few debug shapes 
    /*IndexT x;
    for (x = 0; x < 10; x++)
    {
        IndexT y;
        for (y = 0; y < 10; y++)
        {
            quaternion rot = quaternion::rotationyawpitchroll(0.0f, curTime, 0.0f);
            matrix44 m = matrix44::affinetransformation(1.0f, vector::nullvec(), rot, point(x * 2.0f, 1.0f, y * 2.0f));
            DebugShapeRenderer::Instance()->DrawBox(m, float4(1.0f, 0.0f, 0.0f, 0.5f));
        }
    }*/
    
    // move light
    Ptr<Input::GamePad> pad = Input::InputServer::Instance()->GetDefaultGamePad(0);
    n_assert(pad.isvalid());
    matrix44 tr = this->testSpotLight->GetTransform();
    if (pad->ButtonPressed(Input::GamePad::DPadLeftButton))
    {
        tr.translate(vector(0, 0, -0.1f));
    }
    if (pad->ButtonPressed(Input::GamePad::DPadRightButton))
    {
        tr.translate(vector(0, 0, 0.1f));
    }
    if (pad->ButtonPressed(Input::GamePad::DPadUpButton))
    {
        tr.translate(vector(0, 0.1f, 0));
    }
    if (pad->ButtonPressed(Input::GamePad::DPadDownButton))
    {
        tr.translate(vector(0, -0.1f, 0));
    }
    if (pad->ButtonPressed(Input::GamePad::L1Button))
    {
        this->rotX += 0.5f;
        matrix44 rot = matrix44::rotationx(n_deg2rad(this->rotX));
        rot.scale(point(10,10,10));
        rot.set_position(tr.get_position());
        tr = rot;
    }
    if (pad->ButtonPressed(Input::GamePad::R1Button))
    {
        this->rotX -= 0.5f;
        matrix44 rot = matrix44::rotationx(n_deg2rad(this->rotX));
        rot.scale(point(10,10,10));
        rot.set_position(tr.get_position());
        tr = rot;
    }
    this->testSpotLight->SetTransform(tr);

    ViewerApplication::OnUpdateFrame();
}

//------------------------------------------------------------------------------
/**
*/
void
TestViewerApplication::AppendTestModel()
{              
    IndexT j;
    IndexT i;
    bool createStatic = true;
    float spacing = 5.0f;
    for (j = 0; j < 4; ++j)
    {
        for (i = 0; i < 4; ++i)
        {
            Ptr<ModelEntity> model = ModelEntity::Create();
            float dir = (i % 2 ? -1.0f : 1.0f);
            float x = (i+1) * spacing * dir - dir * 0.5f * spacing;
            model->SetTransform(matrix44::translation(x, 0.0, 2.0f * spacing * j));
            if (createStatic)
            {                                                                   
                model->SetResourceId(ResourceId("mdl:examples/tiger_lod.n3"));  
                this->stage->AttachEntity(model.cast<GraphicsEntity>());
            }
            else
            {                                                             
                //model->SetResourceId(ResourceId("mdl:characters/mensch_m.n3"));
				model->SetResourceId(ResourceId("mdl:characters/wing_skeleton.n3"));
                this->stage->AttachEntity(model.cast<GraphicsEntity>());

                // apply skin
                Ptr<Graphics::ApplySkinList> skinList = Graphics::ApplySkinList::Create();
                //skinList->SetSkinList(StringAtom("mann_nackt"));
				skinList->SetSkinList(StringAtom("_auto_default_"));
                model->HandleMessage(skinList.cast<Messaging::Message>());

                // play animation
                Ptr<Graphics::AnimPlayClip> playClip = Graphics::AnimPlayClip::Create();
				//playClip->SetClipName("gehen_01");
                playClip->SetClipName("twist");
                playClip->SetTrackIndex(0);
                playClip->SetLoopCount(1.0f);
                model->HandleMessage(playClip.cast<Messaging::Message>());
            } 
            // createStatic  = !createStatic;
            this->models.Append(model);  
        } 
    }   
}


} // namespace Tools
