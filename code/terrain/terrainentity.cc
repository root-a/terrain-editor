//------------------------------------------------------------------------------
//  billboardentity.cc
//  (C) 2013-2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "terrainentity.h"
#include "resources/resourcemanager.h"
#include "coregraphics/shaderserver.h"
#include "billboards/billboardnodeinstance.h"
#include "models/nodes/statenodeinstance.h"
#include "models/visresolver.h"

using namespace Resources;
using namespace CoreGraphics;
using namespace Models;
using namespace Materials;
using namespace Models;
using namespace Graphics;
using namespace Messaging;
using namespace Math;
using namespace Billboards;

namespace Graphics
{
	__ImplementClass(Graphics::TerrainEntity, 'BLEN', Graphics::GraphicsEntity);

	Ptr<Models::Model> TerrainEntity::terrainModel = 0;
	Ptr<Terrain::TerrainNode> TerrainEntity::terrainNode = 0;

	//------------------------------------------------------------------------------
	/**
	*/
	TerrainEntity::TerrainEntity() :
		texture(0),
		color(1, 1, 1, 1),
		modelInstance(0),
		viewAligned(false)
	{
		this->SetType(GraphicsEntityType::Model);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	TerrainEntity::~TerrainEntity()
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		TerrainEntity::OnActivate()
	{
		n_assert(!this->IsActive());
		n_assert(this->resource.IsValid());
		n_assert(!this->texture.isvalid());
		n_assert(!this->modelInstance.isvalid());
		n_assert(!this->textureVariable.isvalid());
		GraphicsEntity::OnActivate();

		ResourceManager* resManager = ResourceManager::Instance();
		ShaderServer* shdServer = ShaderServer::Instance();

		// create texture
		this->texture = resManager->CreateManagedResource(Texture::RTTI, this->resource, NULL, true).downcast<ManagedTexture>();

		// setup base model if needed
		if (!TerrainEntity::terrainModel.isvalid())
		{
			TerrainEntity::terrainModel = Model::Create();
			TerrainEntity::terrainNode = Terrain::TerrainNode::Create();
			TerrainEntity::terrainNode->SetBoundingBox(Math::bbox(Math::point(0, 0, 0), Math::vector(1, 1, 1)));
			TerrainEntity::terrainNode->SetSurfaceName("sur:system/billboard");
			TerrainEntity::terrainNode->SetName("root");
			TerrainEntity::terrainNode->LoadResources(true);
			TerrainEntity::terrainModel->AttachNode(TerrainEntity::terrainNode.upcast<ModelNode>());
		}

		// create model instance
		this->modelInstance = TerrainEntity::terrainModel->CreateInstance();
		this->modelInstance->SetTransform(this->transform);
		this->modelInstance->SetPickingId(this->pickingId);

		// get node instance and set the view space aligned flag
		Ptr<BillboardNodeInstance> nodeInstance = this->modelInstance->GetRootNodeInstance().downcast<BillboardNodeInstance>();

		// setup material
		const Ptr<SurfaceInstance>& surface = nodeInstance->GetSurfaceInstance();
		this->textureVariable = surface->GetConstant("AlbedoMap");
		this->colorVariable = surface->GetConstant("Color");

		// create a variable instance and set the texture
		this->textureVariable->SetTexture(this->texture->GetTexture());
		nodeInstance->SetInViewSpace(this->viewAligned);

		// set to be valid
		this->SetValid(true);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		TerrainEntity::OnDeactivate()
	{
		n_assert(this->IsActive());
		n_assert(this->texture.isvalid());
		n_assert(this->modelInstance.isvalid());
		n_assert(this->textureVariable.isvalid());

		// cleanup resources
		ResourceManager* resManager = ResourceManager::Instance();
		resManager->DiscardManagedResource(this->texture.upcast<ManagedResource>());

		// discard model instance
		this->modelInstance->GetModel()->DiscardInstance(this->modelInstance);
		this->modelInstance = 0;

		// discard texture variable
		this->textureVariable = 0;
		this->colorVariable = 0;

		// kill model if this is our last billboard entity
		if (TerrainEntity::terrainModel->GetInstances().Size() == 0)
		{
			TerrainEntity::terrainNode->UnloadResources();
			TerrainEntity::terrainNode = 0;
			TerrainEntity::terrainModel->Unload();
			TerrainEntity::terrainModel = 0;
		}

		// up to parent class
		GraphicsEntity::OnDeactivate();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		TerrainEntity::OnHide()
	{
		if (this->modelInstance.isvalid())
		{
			this->modelInstance->OnHide(this->entityTime);
		}
		GraphicsEntity::OnHide();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		TerrainEntity::OnShow()
	{
		if (this->modelInstance.isvalid())
		{
			this->modelInstance->OnShow(this->entityTime);
		}
		GraphicsEntity::OnShow();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		TerrainEntity::OnResolveVisibility(IndexT frameIndex, bool updateLod)
	{
		n_assert(this->modelInstance.isvalid());
		VisResolver::Instance()->AttachVisibleModelInstancePlayerCamera(frameIndex, this->modelInstance, updateLod);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		TerrainEntity::OnTransformChanged()
	{
		// set transform of model instance
		if (this->modelInstance.isvalid())
		{
			this->modelInstance->SetTransform(this->transform);
		}

		GraphicsEntity::OnTransformChanged();
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		TerrainEntity::OnNotifyCullingVisible(const Ptr<GraphicsEntity>& observer, IndexT frameIndex)
	{
		if (this->IsVisible())
		{
			// call back our model-instance
			if (this->modelInstance.isvalid())
			{
				this->modelInstance->OnNotifyCullingVisible(frameIndex, this->entityTime);
			}
		}

		// call parent-class
		GraphicsEntity::OnNotifyCullingVisible(observer, frameIndex);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		TerrainEntity::SetColor(const Math::float4& color)
	{
		this->color = color;

		// update billboard color
		scalar biggest = n_max(n_max(this->color.x(), this->color.y()), this->color.z());
		float4 normalizedColor = this->color;
		normalizedColor /= biggest;
		normalizedColor.set_w(1);
		this->colorVariable->SetValue(normalizedColor);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		TerrainEntity::OnRenderBefore(IndexT frameIndex)
	{
		if (this->renderBeforeFrameIndex != frameIndex)
		{
			// if our model instance is valid, let it update itself
			if (this->modelInstance.isvalid())
			{
				this->modelInstance->OnRenderBefore(frameIndex, this->entityTime);
			}

			GraphicsEntity::OnRenderBefore(frameIndex);
		}
	}

	//------------------------------------------------------------------------------
	/**
	Handle a message, override this method accordingly in subclasses!
	*/
	void
		TerrainEntity::HandleMessage(const Ptr<Message>& msg)
	{
		__Dispatch(TerrainEntity, this, msg);
	}

} // namespace Graphics