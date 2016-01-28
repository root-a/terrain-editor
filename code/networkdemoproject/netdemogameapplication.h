#pragma once
//------------------------------------------------------------------------------
/**
	@class Demos::DemoProjectApplication

	NebulaT network demo project application.

	(C) 2011-2015 Individual contributors, see AUTHORS file
*/
#include "graphics/spotlightentity.h"
#include "graphics/modelentity.h"
#include "graphics/globallightentity.h"
#include "graphics/pointlightentity.h"
#include "graphicsfeatureunit.h"
#include "physicsfeatureunit.h"
#include "basegamefeatureunit.h"
#include "appgame/gameapplication.h"
#include "scriptingfeature/scriptingfeature.h"
#include "ui/uifeatureunit.h"
#include "effects/effectsfeatureunit.h"
#include "forest/forestrendermodule.h"
#include "posteffect/posteffectfeatureunit.h"
#include "multiplayerfeatureunit.h"
#include "networkgame.h"
#include "networkplayer.h"

namespace  Demos
{
class DemoProjectApplication : public App::GameApplication
{
public:
	/// constructor
	DemoProjectApplication(void);
	/// destructor
	virtual ~DemoProjectApplication(void);
	/// open application
	virtual bool Open();
	/// close application
	virtual void Close();

private:

	/// setup application state handlers
	virtual void SetupStateHandlers();
	/// setup game features
	virtual void SetupGameFeatures();
	/// cleanup game features
	virtual void CleanupGameFeatures();

	Ptr<UI::UiFeatureUnit> uiFeature;
	Ptr<UI::UiLayout> mainLayout;
	Ptr<PhysicsFeature::PhysicsFeatureUnit> physicsFeature;
	Ptr<EffectsFeature::EffectsFeatureUnit> fxFeature;
	Ptr<GraphicsFeature::GraphicsFeatureUnit> graphicsFeature;
	Ptr<BaseGameFeature::BaseGameFeatureUnit> baseGameFeature;
	Ptr<ScriptingFeature::ScriptingFeatureUnit> scriptingFeature;	
	Ptr<PostEffect::PostEffectFeatureUnit> postEffectFeature;
	Ptr<MultiplayerFeature::MultiplayerFeatureUnit> multiplayerFeature;
	Ptr<MultiplayerFeature::NetworkGame> networkGame;
	Ptr<MultiplayerFeature::NetworkPlayer> player;
};

}
