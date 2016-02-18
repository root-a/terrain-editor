//------------------------------------------------------------------------------
//  terrainaddon.cc
//  (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core\rttimacros.h"
#include "brushattributes.h"
#include "resources\resourcemanager.h"
#include "coregraphics\pixelformat.h"


namespace Terrain
{
	__ImplementClass(Terrain::BrushAttributes, 'TBAT', Core::RefCounted);

	//------------------------------------------------------------------------------
	/**
	*/
	BrushAttributes::BrushAttributes() : radius(32), strength(0.1f), sampledBrushBuffer(nullptr), brushTextureBuffer(nullptr)
	{
		
	}

	//------------------------------------------------------------------------------
	/**
	*/
	BrushAttributes::~BrushAttributes()
	{
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		BrushAttributes::Setup(Resources::ResourceId resourceID)
	{
		texture = Resources::ResourceManager::Instance()->CreateManagedResource(CoreGraphics::Texture::RTTI, resourceID, NULL, true).downcast<Resources::ManagedTexture>();
		ILenum imageFormat;
		imageFormat = IL_DDS;
		ConvertTexture(texture->GetTexture(), imageFormat);
		ResampleTexture();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		BrushAttributes::Discard()
	{
		Resources::ResourceManager::Instance()->DiscardManagedResource(this->texture.upcast<Resources::ManagedResource>());
		Memory::Free(Memory::DefaultHeap, this->brushTextureBuffer);
		Memory::Free(Memory::DefaultHeap, this->sampledBrushBuffer);
		this->brushTextureBuffer = 0;
		this->sampledBrushBuffer = 0;
	}

	void BrushAttributes::ConvertTexture(const Ptr<CoreGraphics::Texture>& tex, ILenum imageFileType)
	{
		n_assert(tex->GetType() == CoreGraphics::Texture::Texture2D);
		bool retval = false;

		SizeT maxLevels = tex->GetNumMipLevels();
		SizeT mipLevelToSave = 0;//this->mipLevel;
		if (mipLevelToSave >= maxLevels)
		{
			mipLevelToSave = maxLevels - 1;
		}

		// create il image
		ILint image = ilGenImage();
		ilBindImage(image);

		// convert our pixel formats to IL components
		ILuint channels;
		ILuint format;
		ILuint type;
		channels = CoreGraphics::PixelFormat::ToChannels(tex->GetPixelFormat());
		format = CoreGraphics::PixelFormat::ToILComponents(tex->GetPixelFormat());
		type = CoreGraphics::PixelFormat::ToILType(tex->GetPixelFormat());

		CoreGraphics::Texture::MapInfo mapInfo;
		tex->Map(mipLevelToSave, Base::ResourceBase::MapRead, mapInfo);
		
		orgSize = mapInfo.mipWidth;

		// create image
		ILboolean result = ilTexImage(mapInfo.mipWidth, mapInfo.mipHeight, 1, channels, format, type, (ILubyte*)mapInfo.data);
		n_assert(result == IL_TRUE);

		// flip image if it's a GL texture
		if (!tex->IsRenderTargetAttachment())
		{
			iluFlipImage();
		}

		// now save as PNG (will support proper alpha)
		ilConvertImage(IL_RGBA, IL_UNSIGNED_BYTE);
		ILubyte* uncompressedData = ilGetData();

		int framesize = mapInfo.mipWidth*mapInfo.mipHeight;
		brushTextureBuffer = (unsigned char*)Memory::Alloc(Memory::DefaultHeap, framesize);
				
		int j = 0;
		for (int i = 0; i < framesize; i++)
		{
			brushTextureBuffer[i] = (unsigned char)uncompressedData[j];
			j += 4;
		}

		tex->Unmap(mipLevelToSave);


		ilDeleteImage(image);
	}

	void BrushAttributes::ResampleTexture()
	{
		//create il image
		ILint image = ilGenImage();
		ilBindImage(image);
		// create image
		ILboolean result = ilTexImage(orgSize, orgSize, 1, 1, IL_RED, IL_UNSIGNED_BYTE, (ILubyte*)this->brushTextureBuffer);
		n_assert(result == IL_TRUE);

		size = radius * 2;
		iluScale(size, size, 0);
		ILubyte* uncompressedData = ilGetData();
		if (this->sampledBrushBuffer != nullptr) Memory::Free(Memory::DefaultHeap, this->sampledBrushBuffer);		
		int framesize = size*size;
		this->sampledBrushBuffer = (unsigned char*)Memory::Alloc(Memory::DefaultHeap, framesize);
		memcpy(this->sampledBrushBuffer, uncompressedData, framesize);

		ilDeleteImage(image);
	}

	void BrushAttributes::SetRadius(int newRadius)
	{
		radius = newRadius;
		ResampleTexture();
	}

	int BrushAttributes::GetRadius()
	{
		return radius;
	}

} // namespace Terrain