#pragma once

#include <Insight/Core.h>

#include "Insight/Math/Transform.h"

#include "Platform/DirectX_Shared/Constant_Buffer_Types.h"

#include "Insight/Rendering/Geometry/Vertex_Buffer.h"
#include "Insight/Rendering/Geometry/Index_Buffer.h"

namespace Insight {


	class INSIGHT_API Mesh
	{
	public:
		Mesh(const Verticies& Verticies, const Indices& Indices);
		Mesh(Mesh&& mesh) noexcept;
		~Mesh();

		void PreRender(const ieMatrix4x4& parentMat);
		void Render();
		void Destroy();
		void OnImGuiRender();

		inline ieTransform& GetTransformRef() { return m_Transform; }
		inline const ieTransform& GetTransform() const { return m_Transform; }
		inline CB_VS_PerObject GetConstantBuffer() { return m_ConstantBufferPerObject; }

		uint32_t GetVertexCount();
		uint32_t GetVertexBufferSize();

		uint32_t GetIndexCount();
		uint32_t GetIndexBufferSize();

	private:
		void Init(const Verticies& verticies, const Indices& indices);
		void CreateBuffers(const Verticies& Verticies, const Indices& Indices);
		void UpdateAccelerationStructures();
	private:
		ieVertexBuffer* m_pVertexBuffer;
		ieIndexBuffer* m_pIndexBuffer;

		ieTransform		m_Transform;
		CB_VS_PerObject	m_ConstantBufferPerObject = {};

		bool			m_CastsShadows = true;
		uint32_t		m_RTInstanceIndex = 0U;
		bool			m_ShouldUpdateAS = false;
	};
}
