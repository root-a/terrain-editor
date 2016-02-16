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
#include "brushattributes.h"

namespace Terrain
{ 
	enum KeyMod
	{
		Shift,
		Ctrl,
		None
	};
	class Brush : public Core::RefCounted
	{
		__DeclareClass(Brush);
	public:
		/// constructor
		Brush();
		/// destructor
		virtual ~Brush();
		void SetAttributes(Ptr<Terrain::BrushAttributes> attributes);
		virtual void ExecuteBrushFunction(const Math::float4& pos, float* textureBuffer, const Math::float2& textureSize, const KeyMod modifier, float maxHeight);

		Ptr<Terrain::BrushAttributes> attributes;
	private:
		
	};
} // namespace Grid