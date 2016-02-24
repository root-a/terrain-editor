//------------------------------------------------------------------------------
//  terrainaddon.cc
//  (C) 2016 Individual contributors, see AUTHORS file
//------------------------------------------------------------------------------
#include "stdneb.h"
#include "core\rttimacros.h"
#include "resources\resourcemanager.h"
#include "coregraphics\pixelformat.h"
#include "brushtool.h"


namespace Terrain
{
	__ImplementClass(Terrain::BrushTool, 'TBTL', Core::RefCounted);

	//------------------------------------------------------------------------------
	/**
	*/
	BrushTool::BrushTool() : radius(32), strength(0.01f), maxHeight(1024.f)
	{
		
	}

	//------------------------------------------------------------------------------
	/**
	*/
	BrushTool::~BrushTool()
	{
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		BrushTool::Setup()
	{
		LoadBrushTextures();
		brushSmooth = Terrain::BrushSmooth::Create();
		brushDefaultAddRemove = Terrain::BrushFunction::Create();
		SetTexture(brushTextures.Front());
		SetFunction(brushDefaultAddRemove);
	}

	//------------------------------------------------------------------------------
	/**
	*/
	void
		BrushTool::Discard()
	{

	}

	void BrushTool::SetRadius(int newRadius)
	{
		radius = newRadius;
		brushTexture->ResampleTexture(radius * 2);
	}

	int BrushTool::GetRadius()
	{
		return radius;
	}

	void BrushTool::SetStrength(float newStrength)
	{
		strength = newStrength;
	}

	float BrushTool::GetStrength()
	{
		return strength;
	}

	void BrushTool::SetTexture(Ptr<Terrain::BrushTexture> newTexture)
	{
		brushTexture = newTexture;
		int properBrushSize = this->radius * 2;
		if (brushTexture->size != properBrushSize)
		{
			brushTexture->ResampleTexture(properBrushSize);
		}
	}

	void BrushTool::SetFunction(Ptr<Terrain::BrushFunction> newFunction)
	{
		function = newFunction;
	}

	void BrushTool::Paint(const Math::float4& pos, float* textureBuffer, const Math::float2& textureSize, const float modifier)
	{

		function->ExecuteBrushFunction(radius, strength, brushTexture, pos, textureBuffer, textureSize, modifier, maxHeight);
	}

	void BrushTool::ActivateSmoothBrush()
	{
		function = brushSmooth.cast<BrushFunction>();
	}

	void BrushTool::ActivateDefaultBrush()
	{
		function = brushDefaultAddRemove.cast<BrushFunction>();
	}

	void BrushTool::LoadBrushTextures()
	{
		Ptr<Terrain::BrushTexture> texture = Terrain::BrushTexture::Create();
		texture->Setup("tex:system/lightcones.dds");
		brushTextures.Append(texture);
	}

} // namespace Terrain