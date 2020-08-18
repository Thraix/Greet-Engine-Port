#pragma once

#include <graphics/fonts/Font.h>
#include <graphics/gui/GUIUtils.h>
#include <graphics/gui/TLBR.h>
#include <logging/Log.h>
#include <math/Vec4.h>
#include <utils/MetaFile.h>

#include <unordered_map>
#include <string>

namespace Greet
{
  template<typename T>
  struct StyleVariable
  {
    bool mbSetVariable = false; // if false the value will be set to the inherited value
    T mVariable;
    T* mpVariable;

    StyleVariable(bool abSetVariable, T* apVariable)
      : mbSetVariable{abSetVariable}, mVariable{*apVariable}, mpVariable{apVariable}
    {}
  };

  template <typename T>
  struct StyleVariableMap
  {
    std::unordered_map<std::string, T*> values;

    StyleVariableMap()
    {}

    StyleVariableMap(const std::initializer_list<std::pair<std::string, T*>>& values)
      : values{values.begin(), values.end()}
    {}

    void Merge(const StyleVariableMap& avValues)
    {
      for(auto&& value : avValues.values)
      {
        auto it = values.find(value.first);
        if(it != values.end())
        {
          Log::Error("Styling already exist for component: ", value.first);
          continue;
        }
        values.emplace(value.first, value.second);
      }
    }
  };

  template <typename T, typename StrToValFunc>
  struct StyleMap
  {
    std::unordered_map<std::string, StyleVariable<T>> values;
    StyleMap* inherit = nullptr;

    void SetInherit(StyleMap* aInherit)
    {
      inherit = aInherit;
    }

    void Load(const std::unordered_map<std::string, T>& avValues)
    {
      for(auto&& value : avValues)
      {
        SetValue(value.first, value.second);
      }
    }

    void Load(const std::string& mode, const XMLObject& object)
    {
      for(auto&& value : values)
      {
        std::string attribute = mode == "normal" ? value.first : mode + "-" + value.first;
        if(object.HasAttribute(attribute))
          SetValue(value.first, StrToValFunc::Get(object.GetAttribute(attribute)));
      }
    }

    void Load(const std::string& mode, const MetaFileClass& styling)
    {
      for(auto&& value : values)
      {
        std::string attribute = mode == "normal" ? value.first : mode + "-" + value.first;
        if(styling.HasValue(attribute) && !value.second.mbSetVariable)
        {
          SetValue(value.first, StrToValFunc::Get(styling.GetValue(attribute)));
        }
      }
    }


    void AddVariables(const StyleVariableMap<T>& avValues)
    {
      for(auto&& value : avValues.values)
      {
        auto it = values.find(value.first);
        if(it != values.end())
        {
          Log::Error("Styling already exist for component: ", value.first);
          continue;
        }
        values.emplace(value.first, StyleVariable{false, value.second});
      }
    }

    bool HasValue(const std::string& key)
    {
      return values.find(key) != values.end();
    }

    const T& GetValue(const std::string& key) const
    {
      auto it = values.find(key);
      ASSERT(it != values.end(), "Component does not contain styling: ", key);
      if(!it->second.mbSetVariable && inherit)
        return inherit->GetValue(key);
      return it->second.mVariable;
    }

    StyleMap& SetValue(const std::string& key, const T& value)
    {
      auto it = values.find(key);
      if(it == values.end())
      {
        Log::Error("Component doesn't contain styling: ", key);
        return *this;
      }
      it->second.mbSetVariable = true;
      it->second.mVariable  = value;
      *it->second.mpVariable = value;
      return *this;
    }

    void SetStyle()
    {
      for(auto&& value : values)
      {
        *value.second.mpVariable = GetValue(value.first);
      }
    }
  };

  struct ColorStyle
  {
    static Vec4 Get(const std::string& str) { return GUIUtils::GetColor(str); }
  };

  struct TLBRStyle
  {
    static TLBR Get(const std::string& str) { return GUIUtils::GetTLBR(str); }
  };

  struct FloatStyle
  {
    static float Get(const std::string& str) { return GUIUtils::GetFloat(str); }
  };

  struct IntStyle
  {
    static int Get(const std::string& str) { return GUIUtils::GetInt(str); }
  };

  struct BoolStyle
  {
    static bool Get(const std::string& str) { return GUIUtils::GetBoolean(str); }
  };

  struct FontStyle
  {
    static Font Get(const std::string& str) { return GUIUtils::GetFont(str); }
  };

  struct GUISizeStyle
  {
    static GUISize Get(const std::string& str) { return GUIUtils::GetGUISize(str); }
  };
}
