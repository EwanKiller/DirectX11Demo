// 生产常见的集合体模型

#ifndef GEOMETRY_H
#define GEOMETRY_H

#include <vector>
#include <string>
#include <map>
#include "Vertex.h"

namespace Geometry
{
	template<class VertexType = VertexPosNormalTex,class IndexType = WORD>
	struct MeshData
	{
		std::vector<VertexType> vertexVec; // 顶点数组 
		std::vector<IndexType> indexVec;   // 索引数组

		MeshData()
		{
			// 需检验索引类型合法性
			static_assert(sizeof(IndexType) == 2 || sizeof(IndexType) == 4, "The size of IndexType must be 2 bytes or 4 bytes! ");
			static_assert(std::is_unsigned<IndexType>::value, "IndexType must be unsigned integer!");
		}
	};

	// 创建球体网格数据,levels & slices 控制精度	 
	template<class VertexType = VertexPosNormalTex,class IndexType = WORD>
	MeshData<VertexType, IndexType> CreateSphere(float radius = 1.0f, UINT levels = 20, UINT slices = 20,
		const DirectX::XMFLOAT4& color = { 1.0f,1.0f,1.0f,1.0f });

	// 创建立方体网格数据
	template<class VertexType = VertexPosNormalTex,class IndexType = WORD>
	MeshData<VertexType, IndexType> CreateBox(float width = 2.0f, float height = 2.0f, float depth = 2.0f,
		const DirectX::XMFLOAT4& color = { 1.0f,1.0f,1.0f,1.0f });

	// 创建圆柱体网格数据,slices 控制精度
	template<class VertexType = VertexPosNormalTex,class IndexType = WORD>
	MeshData<VertexType, IndexType>CreateCylinder(float radius = 1.0f,float height = 2.0f, UINT slices = 20,
		const DirectX::XMFLOAT4& color = { 1.0f,1.0f,1.0f,1.0f });

	// 创建只有圆柱体侧面的网格数据，slices 控制精度
	template<class VertexType = VertexPosNormalTex,class IndexType = WORD>
	MeshData<VertexType, IndexType>CreateCylinerWithoutCap(float radius = 1.0f, float height = 2.0f, UINT slices = 20,
		const DirectX::XMFLOAT4& color = { 1.0f,1.0f,1.0f,1.0f });

	// 创建只有圆锥体侧面网格数据，slices 控制精度
	template<class VertexType = VertexPosNormalTex,class IndexType = WORD>
	MeshData<VertexType, IndexType>CreateConeWithoutCap(float radius = 1.0f, float height = 2.0f, UINT slices = 20,
		const DirectX::XMFLOAT4& color = { 1.0f,1.0f,1.0f,1.0f });
}

#endif // !GEOMETRY_H
