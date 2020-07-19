#pragma once

#include <Insight/Core.h>

#include "Insight/Rendering/Geometry/Model.h"

namespace Insight {

	using namespace Microsoft::WRL;

	class GeometryManager
	{
	public:
		typedef std::vector<StrongModelPtr> SceneModels;
	public:
		GeometryManager();
		~GeometryManager();

		bool Init();
		virtual bool LoadResourcesFromJson(const rapidjson::Value& jsonMeshes);

		void Render(RenderPass RenderPass);

		SceneModels* GetSceneModels() { return &m_Models; }

		void GatherGeometry();
		void PostRender();
		void FlushModelCache();
		
		inline void RegisterModel(StrongModelPtr Model) { m_Models.push_back(Model); }
		inline void UnRegisterModel(StrongModelPtr Model){ std::remove(m_Models.begin(), m_Models.end(), Model); }
	private:
		SceneModels m_Models;  
		D3D12_GPU_VIRTUAL_ADDRESS m_CbvUploadHeapHandle;
		D3D12_GPU_VIRTUAL_ADDRESS m_CbvMaterialHeapHandle;
		
		UINT8* m_CbvPerObjectGPUAddress = nullptr;
		UINT8* m_CbvMaterialGPUAddress = nullptr;

		ID3D12Resource* m_ConstantBufferUploadHeaps = nullptr;
		ID3D12Resource* m_ConstantBufferMaterialUploadHeaps = nullptr;
		ID3D12GraphicsCommandList* m_pScenePassCommandList = nullptr;
		ID3D12GraphicsCommandList* m_pShadowPassCommandList = nullptr;

		int ConstantBufferPerObjectAlignedSize = (sizeof(CB_VS_PerObject) + 255) & ~255;
		int ConstantBufferPerObjectMaterialAlignedSize = (sizeof(CB_PS_VS_PerObjectAdditives) + 255) & ~255;
		UINT32 m_PerObjectCBDrawOffset = 0u;
		UINT32 m_GPUAddressUploadOffset = 0u;
	};

}
