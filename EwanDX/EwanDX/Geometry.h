// ���������ļ�����ģ��

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
		std::vector<VertexType> vertexVec; // �������� 
		std::vector<IndexType> indexVec;   // ��������

		MeshData()
		{
			// ������������ͺϷ���
			static_assert(sizeof(IndexType) == 2 || sizeof(IndexType) == 4, "The size of IndexType must be 2 bytes or 4 bytes! ");
			static_assert(std::is_unsigned<IndexType>::value, "IndexType must be unsigned integer!");
		}
	};

	// ����������������,levels & slices ���ƾ���	 
	template<class VertexType = VertexPosNormalTex,class IndexType = WORD>
	MeshData<VertexType, IndexType> CreateSphere(float radius = 1.0f, UINT levels = 20, UINT slices = 20,
		const DirectX::XMFLOAT4& color = { 1.0f,1.0f,1.0f,1.0f });

	// ������������������
	template<class VertexType = VertexPosNormalTex,class IndexType = WORD>
	MeshData<VertexType, IndexType> CreateBox(float width = 2.0f, float height = 2.0f, float depth = 2.0f,
		const DirectX::XMFLOAT4& color = { 1.0f,1.0f,1.0f,1.0f });

	// ����Բ������������,slices ���ƾ���
	template<class VertexType = VertexPosNormalTex,class IndexType = WORD>
	MeshData<VertexType, IndexType>CreateCylinder(float radius = 1.0f,float height = 2.0f, UINT slices = 20,
		const DirectX::XMFLOAT4& color = { 1.0f,1.0f,1.0f,1.0f });

	// ����ֻ��Բ���������������ݣ�slices ���ƾ���
	template<class VertexType = VertexPosNormalTex,class IndexType = WORD>
	MeshData<VertexType, IndexType>CreateCylinerWithoutCap(float radius = 1.0f, float height = 2.0f, UINT slices = 20,
		const DirectX::XMFLOAT4& color = { 1.0f,1.0f,1.0f,1.0f });

	// ����ֻ��Բ׶������������ݣ�slices ���ƾ���
	template<class VertexType = VertexPosNormalTex,class IndexType = WORD>
	MeshData<VertexType, IndexType>CreateConeWithoutCap(float radius = 1.0f, float height = 2.0f, UINT slices = 20,
		const DirectX::XMFLOAT4& color = { 1.0f,1.0f,1.0f,1.0f });
}

#endif // !GEOMETRY_H
