//------------------------------------------------------------------------------
//  terrainaddon.cc
//  (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core\rttimacros.h"
#include "brushsmooth.h"
#include "resources\resourcemanager.h"
using namespace Math;

namespace Terrain
{
	__ImplementClass(Terrain::BrushSmooth, 'TSBT', Core::RefCounted);

	//------------------------------------------------------------------------------
	/**
	*/
	BrushSmooth::BrushSmooth()
	{
		
	}

	//------------------------------------------------------------------------------
	/**
	*/
	BrushSmooth::~BrushSmooth()
	{
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		BrushSmooth::ExecuteBrushFunction(const Math::float4& pos, float* textureBuffer, const Math::float2& textureSize, const KeyMod modifier, float maxHeight)
	{
		//using the attributes update the cpu buffer of texture
		int x = (int)pos.x();
		int y = (int)pos.z();
		int radius = attributes->GetRadius();
		float mod = 0.f;
		switch (modifier)
		{
		case KeyMod::Ctrl :
			mod = -1.f;
			break;
		default:
			mod = 1.f;
		}
		
		int height = (int)textureSize.y();
		int width = (int)textureSize.x();

		//calculate region
		int y_startInit = y - radius;
		int x_startInit = x - radius;

		int y_end = y + radius;
		int x_end = x + radius;
		
		//calculate brush size, we need to clamp at borders
		int y_brush_start = 0;
		int x_brush_startInit = 0;

		//have to clamp the range of x if the brush pos is at the border
		if (x_startInit < 0)
		{
			x_brush_startInit = x_brush_startInit - x_startInit;
			x_startInit = 0;
		}
		x_end = Math::n_min(width, x_end);
		if (y_startInit < 0)
		{
			y_brush_start = y_brush_start - y_startInit; //no need to calculate ends for brush
			y_startInit = 0;
		}
		y_end = Math::n_min(height, y_end);

		//now we update only the region and we clamp it if big brush is close to the border

		//first i will need to get the region 
		//blur it
		//and apply

		//store region
		int blurRegionWidth = (x_end - x_startInit);
		int blurRegionHeight = (y_end - y_startInit);
		int regionSize = blurRegionWidth*blurRegionHeight*4;
		if (regionSize <= 0) return;
		float * regionToBlur = (float*)Memory::Alloc(Memory::DefaultHeap, regionSize);

		float* blurredRegion = (float*)Memory::Alloc(Memory::DefaultHeap, regionSize);

		int regionIndex = 0;
		for (int y_start = y_startInit; y_start < y_end; y_start++)
		{
			int currentColBufferIndex = height*y_start;
			for (int x_start = x_startInit; x_start < x_end; x_start++)
			{
				int currentBufferIndex = currentColBufferIndex + x_start;
				
				regionToBlur[regionIndex] = textureBuffer[currentBufferIndex];
				regionIndex++;
			}
		}

		GaussianBlur(regionToBlur, blurredRegion, blurRegionWidth, blurRegionHeight, 5);
		Memory::Free(Memory::DefaultHeap, regionToBlur);
		
		//apply the blurred data

		int currentBrushIndex = 0;
		regionIndex = 0;
		for (int y_start = y_startInit; y_start < y_end; y_start++)
		{
			int currentColBufferIndex = height*y_start;
			int currentColBrushIndex = attributes->size*y_brush_start;
			int x_brush_start = x_brush_startInit;
			for (int x_start = x_startInit; x_start < x_end; x_start++)
			{
				int currentBufferIndex = currentColBufferIndex + x_start;
				currentBrushIndex = currentColBrushIndex + x_brush_start;

				float brushValue = attributes->sampledBrushBuffer[currentBrushIndex] / 255.f; //normalize to use as mask, brush values are from 0 - 255
				float bluredValue = blurredRegion[regionIndex];
				float textureValue = textureBuffer[currentBufferIndex];
				float interpolatedValue = Math::n_lerp(textureValue, bluredValue, brushValue);
				textureBuffer[currentBufferIndex] = interpolatedValue;
				regionIndex++;
				x_brush_start++;
			}
			y_brush_start++;
		}
		Memory::Free(Memory::DefaultHeap, blurredRegion);
	}

	float* BrushSmooth::BoxesForGauss(float radius, int n)  // standard deviation, number of boxes
	{
		float wIdeal = Math::n_sqrt((12.f * radius*radius / (float)n) + 1.f);  // Ideal averaging filter width 
		float wl = Math::n_floor(wIdeal);  if ((int)wl % 2 == 0) wl--;
		float wu = wl + 2.f;

		float mIdeal = (12.f * radius*radius - (float)n*wl*wl - 4.f * (float)n*wl - 3.f * (float)n) / (-4.f * wl - 4.f);
		int m = (int)Math::n_floor(mIdeal+0.5f);
		// var sigmaActual = Math.sqrt( (m*wl*wl + (n-m)*wu*wu - n)/12 );

		float* sizes = new float[n];  for (int i = 0; i < n; i++) sizes[i] = (i < m ? wl : wu);
		return sizes;
	}

	//linear time gaussian blur
	void BrushSmooth::GaussianBlur(float* source, float* dest, const int width, const int height, const float radius)
	{
		float* bxs = BoxesForGauss(radius, 3);
		BoxBlur(source, dest, width, height, (int)((bxs[0] - 1.f) / 2.f));
		BoxBlur(dest, source, width, height, (int)((bxs[1] - 1.f) / 2.f));
		BoxBlur(source, dest, width, height, (int)((bxs[2] - 1.f) / 2.f));
	}

	void BrushSmooth::BoxBlur(float* source, float* dest, const int width, const int height, const int radius)
	{
		int size = width*height;
		for (int i = 0; i < size; i++) dest[i] = source[i];
		BoxBlurH(dest, source, width, height, radius);
		BoxBlurT(source, dest, width, height, radius);
	}

	void BrushSmooth::BoxBlurH(float* source, float* dest, const int width, const int height, const int radius)
	{
		float iarr = 1.f / (float)(radius + radius + 1);
		for (int i = 0; i < height; i++) {
			int ti = i*width, li = ti, ri = ti + radius;
			float fv = source[ti], lv = source[ti + width - 1], val = ((float)radius + 1.f)*fv;
			for (int j = 0; j < radius; j++)				    val += source[ti + j];
			for (int j = 0; j <= radius; j++)				  { val += source[ri++] - fv;			  dest[ti++] = val*iarr; }
			for (int j = radius + 1; j < width - radius; j++) { val += source[ri++] - source[li++];   dest[ti++] = val*iarr; }
			for (int j = width - radius; j < width; j++)	  { val += lv - source[li++];			  dest[ti++] = val*iarr; }
		}
	}

	void BrushSmooth::BoxBlurT(float* source, float* dest, const int width, const int height, const int radius)
	{
		float iarr = 1.f / (float)(radius + radius + 1);
		for (int i = 0; i < width; i++) {
			int ti = i, li = ti, ri = ti + radius*width;
			float fv = source[ti], lv = source[ti + width*(height - 1)], val = ((float)radius + 1.f)*fv;
			for (int j = 0; j < radius; j++)							 val += source[ti + j*width];
			for (int j = 0; j <= radius; j++)						   { val += source[ri] - fv;			dest[ti] = val*iarr;  ri += width; ti += width; }
			for (int j = radius + 1; j < height - radius; j++)		   { val += source[ri] - source[li];	dest[ti] = val*iarr;  li += width; ri += width; ti += width; }
			for (int j = height - radius; j < height; j++)			   { val += lv - source[li];			dest[ti] = val*iarr;  li += width; ti += width; }
		}
	}

} // namespace Terrain