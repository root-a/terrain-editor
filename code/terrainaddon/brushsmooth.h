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
#include "brush.h"

namespace Terrain
{ 
	class BrushSmooth : public Brush
	{
		__DeclareClass(BrushSmooth);
	public:
		/// constructor
		BrushSmooth();
		/// destructor
		virtual ~BrushSmooth();
		virtual void ExecuteBrushFunction(const Math::float4& pos, float* textureBuffer, const Math::float2& textureSize, const KeyMod modifier, float maxHeight);
		
	private:
		float* BoxesForGauss(float radius, int n);
		void GaussianBlur(float* source, float* dest, const int width, const int height, const float radius);
		void BoxBlur(float* source, float* dest, const int width, const int height, const int radius);
		void BoxBlurH(float* source, float* dest, const int width, const int height, const int radius);
		void BoxBlurT(float* source, float* dest, const int width, const int height, const int radius);
	};
} // namespace Grid