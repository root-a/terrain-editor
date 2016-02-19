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
#include "brushfunc.h"

namespace Terrain
{ 
	class BrushSmooth : public BrushFunction
	{
		__DeclareClass(BrushSmooth);
	public:
		/// constructor
		BrushSmooth();
		/// destructor
		virtual ~BrushSmooth();
		virtual void ExecuteBrushFunction(int radius, float strength, const Ptr<Terrain::BrushTexture> brushtexture, const Math::float4& pos, float* textureBuffer, const Math::float2& textureSize, const float modifier, float maxHeight);
		
	private:
		float* BoxesForGauss(float radius, int n);
		void GaussianBlur(float* source, float* dest, const int width, const int height, const float radius);
		void BoxBlur(float* source, float* dest, const int width, const int height, const int radius);
		void BoxBlurH(float* source, float* dest, const int width, const int height, const int radius);
		void BoxBlurT(float* source, float* dest, const int width, const int height, const int radius);
	};
} // namespace Grid