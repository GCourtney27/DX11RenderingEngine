#pragma once

/*
	Single include for any application utilizing the Insight Engine.
*/

// Core
#include "Insight/Core/Application.h"
#include "Insight/Core/Layer/Layer.h"
#include "Insight/Core/Layer/ImGui_Layer.h"
#if defined (IE_PLATFORM_BUILD_WIN32)
#include "Platform/Win32/Win32_Window.h"
#elif defined (IE_PLAFORM_BUILD_UWP)
// TODO UWP Window include
#endif

// Util
#include "Insight/Core/Log.h"
#include "Insight/Core/ie_Exception.h"
#include "Insight/Utilities/Profiling.h"

// Rendering
#include "Insight/Rendering/Material.h"
#include "Platform/DirectX_12/Direct3D12_Context.h"
#include "Platform/DirectX_12/Wrappers/D3D12_Texture.h"

// Game Framework
#include "Insight/Runtime/Components/Static_Mesh_Component.h"



