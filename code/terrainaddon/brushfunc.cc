//------------------------------------------------------------------------------
//  terrainaddon.cc
//  (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core\rttimacros.h"
#include "brushfunc.h"
#include "resources\resourcemanager.h"
using namespace Math;

namespace Terrain
{
	__ImplementClass(Terrain::BrushFunction, 'TBFN', Core::RefCounted);

	//------------------------------------------------------------------------------
	/**
	*/
	BrushFunction::BrushFunction()
	{
		
	}

	//------------------------------------------------------------------------------
	/**
	*/
	BrushFunction::~BrushFunction()
	{
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		BrushFunction::ExecuteBrushFunction(int radius, float strength, const Ptr<Terrain::BrushTexture> brushtexture, const Math::float4& pos, float* textureBuffer, const Math::float2& textureSize, const float modifier, float maxHeight)
	{
		//using the attributes update the cpu buffer of texture
		int x = (int)pos.x();
		int y = (int)pos.z();
		
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

		int currentBrushIndex = 0;
		for (int y_start = y_startInit; y_start < y_end; y_start++)
		{
			int currentColBufferIndex = height*y_start;
			int currentColBrushIndex = brushtexture->size*y_brush_start;
			int x_brush_start = x_brush_startInit;
			for (int x_start = x_startInit; x_start < x_end; x_start++)
			{
				int currentBufferIndex = currentColBufferIndex + x_start;
				currentBrushIndex = currentColBrushIndex + x_brush_start;

				float brushValue = brushtexture->sampledBrushBuffer[currentBrushIndex];
				float textureValue = textureBuffer[currentBufferIndex];

				textureValue += (strength*brushValue*modifier);
				textureValue = Math::n_clamp(textureValue, 0.f, maxHeight);
				textureBuffer[currentBufferIndex] = textureValue;

				x_brush_start++;
			}
			y_brush_start++;
		}
	}

} // namespace Terrain