struct DirectionalLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 direction;
	float pad;
};

struct PointLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 position;
	float range;
	float3 attenuation;
	float pad;
};

struct SpotLight
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float3 position;
	float range;
	float3 direction;
	float spot;
	float3 attenuation;
	float pad;
};

struct LightMaterial
{
	float4 ambient;
	float4 diffuse;
	float4 specular;
	float4 reflect;
};


void ComputeDirectionalLight(LightMaterial mat, DirectionalLight L, float3 normal, float3 toEye,
	 out float4 ambient, out float4 diffuse, out float4 spec)
{
	ambient = float4(0.0, 0.0, 0.0, 0.0);
	diffuse = float4(0.0, 0.0, 0.0, 0.0);
	spec = float4(0.0, 0.0, 0.0, 0.0);

	float3 lightVec = -L.direction;
	ambient = mat.ambient * L.ambient;
	float diffuseFactor = dot(lightVec, normal);
	if (diffuseFactor > 0)
	{
		float3 v = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(v, toEye), 0.0), mat.specular.w);
		diffuse = diffuseFactor * mat.diffuse * L.diffuse;
		spec = specFactor * mat.specular * L.specular;
	}
}

void ComputePointLight(LightMaterial mat, PointLight L, float3 pos, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	ambient = float4(0, 0, 0, 0);
	diffuse = float4(0, 0, 0, 0);
	spec = float4(0, 0, 0, 0);

	float3 lightVec = L.position - pos;
		float d = length(lightVec);
	if (d > L.range)
		return;

	lightVec /= d;
	ambient = mat.ambient * L.ambient;	
	float diffuseFactor = dot(lightVec, normal);
	if (diffuseFactor > 0.0)
	{
		float3 v = reflect(-lightVec, normal);
			float specFactor = pow(max(dot(v, toEye), 0.0), mat.specular.w);
		diffuse = diffuseFactor * mat.diffuse * L.diffuse;
		spec = specFactor * mat.specular * L.specular;
	}

	float att = 1.0 / dot(L.attenuation, float3(1.0, d, d*d));
	diffuse *= att;
	spec *= att;
}


void ComputeSpotLight(LightMaterial mat, SpotLight L, float3 pos, float3 normal, float3 toEye,
	out float4 ambient, out float4 diffuse, out float4 spec)
{
	ambient = float4(0, 0, 0, 0);
	diffuse = float4(0, 0, 0, 0);
	spec = float4(0, 0, 0, 0);
	float3 lightVec = L.position - pos;
		float d = length(lightVec);
	if (d > L.range)
		return;
	lightVec /= d;
	ambient = mat.ambient * L.ambient;
	float diffuseFactor = dot(lightVec, normal);
	if (diffuseFactor > 0.0)
	{
		float3 v = reflect(-lightVec, normal);
			float specFactor = pow(max(dot(v, toEye), 0.0), mat.specular.w);
		diffuse = diffuseFactor * mat.diffuse * L.diffuse;
		spec = specFactor * mat.specular * L.specular;
	}

	float spot = pow(max(dot(-lightVec, L.direction), 0.0), L.spot);
	float att = spot / dot(L.attenuation, float3(1.0, d, d*d));
	ambient *= spot;
	diffuse *= att;
	spec *= att;
}
