//------------------------------------------------------------------------------
//  terrainaddon.cc
//  (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core\rttimacros.h"
#include "brush.h"
#include "resources\resourcemanager.h"

namespace Terrain
{
	__ImplementClass(Terrain::Brush, 'TBAD', Core::RefCounted);

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
		Brush::Setup(Resources::ResourceId resourceID)
	{
		this->resourceID = resourceID;
		this->radius = 10.f;
		this->innerRadius = 5.f;
		this->texture = Resources::ResourceManager::Instance()->CreateManagedResource(CoreGraphics::Texture::RTTI, this->resourceID, NULL, true).downcast<Resources::ManagedTexture>();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		Brush::Discard()
	{
		Resources::ResourceManager::Instance()->DiscardManagedResource(this->texture.upcast<Resources::ManagedResource>());
	}

} // namespace Terrain