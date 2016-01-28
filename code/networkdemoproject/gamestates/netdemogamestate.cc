//------------------------------------------------------------------------------
//  DemoGameState.cc
//  (C) 2013-2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "netdemogamestate.h"
#include "math/vector.h"
#include "math/matrix44.h"
#include "graphicsfeatureunit.h"
#include "managers/factorymanager.h"
#include "managers/entitymanager.h"
#include "managers/enventitymanager.h"
#include "scriptfeature/managers/scripttemplatemanager.h"
#include "attr/attribute.h"
#include "graphicsfeature/graphicsattr/graphicsattributes.h"
#include "managers/focusmanager.h"
#include "input/keyboard.h"
#include "scriptingfeature/properties/scriptingproperty.h"
#include "scriptingfeature/scriptingprotocol.h"
#include "effects/effectsfeatureunit.h"
#include "networkdemoproject/netdemogameapplication.h"

namespace Demos
{
__ImplementClass(Demos::DemoGameState, 'DMGS', BaseGameFeature::GameStateHandler);

using namespace BaseGameFeature;
using namespace GraphicsFeature;
using namespace Util;
using namespace Math;

//------------------------------------------------------------------------------
/**
*/
DemoGameState::DemoGameState()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
DemoGameState::~DemoGameState()
{
	// empty
}

//------------------------------------------------------------------------------
/**
*/
void 
DemoGameState::OnStateEnter( const Util::String& prevState )
{
	GameStateHandler::OnStateEnter(prevState);	

	const Ptr<UI::UiLayout>& layout = UI::UiFeatureUnit::Instance()->GetLayout("demo");
	Ptr<UI::UiElement> element = layout->GetElement("updatetext");
	element->SetText("Entered state");
	
}

//------------------------------------------------------------------------------
/**
*/
void 
DemoGameState::OnStateLeave( const Util::String& nextState )
{
	GameStateHandler::OnStateLeave(nextState);
}

//------------------------------------------------------------------------------
/**
*/
Util::String 
DemoGameState::OnFrame()
{
	//handle all user input
	if (Input::InputServer::HasInstance())
	{
		this->HandleInput();
	}
		
	return GameStateHandler::OnFrame();
}

//------------------------------------------------------------------------------
/**
*/
void 
DemoGameState::OnLoadBefore()
{
	
}

//------------------------------------------------------------------------------
/**
*/
void 
DemoGameState::OnLoadAfter()
{

}

//------------------------------------------------------------------------------
/**
*/
void 
DemoGameState::HandleInput()
{
	const Ptr<Input::Keyboard>& kbd = Input::InputServer::Instance()->GetDefaultKeyboard();


	// reload layout if key gets pressed
	if (kbd->KeyDown(Input::Key::F1))
	{
        const Ptr<UI::UiLayout>& layout = UI::UiFeatureUnit::Instance()->GetLayout("demo");
        layout->Reload();
	}
	if(kbd->KeyDown(Input::Key::X))
	{
		DemoProjectApplication::Instance()->RequestState("Exit");
	}
	if (kbd->KeyDown(Input::Key::F2))
	{
		EffectsFeature::EffectsFeatureUnit::Instance()->EmitGraphicsEffect(Math::matrix44::translation(n_rand(-5, 5), 10, n_rand(-5, 5)), "mdl:particles/newparticle.n3", 10.0f);
	}
	if (kbd->KeyDown(Input::Key::C))
	{
		MultiplayerFeature::NetworkGame::Instance()->CreateRoom();
	}
	if (kbd->KeyDown(Input::Key::U))
	{
		MultiplayerFeature::NetworkGame::Instance()->UnpublishFromMaster();
	}
	if (kbd->KeyDown(Input::Key::R))
	{
		MultiplayerFeature::NetworkGame::Instance()->UpdateRoomList();
	}
	if (kbd->KeyDown(Input::Key::J))
	{
		Util::String id = MultiplayerFeature::NetworkGame::Instance()->GetMasterList()->GetString(Attr::Id,0);
		MultiplayerFeature::NetworkGame::Instance()->JoinRoom(id);

	}
	if (kbd->KeyDown(Input::Key::G))
	{
		App::GameApplication::Instance()->FindStateHandlerByName("DemoState").cast<BaseGameFeature::GameStateHandler>()->SetLevelName("net");
		App::GameApplication::Instance()->FindStateHandlerByName("DemoState").cast<BaseGameFeature::GameStateHandler>()->SetSetupMode(BaseGameFeature::GameStateHandler::LoadNetworkedLevel);
		App::GameApplication::Instance()->RequestState("DemoState");
	}
	if (kbd->KeyDown(Input::Key::Q))
	{
		Ptr<Game::Entity> entity = FactoryManager::Instance()->CreateEntityByTemplate("Simple", "box");		
		Math::matrix44 trans;
		trans.translate(Math::float4(0, 4, 0, 0));
		entity->SetMatrix44(Attr::Transform, trans);
		EntityManager::Instance()->AttachEntity(entity);
		this->boxes.Append(entity);
	}
	if (kbd->KeyDown(Input::Key::E))
	{
		for (int i = 0; i < this->boxes.Size(); i++)
		{
			EntityManager::Instance()->RemoveEntity(this->boxes[i]);
		}
		this->boxes.Clear();
	}
	if (kbd->KeyDown(Input::Key::P))
	{
		this->player = FactoryManager::Instance()->CreateEntityByTemplate("Player", "dummychar");
		EntityManager::Instance()->AttachEntity(this->player);
	}
}
} // namespace Tools