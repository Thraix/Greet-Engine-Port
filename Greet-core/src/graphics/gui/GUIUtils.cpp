#include "GUIUtils.h"

#include "Container.h"
#include "Content.h"

namespace Greet
{
	Container* GUIUtils::GetContainer(const XMLObject& object)
	{
		if (object.GetName() == "Container")
		{
			return new Container(object);
		}
		else if (object.GetName() == "FrameContainer")
		{

		}

		return new Container();
	}

	Content* GUIUtils::GetContent(const XMLObject& object, Content* parent)
	{
		if (object.GetName() == "Button")
		{
			//return new Button(object);
		}
		if (object.GetName() == "Content")
		{
			return new Content(object, parent);
		}
		Log::Warning("Could not read XML object ", object.GetName(), ".");
		return new Content(); // Return plain content to avoid crash.
	}
}
