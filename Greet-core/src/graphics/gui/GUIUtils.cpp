#include "GUIUtils.h"

#include <graphics/gui/Frame.h>
#include <graphics/gui/Button.h>

namespace Greet
{
	Frame* GUIUtils::GetFrame(const XMLObject& object)
	{
		if (object.GetName() == "Frame")
		{
			return new Frame(object);
		}
		else if (object.GetName() == "FrameHeader")
		{

		}
		Log::Warning("Could not read XML object ", object.GetName(), ".");
		return new Frame();
	}

	Component* GUIUtils::GetComponent(const XMLObject& object, Component* parent)
	{
		if (object.GetName() == "Button")
		{
			return new Button(object, parent);
		}
		if (object.GetName() == "Component")
		{
			return new Component(object, parent);
		}
		Log::Warning("Could not read XML object ", object.GetName(), ".");
		return new Component(); // Return plain content to avoid crash.
	}
}
