#pragma once

#include <Runtime/Core.h>
#include <Runtime/Graphics/Public/GraphicsCore.h>

#include "Runtime/Graphics/Public/ResourceManagement/ITextureManager.h"
#include "Platform/DirectX12/Public/Resource/D3D12Texture.h"

#include "Runtime/Graphics/Public/Resource/ITexture.h"

namespace Insight
{
	namespace Graphics
	{
		namespace DX12
		{
			class INSIGHT_API D3D12ManagedTexture : public IManagedTexture, public D3D12Texture
			{
			public:
				D3D12ManagedTexture(const EString& FileName)
					: IManagedTexture(FileName)
				{
				}
				virtual ~D3D12ManagedTexture() = default;

				virtual void CreateFromMemory(ByteArray memory, EDefaultTexture fallback, bool bForceSRGB) override;

			};


			class INSIGHT_API D3D12TextureManager : public ITextureManager
			{
				friend class IRenderContextFactory;
				friend class D3D12RenderContextFactory;
			public:
				D3D12TextureManager() 
				{
					Initialize();
				}
				virtual ~D3D12TextureManager() 
				{
					UnInitialize();
				}

				virtual ITextureRef LoadTexture(const EString& FileName, EDefaultTexture Fallback, bool forceSRGB) override;

			private:
				virtual IManagedTexture* FindOrLoadTexture(const EString& FileName, EDefaultTexture Fallback, bool forceSRGB) override;
				virtual void DestroyTexture(const EString& Key) override;
				
			protected:
				virtual void Initialize() override;
				virtual void UnInitialize() override;

			private:
				std::map<EString, std::unique_ptr<D3D12ManagedTexture>> m_TextureCache;
				D3D12Texture m_DefaultTextures[DT_NumDefaultTextures];
			};
		}
	}
}
