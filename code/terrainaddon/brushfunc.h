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
#include "brushtexture.h"

namespace Terrain
{ 
	class BrushFunction : public Core::RefCounted
	{
		__DeclareClass(BrushFunction);
	public:
		/// constructor
		BrushFunction();
		/// destructor
		virtual ~BrushFunction();
		virtual void ExecuteBrushFunction(int radius, float strength, const Ptr<Terrain::BrushTexture> brushtexture, const Math::float4& pos, float* textureBuffer, const Math::float2& textureSize, const float modifier, float maxHeight);

		
	private:
		
	};
} // namespace Grid