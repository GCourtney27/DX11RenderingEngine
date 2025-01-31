#include <Engine_pch.h>

#include "D3D12_Sphere_Renderer.h"

#include "Platform/DirectX_12/Direct3D12_Context.h"



namespace Insight {


	void ieD3D12SphereRenderer::Init(float radius, int slices, int segments)
	{
		m_Radius = radius;
		m_Slices = slices;
		m_Segments = segments;

		ResourceSetup();
	}

	void ieD3D12SphereRenderer::ResourceSetup()
	{
		Direct3D12Context& RenderContext = Renderer::GetAs<Direct3D12Context>();
		ID3D12Device* pDevice = &RenderContext.GetDeviceContext();

		HRESULT hr;

		std::vector< SimpleVertex > verts;
		verts.resize((m_Segments + 1) * m_Slices + 2);

		const float _pi = XM_PI;
		const float _2pi = XM_2PI;

		verts[0].position = XMFLOAT4(0, m_Radius, 0, 1);
		for (int lat = 0; lat < m_Slices; lat++)
		{
			float a1 = _pi * (float)(lat + 1) / (m_Slices + 1);
			float sin1 = sinf(a1);
			float cos1 = cosf(a1);

			for (int lon = 0; lon <= m_Segments; lon++)
			{
				float a2 = _2pi * (float)(lon == m_Segments ? 0 : lon) / m_Segments;
				float sin2 = sinf(a2);
				float cos2 = cosf(a2);

				verts[lon + lat * (m_Segments + 1) + 1].position = XMFLOAT4(sin1 * cos2 * m_Radius, cos1 * m_Radius, sin1 * sin2 * m_Radius, 1);
			}
		}
		verts[verts.size() - 1].position = XMFLOAT4(0, -m_Radius, 0, 1);

		int NumFaces = (int)verts.size();
		int NumTris = NumFaces * 2;
		int NumIndices = NumTris * 3;
		std::vector<int>  triangles(NumIndices);
		//int* triangles = new int[nbIndexes];


		int i = 0;
		for (int lon = 0; lon < m_Segments; lon++)
		{
			triangles[i++] = lon + 2;
			triangles[i++] = lon + 1;
			triangles[i++] = 0;
		}

		// Middle
		for (int lat = 0; lat < m_Slices - 1; lat++)
		{
			for (int lon = 0; lon < m_Segments; lon++)
			{
				int current = lon + lat * (m_Segments + 1) + 1;
				int next = current + m_Segments + 1;

				triangles[i++] = current;
				triangles[i++] = current + 1;
				triangles[i++] = next + 1;

				triangles[i++] = current;
				triangles[i++] = next + 1;
				triangles[i++] = next;
			}
		}

		// Bottom Cap
		for (int lon = 0; lon < m_Segments; lon++)
		{
			triangles[i++] = (int)verts.size() - 1;
			triangles[i++] = (int)verts.size() - (lon + 2) - 1;
			triangles[i++] = (int)verts.size() - (lon + 1) - 1;
		}
		m_NumTriangles = (int)verts.size();
		m_NumIndices = (int)triangles.size();

		//Create D3D resources
		D3D12_HEAP_PROPERTIES heapProperty;
		ZeroMemory(&heapProperty, sizeof(heapProperty));
		heapProperty.Type = D3D12_HEAP_TYPE_UPLOAD;

		D3D12_RESOURCE_DESC resourceDesc;
		ZeroMemory(&resourceDesc, sizeof(resourceDesc));
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Alignment = 0;
		resourceDesc.SampleDesc.Count = 1;
		resourceDesc.SampleDesc.Quality = 0;
		resourceDesc.MipLevels = 1;
		resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.Width = sizeof(SimpleVertex) * verts.size();
		resourceDesc.Height = 1;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		hr = pDevice->CreateCommittedResource(&heapProperty, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(m_VertexBuffer.GetAddressOf()));
		ThrowIfFailed(hr, "Faield to create commited resource for skysphere");

		UINT8* dataBegin;
		hr = m_VertexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&dataBegin));
		ThrowIfFailed(hr, "Failed to map resource fo GPU");
		memcpy(dataBegin, &verts[0], sizeof(SimpleVertex) * verts.size());
		m_VertexBuffer->Unmap(0, nullptr);

		m_VertexView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
		m_VertexView.StrideInBytes = sizeof(SimpleVertex);
		m_VertexView.SizeInBytes = sizeof(SimpleVertex) * (UINT)verts.size();

		resourceDesc.Width = sizeof(int) * triangles.size();
		hr = pDevice->CreateCommittedResource(&heapProperty, D3D12_HEAP_FLAG_NONE, &resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(m_IndexBuffer.GetAddressOf()));
		ThrowIfFailed(hr, "Failed to create commited resource for skysphere");
		m_IndexBuffer->Map(0, nullptr, reinterpret_cast<void**>(&dataBegin));
		ThrowIfFailed(hr, "Failed to map commited resource to GPU");
		memcpy(dataBegin, &triangles[0], sizeof(int) * triangles.size());
		m_IndexBuffer->Unmap(0, nullptr);


		m_IndexView.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
		m_IndexView.Format = DXGI_FORMAT_R32_UINT;
		m_IndexView.SizeInBytes = sizeof(int) * (UINT)triangles.size();
	}

	void ieD3D12SphereRenderer::Render(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList)
	{
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		commandList->IASetIndexBuffer(&m_IndexView);
		commandList->IASetVertexBuffers(0, 1, &m_VertexView);
		commandList->DrawIndexedInstanced(m_NumIndices, 1, 0, 0, 0);
	}

}
