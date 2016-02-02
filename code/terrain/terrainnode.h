#pragma once
//------------------------------------------------------------------------------
/**
@class Models::BillboardNode

representation of a billboard shape.
Is basically a shape node but without a mesh load from file.

(C) 2013-2015 Individual contributors, see AUTHORS file
*/
//------------------------------------------------------------------------------
#include "models/nodes/statenode.h"
#include "coregraphics/vertexbuffer.h"
#include "coregraphics/indexbuffer.h"
#include "coregraphics/primitivegroup.h"
#include "coregraphics/vertexlayout.h"
#include "coregraphics/shaderinstance.h"
namespace Terrain
{
	class TerrainNode : public Models::StateNode
	{
		__DeclareClass(TerrainNode);
	public:
		/// constructor
		TerrainNode();
		/// destructor
		virtual ~TerrainNode();

		/// create a model node instance
		virtual Ptr<Models::ModelNodeInstance> CreateNodeInstance() const;

		/// set material name
		void SetSurfaceName(const Util::String& name);

	};

} // namespace Models
//------------------------------------------------------------------------------