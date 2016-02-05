//------------------------------------------------------------------------------
//  gridaddon.cc
//  (C) 2012-2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "terrainaddon.h"
#include "rendermodules/rt/rtpluginregistry.h"
#include "terrainplugin.h"

namespace Terrain
{
	__ImplementClass(Terrain::TerrainAddon, 'TRAD', Core::RefCounted);
	__ImplementSingleton(Terrain::TerrainAddon);

	//------------------------------------------------------------------------------
	/**
	*/
	TerrainAddon::TerrainAddon()
	{
		__ConstructSingleton;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	TerrainAddon::~TerrainAddon()
	{
		__DestructSingleton;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		TerrainAddon::Setup()
	{
		// register render plugin
		RenderModules::RTPluginRegistry::Instance()->RegisterRTPlugin(&TerrainRTPlugin::RTTI);
		this->plugin = RenderModules::RTPluginRegistry::Instance()->GetPluginByRTTI<TerrainRTPlugin>(&TerrainRTPlugin::RTTI);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		TerrainAddon::Discard()
	{
		this->plugin = 0;

		// deregister plugin
		RenderModules::RTPluginRegistry::Instance()->UnregisterRTPlugin(&TerrainRTPlugin::RTTI);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		TerrainAddon::SetVisible(bool b)
	{
		this->plugin->SetVisible(b);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		TerrainAddon::SetGridSize(float size)
	{
		this->plugin->SetGridSize(size);
	}

	void TerrainAddon::UpdateTexture(void* data, SizeT size, SizeT width, SizeT height, IndexT left, IndexT top, IndexT mip)
	{
		this->plugin->UpdateTexture(data, size, width, height, left, top, mip);
	}

} // namespace Grid