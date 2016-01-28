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
vsMainPrimitives(in vec4 position, in vec3 normal, in vec2 uv, [slot=5] in vec4 color, out vec2 UV, out vec4 Color)  
{
	float height = texture(HeightMap, uv.xy).r;
	vec4 offset = vec4(position.x, position.y + height * HeightMultiplier, position.z, 1.0f);
	gl_Position = ViewProjection * Model * position;
	Color = color;
	UV = uv;
}
	
//------------------------------------------------------------------------------
/**
*/
shader
void
psMainPrimitives(in vec2 uv, in vec4 color, [color0] out vec4 Color) 
{
	Color = color;
}




//------------------------------------------------------------------------------
/**
*/
shader
void
vsMainShape(in vec4 position, in vec3 normal, in vec2 uv, out vec2 UV)  
{
	float height = texture(HeightMap, uv.xy).r;
	vec4 offset = vec4(position.x, position.y + height * HeightMultiplier, position.z, 1.0f);
	gl_Position = ViewProjection * Model * offset;
	UV = uv;
}
	
//------------------------------------------------------------------------------
/**
*/
shader
void
psMainShape(in vec2 uv, [color0] out vec4 Color) 
{
	Color = MatDiffuse;
}




//------------------------------------------------------------------------------
/**
*/
SimpleTechnique(DefaultPrim, "Colored", vsMainPrimitives(), psMainPrimitives(), DepthDisabledState);
SimpleTechnique(DepthPrim, "Colored|Alt0", vsMainPrimitives(), psMainPrimitives(), DepthEnabledState);
SimpleTechnique(WireframePrim, "Colored|Alt1", vsMainPrimitives(), psMainPrimitives(), WireframeState);

SimpleTechnique(DefaultShape, "MyStatic", vsMainShape(), psMainShape(), DepthDisabledState);
SimpleTechnique(DepthShape, "MyStatic|Alt0", vsMainShape(), psMainShape(), DepthEnabledState);
SimpleTechnique(WireframeShape, "MyStatic|Alt1", vsMainShape(), psMainShape(), WireframeState);