// 材质表面
struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Reflect;
};

// 方向光
struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float Pad;
};

// 点光
struct PointLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Att;
	float Pad;
};

// 聚光
struct SpotLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;

	float3 Position;
	float Range;

	float3 Direction;
	float Spot;

	float3 Att;
	float Pad;
};

void ComputeDirectionalLight(Material mat,
	DirectionalLight light,
	float3 normal,
	float3 toEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec)
{
	// 初始化
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// 光向量与照射的方向相反
	float3 lightVec = -light.Direction;

	// 添加环境光
	ambient = mat.Ambient * light.Ambient;

	// 漫反射系数
	float diffuseFactor = dot(lightVec, normal);

	// 展开，避免动态分支
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		// 光反射后的单位方向向量
		float3 reflectVec = reflect(-lightVec, normal);
		// 镜面反射系数
		float specFactor = pow(max(dot(reflectVec, toEye), 0.0f), mat.Specular.w);
		// 计算环境光
		diffuse = diffuseFactor * mat.Diffuse * light.Diffuse;
		// 计算镜面反射光
		spec = specFactor * mat.Specular * light.Specular;
	}
}

void ComputePointLight(Material mat,
	PointLight light,
	float3 pos,
	float3 normal,
	float3 toEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec)
{
	// 初始化
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// 从表面到光源的方向
	float3 lightVec = light.Position - pos;

	// 表面到光线的距离
	float dis = length(lightVec);

	if (dis > light.Range)
		return;

	// 标准化光向量
	lightVec /= dis;

	// 计算环境光
	ambient = mat.Ambient * light.Ambient;

	// 计算漫反射系数
	float diffuseFactor = dot(lightVec, normal);

	// 展开，避免动态分支
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		// 反射后的光的方向单位向量
		float3 reflectVec = reflect(-lightVec, normal);
		// 计算镜面反射系数
		float specFactor = pow(max(dot(reflectVec, toEye), 0.0f), mat.Specular.w);
		// 计算漫反射光
		diffuse = diffuseFactor * mat.Diffuse * light.Diffuse;
		// 计算镜面反射光
		spec = specFactor * mat.Specular * light.Specular;
	}
	// 光的衰减
	float att = 1.0f / dot(light.Att, float3(1.0f, dis, dis*dis));
	diffuse *= att;
	spec *= att;
}

void ComputeSpotLight(Material mat,
	SpotLight light,
	float3 pos,
	float3 normal,
	float3 toEye,
	out float4 ambient,
	out float4 diffuse,
	out float4 spec)
{
	// 初始化
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// 表面到光源的方向向量
	float3 lightVec = light.Position - pos;

	// 表面到光源的距离
	float dis = length(lightVec);

	// 范围检测
	if (dis > light.Range)
		return;

	// 标准化光向量
	lightVec /= dis;

	// 计算环境光部分
	ambient = mat.Ambient * light.Ambient;

	// 计算漫反射系数
	float diffuseFactor = dot(lightVec, normal);

	// 展开，避免动态分支
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 reflectVec = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(reflectVec, toEye), 0.0f), mat.Specular.w);
		diffuse = diffuseFactor * mat.Diffuse * light.Diffuse;
		spec = specFactor * mat.Specular * light.Specular;
	}

	// 计算汇聚因子和衰减系数
	float spot = pow(max(dot(-lightVec, light.Direction), 0.0f), light.Spot);
	float att = spot / dot(light.Att, float3(1.0f, dis, dis * dis));

	ambient *= spot;
	diffuse *= att;
	spec *= att;
}
