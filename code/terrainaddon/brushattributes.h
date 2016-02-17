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
#include <IL/il.h>
#include <IL/ilu.h>
//#include "IL/ilu_Scale.h"
#include "coregraphics/ogl4/ogl4types.h"
#include "coregraphics/base/texturebase.h"

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

		void ConvertTexture(const Ptr<CoreGraphics::Texture>& tex, ILenum imageFileType);
		void ResampleTexture();
		float radius;
		float innerRadius;
		float strength;
		int height;
		int width;
		unsigned char* brushTextureBuffer;
		unsigned char* sampledTextureBuffer;
		Ptr<Resources::ManagedTexture> texture;
	private:
		
	};
} // namespace Grid