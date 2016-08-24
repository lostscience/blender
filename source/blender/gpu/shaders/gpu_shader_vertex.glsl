#ifdef USE_OPENSUBDIV
in vec3 normal;
in vec4 position;

out block {
	VertexData v;
} outpt;
#endif

#ifdef USE_INSTANCING
in mat3 ininstmatrix;
in vec3 ininstposition;
in vec4 ininstcolor;

varying vec4 varinstcolor;
varying mat4 varinstmat;
varying mat4 varinstinvmat;
varying mat4 varinstlocaltoviewmat;
varying mat4 varinstinvlocaltoviewmat;

uniform mat4 unfviewmat;

#  if __VERSION__ < 140
/* Manual implementation of inverse(mat4) for GLSL version before 1.40, 
 * copied from https://github.com/stackgl/glsl-inverse/blob/master/index.glsl
 */

mat4 inverse(mat4 m) {
	float a00 = m[0][0], a01 = m[0][1], a02 = m[0][2], a03 = m[0][3],
	a10 = m[1][0], a11 = m[1][1], a12 = m[1][2], a13 = m[1][3],
	a20 = m[2][0], a21 = m[2][1], a22 = m[2][2], a23 = m[2][3],
	a30 = m[3][0], a31 = m[3][1], a32 = m[3][2], a33 = m[3][3],

	b00 = a00 * a11 - a01 * a10,
	b01 = a00 * a12 - a02 * a10,
	b02 = a00 * a13 - a03 * a10,
	b03 = a01 * a12 - a02 * a11,
	b04 = a01 * a13 - a03 * a11,
	b05 = a02 * a13 - a03 * a12,
	b06 = a20 * a31 - a21 * a30,
	b07 = a20 * a32 - a22 * a30,
	b08 = a20 * a33 - a23 * a30,
	b09 = a21 * a32 - a22 * a31,
	b10 = a21 * a33 - a23 * a31,
	b11 = a22 * a33 - a23 * a32,

	det = b00 * b11 - b01 * b10 + b02 * b09 + b03 * b08 - b04 * b07 + b05 * b06;

	return mat4(
		a11 * b11 - a12 * b10 + a13 * b09,
		a02 * b10 - a01 * b11 - a03 * b09,
		a31 * b05 - a32 * b04 + a33 * b03,
		a22 * b04 - a21 * b05 - a23 * b03,
		a12 * b08 - a10 * b11 - a13 * b07,
		a00 * b11 - a02 * b08 + a03 * b07,
		a32 * b02 - a30 * b05 - a33 * b01,
		a20 * b05 - a22 * b02 + a23 * b01,
		a10 * b10 - a11 * b08 + a13 * b06,
		a01 * b08 - a00 * b10 - a03 * b06,
		a30 * b04 - a31 * b02 + a33 * b00,
		a21 * b02 - a20 * b04 - a23 * b00,
		a11 * b07 - a10 * b09 - a12 * b06,
		a00 * b09 - a01 * b07 + a02 * b06,
		a31 * b01 - a30 * b03 - a32 * b00,
		a20 * b03 - a21 * b01 + a22 * b00) / det;
}
#  endif

#endif

varying vec3 varposition;
varying vec3 varnormal;

#ifdef CLIP_WORKAROUND
varying float gl_ClipDistance[6];
#endif


/* Color, keep in sync with: gpu_shader_vertex_world.glsl */

float srgb_to_linearrgb(float c)
{
	if (c < 0.04045)
		return (c < 0.0) ? 0.0 : c * (1.0 / 12.92);
	else
		return pow((c + 0.055) * (1.0 / 1.055), 2.4);
}

void srgb_to_linearrgb(vec3 col_from, out vec3 col_to)
{
	col_to.r = srgb_to_linearrgb(col_from.r);
	col_to.g = srgb_to_linearrgb(col_from.g);
	col_to.b = srgb_to_linearrgb(col_from.b);
}

void srgb_to_linearrgb(vec4 col_from, out vec4 col_to)
{
	col_to.r = srgb_to_linearrgb(col_from.r);
	col_to.g = srgb_to_linearrgb(col_from.g);
	col_to.b = srgb_to_linearrgb(col_from.b);
	col_to.a = col_from.a;
}

bool is_srgb(int info)
{
#ifdef USE_NEW_SHADING
	return (info == 1)? true: false;
#else
	return false;
#endif
}

void set_var_from_attr(float attr, int info, out float var)
{
	var = attr;
}

void set_var_from_attr(vec2 attr, int info, out vec2 var)
{
	var = attr;
}

void set_var_from_attr(vec3 attr, int info, out vec3 var)
{
	if (is_srgb(info)) {
		srgb_to_linearrgb(attr, var);
	}
	else {
		var = attr;
	}
}

void set_var_from_attr(vec4 attr, int info, out vec4 var)
{
	if (is_srgb(info)) {
		srgb_to_linearrgb(attr, var);
	}
	else {
		var = attr;
	}
}

/* end color code */


void main()
{
#ifndef USE_OPENSUBDIV
	vec4 position = gl_Vertex;
	vec3 normal = gl_Normal;
#endif

#ifdef USE_INSTANCING
	mat4 instmat = mat4(vec4(ininstmatrix[0], ininstposition.x),
						vec4(ininstmatrix[1], ininstposition.y),
						vec4(ininstmatrix[2], ininstposition.z),
						vec4(0.0, 0.0, 0.0, 1.0));

	varinstmat = transpose(instmat);
	varinstlocaltoviewmat = unfviewmat * varinstmat;
	varinstinvmat = inverse(varinstmat);
	varinstinvlocaltoviewmat = inverse(varinstlocaltoviewmat);
	varinstcolor = ininstcolor;

	position *= instmat;
	normal *= ininstmatrix;
#endif

	vec4 co = gl_ModelViewMatrix * position;

	varposition = co.xyz;
	varnormal = normalize(gl_NormalMatrix * normal);
	gl_Position = gl_ProjectionMatrix * co;

#ifdef CLIP_WORKAROUND
	int i;
	for (i = 0; i < 6; i++)
		gl_ClipDistance[i] = dot(co, gl_ClipPlane[i]);
#elif !defined(GPU_ATI)
	// Setting gl_ClipVertex is necessary to get glClipPlane working on NVIDIA
	// graphic cards, while on ATI it can cause a software fallback.
	gl_ClipVertex = co; 
#endif 

#ifdef USE_OPENSUBDIV
	outpt.v.position = co;
	outpt.v.normal = varnormal;
#endif
