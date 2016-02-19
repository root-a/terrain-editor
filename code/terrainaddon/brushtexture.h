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
#include "coregraphics/ogl4/ogl4types.h"
#include "coregraphics/base/texturebase.h"

namespace Terrain
{
	class BrushTexture : public Core::RefCounted
	{
		__DeclareClass(BrushTexture);
	public:
		/// constructor
		BrushTexture();
		/// destructor
		virtual ~BrushTexture();

		/// setup
		void Setup(Resources::ResourceId resourceID);
		/// discard
		void Discard();
		
		unsigned char* sampledBrushBuffer;
		void ResampleTexture(const int newSize);
		int size;
	private:
		
		void ConvertTexture(const Ptr<CoreGraphics::Texture>& tex, ILenum imageFileType);
		unsigned char* brushTextureBuffer;
		Ptr<Resources::ManagedTexture> texture;
		
		int orgSize;
	};
} // namespace Grid