// ���ʱ���
struct Material
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Reflect;
};

// �����
struct DirectionalLight
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float3 Direction;
	float Pad;
};

// ���
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

// �۹�
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
	// ��ʼ��
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// ������������ķ����෴
	float3 lightVec = -light.Direction;

	// ��ӻ�����
	ambient = mat.Ambient * light.Ambient;

	// ������ϵ��
	float diffuseFactor = dot(lightVec, normal);

	// չ�������⶯̬��֧
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		// �ⷴ���ĵ�λ��������
		float3 reflectVec = reflect(-lightVec, normal);
		// ���淴��ϵ��
		float specFactor = pow(max(dot(reflectVec, toEye), 0.0f), mat.Specular.w);
		// ���㻷����
		diffuse = diffuseFactor * mat.Diffuse * light.Diffuse;
		// ���㾵�淴���
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
	// ��ʼ��
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// �ӱ��浽��Դ�ķ���
	float3 lightVec = light.Position - pos;

	// ���浽���ߵľ���
	float dis = length(lightVec);

	if (dis > light.Range)
		return;

	// ��׼��������
	lightVec /= dis;

	// ���㻷����
	ambient = mat.Ambient * light.Ambient;

	// ����������ϵ��
	float diffuseFactor = dot(lightVec, normal);

	// չ�������⶯̬��֧
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		// �����Ĺ�ķ���λ����
		float3 reflectVec = reflect(-lightVec, normal);
		// ���㾵�淴��ϵ��
		float specFactor = pow(max(dot(reflectVec, toEye), 0.0f), mat.Specular.w);
		// �����������
		diffuse = diffuseFactor * mat.Diffuse * light.Diffuse;
		// ���㾵�淴���
		spec = specFactor * mat.Specular * light.Specular;
	}
	// ���˥��
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
	// ��ʼ��
	ambient = float4(0.0f, 0.0f, 0.0f, 0.0f);
	diffuse = float4(0.0f, 0.0f, 0.0f, 0.0f);
	spec = float4(0.0f, 0.0f, 0.0f, 0.0f);

	// ���浽��Դ�ķ�������
	float3 lightVec = light.Position - pos;

	// ���浽��Դ�ľ���
	float dis = length(lightVec);

	// ��Χ���
	if (dis > light.Range)
		return;

	// ��׼��������
	lightVec /= dis;

	// ���㻷���ⲿ��
	ambient = mat.Ambient * light.Ambient;

	// ����������ϵ��
	float diffuseFactor = dot(lightVec, normal);

	// չ�������⶯̬��֧
	[flatten]
	if (diffuseFactor > 0.0f)
	{
		float3 reflectVec = reflect(-lightVec, normal);
		float specFactor = pow(max(dot(reflectVec, toEye), 0.0f), mat.Specular.w);
		diffuse = diffuseFactor * mat.Diffuse * light.Diffuse;
		spec = specFactor * mat.Specular * light.Specular;
	}

	// ���������Ӻ�˥��ϵ��
	float spot = pow(max(dot(-lightVec, light.Direction), 0.0f), light.Spot);
	float att = spot / dot(light.Att, float3(1.0f, dis, dis * dis));

	ambient *= spot;
	diffuse *= att;
	spec *= att;
}
