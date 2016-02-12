//------------------------------------------------------------------------------
//  terrainaddon.cc
//  (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core\rttimacros.h"
#include "brushattributes.h"
#include "resources\resourcemanager.h"

namespace Terrain
{
	__ImplementClass(Terrain::BrushAttributes, 'TBAT', Core::RefCounted);

	//------------------------------------------------------------------------------
	/**
	*/
	BrushAttributes::BrushAttributes() : radius(10.f), innerRadius(1.f), strength(1.f)
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
		CoreGraphics::Texture::MapInfo mapInfo;
		texture->GetTexture()->Map(0, Base::ResourceBase::MapRead, mapInfo);
		std::memcpy(brushTextureBuffer, mapInfo.data, mapInfo.mipWidth*mapInfo.mipHeight*sizeof(float));
		texture->GetTexture()->Unmap(0);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		BrushAttributes::Discard()
	{
		Resources::ResourceManager::Instance()->DiscardManagedResource(this->texture.upcast<Resources::ManagedResource>());
	}

} // namespace Terrain