#pragma once
//------------------------------------------------------------------------------
/**
    @class Demos::DemoGameState
    
    A basic game state
    
    (C) 2013-2015 Individual contributors, see AUTHORS file
*/
#include "statehandlers/gamestatehandler.h"
#include "graphics/spotlightentity.h"
#include "graphics/globallightentity.h"
#include "graphics/pointlightentity.h"
#include "game/entity.h"

//------------------------------------------------------------------------------
namespace Demos
{
class DemoGameState : public BaseGameFeature::GameStateHandler
{
	__DeclareClass(DemoGameState);
public:
	/// constructor
	DemoGameState();
	/// destructor
	virtual ~DemoGameState();

	/// called when the state represented by this state handler is entered
	virtual void OnStateEnter(const Util::String& prevState);
	/// called when the state represented by this state handler is left
	virtual void OnStateLeave(const Util::String& nextState);
	/// called each frame as long as state is current, return new state
	virtual Util::String OnFrame();
	/// called after Db is opened, and before entities are loaded
	virtual void OnLoadBefore();
	/// called after entities are loaded
	virtual void OnLoadAfter();

	// handle all user input; called @ LevelEditorState::OnFrame()
	void HandleInput();

private:
	Util::Array<Ptr<Game::Entity>> boxes;
	Ptr<Game::Entity> player;
}; 
} // namespace Demos
//------------------------------------------------------------------------------