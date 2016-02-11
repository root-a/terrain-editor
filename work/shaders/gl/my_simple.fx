//------------------------------------------------------------------------------
//  simple.fx
//  (C) 2012 Gustav Sterbrant
//------------------------------------------------------------------------------

#include "lib/std.fxh"
#include "lib/shared.fxh"
#include "lib/util.fxh"
#include "lib/techniques.fxh"

mat4 ShapeModel;
//vec4 MatDiffuse;
//vec4 MatAnotherColor;
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
vsMainShape(in vec2 position, [slot=2] in vec2 uv, out vec2 UV, out vec3 Normal, out float Height, out vec3 ViewSpacePos)  
{
	float height = texture(HeightMap, uv).r;
	vec4 offset = vec4(position.x, height * HeightMultiplier, position.y, 1.0f);
	gl_Position = ViewProjection * Model * offset;
	UV = uv;
	Height = height;
	
	mat4 modelView = View * Model;
	ViewSpacePos = (modelView * offset).xyz;
	// # P.xy store the position for which we want to calculate the normals
	// # height() here is a function that return the height at a point in the terrain

	// read neightbor heights using an arbitrary small offset
	vec3 off = vec3(1.0, 1.0, 0.0);
	float hL = texture(HeightMap, offset.xy - off.xz).r;
	float hR = texture(HeightMap, offset.xy + off.xz).r;
	float hD = texture(HeightMap, offset.xy - off.zy).r;
	float hU = texture(HeightMap, offset.xy + off.zy).r;

	// deduce terrain normal
	Normal.x = hL - hR;
	Normal.y = hD - hU;
	Normal.z = 2.0;
	Normal = normalize(Normal);
}
	
//------------------------------------------------------------------------------
/**
*/
shader
void
psMainShape(in vec2 uv, in vec3 Normal, in float Height, in vec3 ViewSpacePos, [color0] out vec4 Color, [color1] out vec4 Normals, [color2] out float Depth) 
{
	Color = vec4(uv*Height,0,1);
	Normals = vec4(Normal,1);
	Depth = length(ViewSpacePos.xyz);
}

SimpleTechnique(DepthShape, "MyStatic", vsMainShape(), psMainShape(), DepthEnabledState);
SimpleTechnique(WireframeShape, "MyStatic|Alt0", vsMainShape(), psMainShape(), WireframeState);