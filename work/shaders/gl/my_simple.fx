//------------------------------------------------------------------------------
//  simple.fx
//  (C) 2012 Gustav Sterbrant
//------------------------------------------------------------------------------

#include "lib/std.fxh"
#include "lib/shared.fxh"
#include "lib/util.fxh"
#include "lib/techniques.fxh"

mat4 ShapeModel;
vec4 MatDiffuse;
vec4 MatAnotherColor;
float HeightMultiplier;
sampler2D HeightMap;

state WireframeState
{
	CullMode = None;	
	BlendEnabled[0] = true;	
	SrcBlend[0] = SrcAlpha;
	DstBlend[0] = OneMinusSrcAlpha;
	FillMode = Line;
	//MultisampleEnabled = true;
};

state DepthEnabledState
{
	CullMode = None;	
	BlendEnabled[0] = true;
	SrcBlend[0] = SrcAlpha;	
	DstBlend[0] = OneMinusSrcAlpha;
	DepthEnabled = true;
	DepthWrite = true;
	MultisampleEnabled = true;
};

state DepthDisabledState
{
	CullMode = None;	
	BlendEnabled[0] = true;
	SrcBlend[0] = SrcAlpha;
	DstBlend[0] = OneMinusSrcAlpha;
	DepthEnabled = false;
	DepthWrite = false;
	//MultisampleEnabled = true;
};


//------------------------------------------------------------------------------
/**
*/
shader
void
vsMainShape(in vec4 position, in vec3 normal, in vec2 uv, out vec2 UV, out float Height)  
{
	float height = texture(HeightMap, uv).r;
	vec4 offset = vec4(position.x, position.y + height * HeightMultiplier, position.z, 1.0f);
	gl_Position = ViewProjection * ShapeModel * offset;
	UV = uv;
	Height = height;
}
	
//------------------------------------------------------------------------------
/**
*/
shader
void
psMainShape(in vec2 uv, in float Height, [color0] out vec4 Color) 
{
	Color = vec4(uv*Height,0,1);
}

SimpleTechnique(DepthShape, "MyStatic", vsMainShape(), psMainShape(), DepthEnabledState);
SimpleTechnique(WireframeShape, "MyStatic|Alt0", vsMainShape(), psMainShape(), WireframeState);