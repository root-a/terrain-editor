//------------------------------------------------------------------------------
//  demogameapplication.cc
//  (C) 2011-2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "graphics/graphicsprotocol.h"
#include "messaging/message.h"
#include "input/keyboard.h"
#include "input/gamepad.h"
#include "managers/factorymanager.h"
#include "managers/entitymanager.h"
#include "gamestates/demogamestate.h"
#include "physicsfeature/physicsprotocol.h"
#include "graphicsfeature/graphicsfeatureprotocol.h"
#include "basegamefeature/basegameprotocol.h"
#include "scriptingfeature/scriptingcommands.h"
#include "demogameapplication.h"
#include "coregraphics/displaymode.h"
#include "uicommands.h"


namespace Tools
{
using namespace Graphics;
using namespace Math;
using namespace Input;
using namespace BaseGameFeature;
using namespace Game;

//------------------------------------------------------------------------------
/**
*/
DemoProjectApplication::DemoProjectApplication(void)
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
DemoProjectApplication::~DemoProjectApplication(void)
{
	if (this->IsOpen())
	{
		this->Close();
	}
}

//------------------------------------------------------------------------------
/**
*/
bool DemoProjectApplication::Open()
{
	n_assert(!this->IsOpen());
	if (GameApplication::Open())
	{
		return true;
	}
	return false;
}

//------------------------------------------------------------------------------
/**
*/
void DemoProjectApplication::Close()
{
	GameApplication::Close();
}


//------------------------------------------------------------------------------
/**
*/
void 
DemoProjectApplication::SetupStateHandlers()
{
	Ptr<DemoGameState> gameState = DemoGameState::Create();
	gameState->SetSetupMode(DemoGameState::NewGame);
	gameState->SetName("DemoState");
	// select the level to be loaded explicitly instead of the default one
	gameState->SetLevelName("demo_full");

	this->AddStateHandler(gameState.get());

	this->SetState("DemoState");


}

//------------------------------------------------------------------------------
/**
*/
void 
DemoProjectApplication::SetupGameFeatures()
{
	GameApplication::SetupGameFeatures();

	// create and attach default graphic features
	this->graphicsFeature = GraphicsFeature::GraphicsFeatureUnit::Create();
	this->graphicsFeature->SetCmdLineArgs(this->GetCmdLineArgs());
	// disable resizing of window
    this->graphicsFeature->SetResizeable(false);
	// enable debug rendering of graphics
	this->graphicsFeature->SetRenderDebug(true);
	this->graphicsFeature->SetupDisplay();

	// game features
	this->baseGameFeature = BaseGameFeatureUnit::Create();
	this->baseGameFeature->SetRenderDebug(true);

	// create fx feature
	this->fxFeature = EffectsFeature::EffectsFeatureUnit::Create();
		
	// create and attach core features
	this->physicsFeature = PhysicsFeature::PhysicsFeatureUnit::Create(); 
	this->physicsFeature->SetRenderDebug(true);

	// setup scripting
	this->scriptingFeature = ScriptingFeature::ScriptingFeatureUnit::Create();

	// setup ui
	this->uiFeature = UI::UiFeatureUnit::Create();

	// setup posteffect
	this->postEffectFeature = PostEffect::PostEffectFeatureUnit::Create();

	this->gameServer->AttachGameFeature(this->graphicsFeature.cast<Game::FeatureUnit>());
	this->gameServer->AttachGameFeature(this->baseGameFeature.upcast<Game::FeatureUnit>());	
	this->gameServer->AttachGameFeature(this->fxFeature.upcast<Game::FeatureUnit>());
	this->gameServer->AttachGameFeature(this->physicsFeature.upcast<Game::FeatureUnit>());
	this->gameServer->AttachGameFeature(this->scriptingFeature.upcast<Game::FeatureUnit>());
	this->gameServer->AttachGameFeature(this->uiFeature.cast<Game::FeatureUnit>());
	this->gameServer->AttachGameFeature(this->postEffectFeature.cast<Game::FeatureUnit>());


	Commands::ScriptingCommands::Register();
	Commands::PhysicsProtocol::Register();
	Commands::GraphicsFeatureProtocol::Register();
	Commands::BaseGameProtocol::Register();
	Commands::UICommands::Register();

	Util::String foo = "1234";
	Util::String foo2 = foo.AsBase64();
	foo2.Strip("\r");
	Util::String foo3 = foo.AsBase64();
	foo3.Strip("\r");
	Util::String foo4 = foo.AsBase64();
	Util::String ff2 = Util::String::FromBase64(foo2);
	Util::String ff3 = Util::String::FromBase64(foo3);

	this->mainLayout = uiFeature->GetLayout("demo");
	this->mainLayout->Show();

	
#if !PUBLIC_BUILD
	this->uiFeature->SetRenderDebug(true);	
#endif

	// modify resolution
	Ptr<UpdateDisplay> upd = UpdateDisplay::Create();
	upd->SetFullscreen(true);
	CoreGraphics::DisplayMode mode;
	mode.SetWidth(1920);
	mode.SetHeight(1200);
	mode.SetAspectRatio(1920 / float(1200));
	upd->SetTripleBufferingEnabled(true);
	upd->SetDisplayMode(mode);
	// uncomment for setting fullscreen
	//GraphicsInterface::Instance()->Send(upd.upcast<Messaging::Message>());
}

//------------------------------------------------------------------------------
/**
*/
void 
DemoProjectApplication::CleanupGameFeatures()
{
	this->mainLayout = 0;	
	this->gameServer->RemoveGameFeature(this->postEffectFeature.upcast<Game::FeatureUnit>());
	this->postEffectFeature = 0;
	this->gameServer->RemoveGameFeature(this->fxFeature.upcast<Game::FeatureUnit>());
	this->fxFeature = 0;
	this->gameServer->RemoveGameFeature(this->baseGameFeature.upcast<Game::FeatureUnit>());
	this->baseGameFeature = 0;
	this->gameServer->RemoveGameFeature(this->uiFeature.upcast<Game::FeatureUnit>());
	this->uiFeature = 0;
	this->gameServer->RemoveGameFeature(this->physicsFeature.upcast<Game::FeatureUnit>());
	this->physicsFeature = 0;
	this->gameServer->RemoveGameFeature(this->scriptingFeature.upcast<Game::FeatureUnit>());
	this->scriptingFeature = 0;
	this->gameServer->RemoveGameFeature(this->graphicsFeature.upcast<Game::FeatureUnit>());
	this->graphicsFeature = 0;

	GameApplication::CleanupGameFeatures();
}
}
