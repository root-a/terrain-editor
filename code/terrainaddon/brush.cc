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

		for (int i = 0; i < (int)textureSize.x()*(int)textureSize.y(); i++)
		{
			textureBuffer[i] = attributes->strength; //
		}
	}

	void Brush::SetAttributes(Ptr<Terrain::BrushAttributes> attributes)
	{
		this->attributes = attributes;
	}

} // namespace Terrain