#pragma once

#include <Insight/Core.h>
#include "Insight/Events/Event.h"

namespace Insight {

	class INSIGHT_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {}
		virtual void OnDetach() {}
		virtual void OnUpdate(const float DeltaTime) {}
		virtual void OnEvent(Event& Event) {}
		virtual void OnImGuiRender() {}

		inline const std::string& GetName() const { return m_DebugName; }
	protected:
		std::string m_DebugName;
	};

}