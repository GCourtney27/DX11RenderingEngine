#pragma once
#include <Engine_pch.h>

#include <Insight/Core.h>
#include "Insight/Events/Event.h"

class RenderingContext;

namespace Insight {

	struct WindowProps
	{
		std::string Title;
		std::string Class;
		uint32_t Width;
		uint32_t Height;

		WindowProps(const std::string& title = "Insight Editor", const std::string winClass = "IE Class", uint32_t width = 1700, uint32_t height = 1000)
			: Title(title), Width(width), Height(height) {}
	};

	class INSIGHT_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;
		
		virtual ~Window() {}

		virtual void OnUpdate() = 0;
		virtual void Shutdown() = 0;

		virtual void PostInit() = 0;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;
		virtual std::pair<uint32_t, uint32_t> GetDimensions() const = 0;
		virtual bool SetWindowTitle(const std::string& newText, bool completlyOverride = false) = 0;
		virtual bool SetWindowTitleFPS(float fps) = 0;

		virtual void Resize(uint32_t newWidth, uint32_t newHeight, bool isMinimized) = 0;
		virtual void ToggleFullScreen(bool enabled) = 0;

		// Window Attributes
		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual bool ProccessWindowMessages() = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual const bool& IsFullScreenActive() const = 0;
		virtual const bool& IsVsyncActive() const = 0;

		virtual void* GetNativeWindow() const = 0;

		virtual inline float GeAspectRatio() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());


	protected:

	};

}