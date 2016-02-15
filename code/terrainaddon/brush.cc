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
		Brush::ExecuteBrushFunction(const Math::float4& pos, float* textureBuffer, const Math::float2& textureSize)
	{
		//using the attributes update the cpu buffer of texture
		int x = (int)pos.x();
		int y = (int)pos.z();
		int radius = (int)attributes->radius;
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

		//int y_start = (y - (diameter / 2));
		//n_printf("\ny: %d\n", y);
		int y_end = y + radius;
		//int x_start = x - (diameter / 2);
		int x_end = x + radius;
		//int currentColBufferIndex = y*height + x;
		//textureBuffer[currentColBufferIndex] = attributes->strength;
		
		for (int y_start = y - radius; y_start < y_end; y_start++)
		{
			int currentColBufferIndex = height*y_start;
			for (int x_start = x - radius; x_start < x_end; x_start++)
			{
				int currentBufferIndex = currentColBufferIndex+x_start;
				textureBuffer[currentBufferIndex] = attributes->strength;
			}
		}		
	}

	void Brush::SetAttributes(Ptr<Terrain::BrushAttributes> attributes)
	{
		this->attributes = attributes;
	}

} // namespace Terrain