#include "ComponentFactory.h"

#include <graphics/gui/Component.h>
#include <graphics/gui/Frame.h>
#include <utils/xml/XMLException.h>

namespace Greet
{
  ComponentFactory::ComponentMap* ComponentFactory::guiComponents;
  FrameFactory::FrameMap* FrameFactory::guiFrames;

  Component* ComponentFactory::GetComponent(const XMLObject& xmlObject, Component* parent)
  {
    ComponentMap* map = GetMap();
    auto it = map->find(xmlObject.GetName());
    if(it == map->end())
    {
      Log::Warning("There is no such component: ", xmlObject.GetName(), ".");
      XMLObject o("Failed", std::map<std::string, std::string>(), "");
      return new Component(o,parent); // Return plain content to avoid crash.
    }
    Component* component = it->second(xmlObject, parent);
    component->LoadStyles(xmlObject);
    return component;
  }

  Frame* FrameFactory::GetFrame(const std::string& filePath)
  {
    try
    {
      return GetFrame(XML::FromFile(filePath));
    }
    catch(XMLException& e)
    {
      Log::Error(e.what());
      return new Frame();
    }
  }

  Frame* FrameFactory::GetFrame(const XMLObject& xmlObject)
  {
    FrameMap* map = GetMap();
    auto it = map->find(xmlObject.GetName());
    if(it == map->end())
    {
      Log::Warning("There is no such frame: ", xmlObject.GetName(), ".");
      XMLObject o("Failed", std::map<std::string, std::string>(), "");
      return new Frame(o); // Return plain content to avoid crash.
    }
    Frame* frame = it->second(xmlObject);
    frame->LoadStyles(xmlObject);
    return frame;
  }
}
