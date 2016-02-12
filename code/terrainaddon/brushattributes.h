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
	class BrushAttributes : public Core::RefCounted
	{
		__DeclareClass(BrushAttributes);
	public:
		/// constructor
		BrushAttributes();
		/// destructor
		virtual ~BrushAttributes();

		/// setup
		void Setup(Resources::ResourceId resourceID);
		/// discard
		void Discard();

		float radius;
		float innerRadius;
		float strength;
		float* brushTextureBuffer;
		Ptr<Resources::ManagedTexture> texture;
	private:
		
	};
} // namespace Grid