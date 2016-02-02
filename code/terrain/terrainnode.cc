//------------------------------------------------------------------------------
//  billboardnode.cc
//  (C) 2013-2015 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "terrainnode.h"
#include "terrainnodeinstance.h"

namespace Terrain
{
	__ImplementClass(Terrain::TerrainNode, 'TRNO', Models::StateNode);

	//------------------------------------------------------------------------------
	/**
	*/
	TerrainNode::TerrainNode()
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	TerrainNode::~TerrainNode()
	{
		// empty
	}

	//------------------------------------------------------------------------------
	/**
	*/
	Ptr<Models::ModelNodeInstance>
		TerrainNode::CreateNodeInstance() const
	{
		Ptr<Models::ModelNodeInstance> newInst = (Models::ModelNodeInstance*) TerrainNodeInstance::Create();
		return newInst;
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		TerrainNode::SetSurfaceName(const Util::String& name)
	{
		this->materialName = name;
	}

} // namespace Models