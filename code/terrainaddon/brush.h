#pragma once
//------------------------------------------------------------------------------
/**
@class Terrain::TerrainAddon

Brush 

(C) 2016 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "core/refcounted.h"
#include "resources/managedtexture.h"

namespace Terrain
{
	class Brush : public Core::RefCounted
	{
		__DeclareClass(Brush);
	public:
		/// constructor
		Brush();
		/// destructor
		virtual ~Brush();

		/// setup
		void Setup(Resources::ResourceId resourceID);
		/// discard
		void Discard();

		float radius;
		float innerRadius;
		Resources::ResourceId resourceID;
		Ptr<Resources::ManagedTexture> texture;
	private:
		
	};
} // namespace Grid