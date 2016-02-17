//------------------------------------------------------------------------------
//  terrainaddon.cc
//  (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core\rttimacros.h"
#include "brush.h"
#include "resources\resourcemanager.h"
using namespace Math;

namespace Terrain
{
	__ImplementClass(Terrain::Brush, 'TBTL', Core::RefCounted);

	//------------------------------------------------------------------------------
	/**
	*/
	Brush::Brush()
	{
		
	}

	//------------------------------------------------------------------------------
	/**
	*/
	Brush::~Brush()
	{
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		Brush::ExecuteBrushFunction(const Math::float4& pos, float* textureBuffer, const Math::float2& textureSize, const KeyMod modifier, float maxHeight)
	{
		//using the attributes update the cpu buffer of texture
		int x = (int)pos.x();
		int y = (int)pos.z();
		int radius = (int)attributes->radius;
		float mod = 0.f;
		switch (modifier)
		{
		case KeyMod::Ctrl :
			mod = -1.f;
			break;
		default:
			mod = 1.f;
		}
		//we got texture buffer
		//we update it with using position and radius
		//then we send the data to the memory texture and update only the region of it

		//at pos x radius by 2
		//range of x values
		//x - (radius/2)
		//x + (radius/2)

		//range of y values
		//y - (radius/2)
		//y + (radius/2)

		//position in buffer
		//x*y
		//we start loop att
		//y - (radius/2)
		//we end loop at
		//y + (radius/2)
		//we do this for values from: //and it's where we perform the actual function on the pixels
		//x - (radius/2) //it's current y * x - (radius/2)
		//to
		//x + (radius/2) //it's current y * x + (radius/2)
		int height = (int)textureSize.y();
		int width = (int)textureSize.x();
		int size = height*width;
		

		//calculate region
		int y_start = y - radius;
		int x_startInit = x - radius;

		int y_end = y + radius;
		int x_end = x + radius;
		
		
		int y_brush_start = 0;
		int x_brush_startInit = 0;
		int y_brush_end = attributes->height;
		int x_brush_end = attributes->width;

		//have to clamp the range of x if the brush pos is on the border
		if (x_startInit < 0)
		{
			x_brush_startInit = x_brush_startInit - x_startInit;
			x_startInit = 0;
		}
		x_end = Math::n_min(width, x_end);
		/*
		if (x_end > width) 
		{
			x_brush_end = x_brush_end - (x_end - width); //no need to calculate ends for brush
			x_end = width;
		}
		*/
		if (y_start < 0)
		{
			y_brush_start = y_brush_start - y_start; //no need to calculate ends for brush
			y_start = 0;
		}
		y_end = Math::n_min(height, y_end);
		/*
		if (y_end > height)
		{
			y_brush_end = y_brush_end - (y_end - height);
			y_end = height;
		}
		*/
		//x_startInit = Math::n_max(0, x_startInit);
		//x_end = Math::n_min(width, x_end);
		//have to clamp the range of y if the brush pos is on the border
		//y_start = Math::n_max(0, y_start);
		//y_end = Math::n_min(height, y_end);

		//now we update only the region and we clamp it if big brush is close to the border

		int currentBrushIndex = 0;
		for (y_start; y_start < y_end; y_start++)
		{
			int currentColBufferIndex = height*y_start;
			int currentColBrushIndex = attributes->height*y_brush_start;
			int x_start = x_startInit;
			int x_brush_start = x_brush_startInit;
			for (x_start; x_start < x_end; x_start++)
			{
				int currentBufferIndex = currentColBufferIndex + x_start;
				currentBrushIndex = currentColBrushIndex + x_brush_start;

				float brushValue = attributes->brushTextureBuffer[currentBrushIndex];
				float textureValue = textureBuffer[currentBufferIndex];

				textureValue += (attributes->strength*brushValue*mod);
				textureValue = Math::n_clamp(textureValue, 0.f, maxHeight);
				textureBuffer[currentBufferIndex] = textureValue;

				x_brush_start++;
			}
			y_brush_start++;
		}
	}

	void Brush::SetAttributes(Ptr<Terrain::BrushAttributes> attributes)
	{
		this->attributes = attributes;
	}

} // namespace Terrain