#pragma once
//------------------------------------------------------------------------------
/**
@class Terrain::TerrainAddon

Renders terrain with specified resolution based on height-map

(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"
#include "core/singleton.h"
#include "terrainplugin.h"
namespace Terrain
{
	class TerrainAddon : public Core::RefCounted
	{
		__DeclareClass(TerrainAddon);
		__DeclareSingleton(TerrainAddon);
	public:
		/// constructor
		TerrainAddon();
		/// destructor
		virtual ~TerrainAddon();

		/// setup
		void Setup();
		/// discard
		void Discard();

		/// set if grid should be visible
		void SetVisible(bool b);
		/// set grid size
		void SetGridSize(float size);

	private:
		Ptr<TerrainRTPlugin> plugin;
	};
} // namespace Grid