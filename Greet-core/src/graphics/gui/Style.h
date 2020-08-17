#pragma once

// TODO: This whole class needs rewritting
// To add a new styling type you have to copy paste
// so much code it gets unmanagable...

#include <graphics/gui/TLBR.h>
#include <unordered_map>

namespace Greet
{

  class Component;
  template<typename T>
  struct StyleVariable
  {
    bool mbSetVariable; // if false the value will be set to the inherited value
    T mVariable;
    T* mpVariable;

    StyleVariable(bool abSetVariable, T* apVariable)
      : mbSetVariable{abSetVariable}, mVariable{*apVariable}, mpVariable{apVariable}
    {}
  };

  template <typename T>
  using StyleVariableMap = std::unordered_map<std::string, T*>;

  struct Styling
  {
    std::unordered_map<std::string, Vec4> colors;
    std::unordered_map<std::string, TLBR> tlbrs;
    std::unordered_map<std::string, float> floats;
    std::unordered_map<std::string, int> ints;
    std::unordered_map<std::string, bool> bools;
    std::unordered_map<std::string, Font> fonts;
    std::unordered_map<std::string, GUISize> sizes;
  };

  struct StylingVariables
  {
    StyleVariableMap<Vec4> colors;
    StyleVariableMap<TLBR> tlbrs;
    StyleVariableMap<float> floats;
    StyleVariableMap<int> ints;
    StyleVariableMap<bool> bools;
    StyleVariableMap<Font> fonts;
    StyleVariableMap<GUISize> sizes;

    void Merge(const StylingVariables& variables)
    {
      for(auto&& color : variables.colors)
      {
        auto it = colors.find(color.first);
        if(it != colors.end())
        {
          Log::Error("Color styling already exist for component: ", color.first);
          continue;
        }
        colors.emplace(color.first, color.second);
      }

      for(auto&& tlbr: variables.tlbrs)
      {
        auto it = tlbrs.find(tlbr.first);
        if(it != tlbrs.end())
        {
          Log::Error("TLBR styling already exist for component: ", tlbr.first);
          continue;
        }
        tlbrs.emplace(tlbr.first, tlbr.second);
      }

      for(auto&& f : variables.floats)
      {
        auto it = floats.find(f.first);
        if(it != floats.end())
        {
          Log::Error("Float styling already exist for component: ", f.first);
          continue;
        }
        floats.emplace(f.first, f.second);
      }

      for(auto&& i : variables.ints)
      {
        auto it = ints.find(i.first);
        if(it != ints.end())
        {
          Log::Error("Int styling already exist for component: ", i.first);
          continue;
        }
        ints.emplace(i.first, i.second);
      }

      for(auto&& b : variables.bools)
      {
        auto it = bools.find(b.first);
        if(it != bools.end())
        {
          Log::Error("Boolean styling already exist for component: ", b.first);
          continue;
        }
        bools.emplace(b.first, b.second);
      }

      for(auto&& font : variables.fonts)
      {
        auto it = fonts.find(font.first);
        if(it != fonts.end())
        {
          Log::Error("Font styling already exist for component: ", font.first);
          continue;
        }
        fonts.emplace(font.first, font.second);
      }

      for(auto&& size : variables.sizes)
      {
        auto it = sizes.find(size.first);
        if(it != sizes.end())
        {
          Log::Error("Size styling already exist for component: ", size.first);
          continue;
        }
        sizes.emplace(size.first, size.second);
      }
    }
  };

  struct ComponentStyle
  {
    template <typename T>
    using StyleMap = std::unordered_map<std::string, StyleVariable<T>>;

    private:
      ComponentStyle* inherit;

      StyleMap<Vec4> colors;
      StyleMap<TLBR> tlbrs;
      StyleMap<float> floats;
      StyleMap<int> ints;
      StyleMap<bool> bools;
      StyleMap<Font> fonts;
      StyleMap<GUISize> sizes;
    public:

      ComponentStyle(const StylingVariables& variables, ComponentStyle* inherit)
        : inherit{inherit}
      {
        AddVariables(variables);
      }

      void SetInherit(ComponentStyle* inherit)
      {
        this->inherit = inherit;
      }

      void Load(const Styling& styling)
      {
        for(auto&& color : styling.colors)
        {
          SetColor(color.first, color.second);
        }

        for(auto&& tlbr: styling.tlbrs)
        {
          SetTLBR(tlbr.first, tlbr.second);
        }

        for(auto&& f : styling.floats)
        {
          SetFloat(f.first, f.second);
        }

        for(auto&& i : styling.ints)
        {
          SetInt(i.first, i.second);
        }

        for(auto&& b : styling.bools)
        {
          SetBool(b.first, b.second);
        }

        for(auto&& font : styling.fonts)
        {
          SetFont(font.first, font.second);
        }

        for(auto&& size : styling.sizes)
        {
          SetSize(size.first, size.second);
        }
      }

      void Load(const std::string& mode, const XMLObject& object)
      {
        // TODO: These should not set the pointer variable
        for(auto&& color : colors)
        {
          std::string attribute = mode == "normal" ? color.first : mode + "-" + color.first;
          if(object.HasAttribute(attribute))
            SetColor(color.first, GUIUtils::GetColor(object.GetAttribute(attribute)));
        }

        for(auto&& tlbr: tlbrs)
        {
          // TODO: This is not correct as you can do marginTop="1"
          std::string attribute = mode == "normal" ? tlbr.first : mode + "-" + tlbr.first;
          if(object.HasAttribute(attribute))
            SetTLBR(tlbr.first, TLBR{attribute, object});
        }

        for(auto&& f : floats)
        {
          std::string attribute = mode == "normal" ? f.first : mode + "-" + f.first;
          if(object.HasAttribute(attribute))
            SetFloat(f.first, GUIUtils::GetFloat(object.GetAttribute(attribute)));
        }

        for(auto&& i : ints)
        {
          std::string attribute = mode == "normal" ? i.first : mode + "-" + i.first;
          if(object.HasAttribute(attribute))
            SetInt(i.first, GUIUtils::GetInt(object.GetAttribute(attribute)));
        }

        for(auto&& b : bools)
        {
          std::string attribute = mode == "normal" ? b.first : mode + "-" + b.first;
          if(object.HasAttribute(attribute))
            SetBool(b.first, GUIUtils::GetBoolean(object.GetAttribute(attribute)));
        }

        for(auto&& font : fonts)
        {
          std::string attribute = mode == "normal" ? font.first : mode + "-" + font.first;
          if(object.HasAttribute(attribute))
            SetFont(font.first, GUIUtils::GetFont(object.GetAttribute(attribute)));
        }

        for(auto&& size : sizes)
        {
          std::string attribute = mode == "normal" ? size.first : mode + "-" + size.first;
          if(object.HasAttribute(attribute))
            SetSize(size.first, GUIUtils::GetGUISize(object.GetAttribute(attribute)));
        }
      }

      void AddVariables(const StylingVariables& style)
      {
        for(auto&& color : style.colors)
        {
          auto it = colors.find(color.first);
          if(it != colors.end())
          {
            Log::Error("Color styling already exist for component: ", color.first);
            continue;
          }
          colors.emplace(color.first, StyleVariable{inherit == nullptr, color.second});
        }

        for(auto&& tlbr: style.tlbrs)
        {
          auto it = tlbrs.find(tlbr.first);
          if(it != tlbrs.end())
          {
            Log::Error("TLBR styling already exist for component: ", tlbr.first);
            continue;
          }
          tlbrs.emplace(tlbr.first, StyleVariable{inherit == nullptr, tlbr.second});
        }

        for(auto&& f : style.floats)
        {
          auto it = floats.find(f.first);
          if(it != floats.end())
          {
            Log::Error("Float styling already exist for component: ", f.first);
            continue;
          }
          floats.emplace(f.first, StyleVariable{inherit == nullptr, f.second});
        }

        for(auto&& i : style.ints)
        {
          auto it = ints.find(i.first);
          if(it != ints.end())
          {
            Log::Error("Int styling already exist for component: ", i.first);
            continue;
          }
          ints.emplace(i.first, StyleVariable{inherit == nullptr, i.second});
        }

        for(auto&& b : style.bools)
        {
          auto it = bools.find(b.first);
          if(it != bools.end())
          {
            Log::Error("Boolean styling already exist for component: ", b.first);
            continue;
          }
          bools.emplace(b.first, StyleVariable{inherit == nullptr, b.second});
        }

        for(auto&& font : style.fonts)
        {
          auto it = fonts.find(font.first);
          if(it != fonts.end())
          {
            Log::Error("Font styling already exist for component: ", font.first);
            continue;
          }
          fonts.emplace(font.first, StyleVariable{inherit == nullptr, font.second});
        }

        for(auto&& size : style.sizes)
        {
          auto it = sizes.find(size.first);
          if(it != sizes.end())
          {
            Log::Error("GUISize styling already exist for component: ", size.first);
            continue;
          }
          sizes.emplace(size.first, StyleVariable{inherit == nullptr, size.second});
        }
      }

      bool HasTLBR(const std::string& name)
      {
        if(tlbrs.find(name) != tlbrs.end())
          return true;
        if(inherit)
          return inherit->HasTLBR(name);
        return false;
      }

      bool HasBool(const std::string& name)
      {
        if(bools.find(name) != bools.end())
          return true;
        if(inherit)
          return inherit->HasBool(name);
        return false;
      }

      bool HasColor(const std::string& name)
      {
        if(colors.find(name) != colors.end())
          return true;
        if(inherit)
          return inherit->HasColor(name);
        return false;
      }

      bool HasFloat(const std::string& name)
      {
        if(floats.find(name) != floats.end())
          return true;
        if(inherit)
          return inherit->HasFloat(name);
        return false;
      }

      bool HasInt(const std::string& name)
      {
        if(ints.find(name) != ints.end())
          return true;
        if(inherit)
          return inherit->HasInt(name);
        return false;
      }

      const TLBR& GetTLBR(const std::string& name) const
      {
        auto it = tlbrs.find(name);
        if(!it->second.mbSetVariable && inherit)
          return inherit->GetTLBR(name);
        return it->second.mVariable;
      }

      bool GetBool(const std::string& name) const
      {
        auto it = bools.find(name);
        if(!it->second.mbSetVariable && inherit)
          return inherit->GetBool(name);
        return it->second.mVariable;
      }

      const Vec4& GetColor(const std::string& name)
      {
        auto it = colors.find(name);
        if(!it->second.mbSetVariable && inherit)
          return inherit->GetColor(name);
        return it->second.mVariable;
      }

      float GetFloat(const std::string& name)
      {
        auto it = floats.find(name);
        if(!it->second.mbSetVariable && inherit)
          return inherit->GetFloat(name);
        return it->second.mVariable;
      }

      int GetInt(const std::string& name) const
      {
        auto it = ints.find(name);
        if(!it->second.mbSetVariable && inherit)
          return inherit->GetInt(name);
        return it->second.mVariable;
      }

      const Font& GetFont(const std::string& name) const
      {
        auto it = fonts.find(name);
        if(!it->second.mbSetVariable && inherit)
          return inherit->GetFont(name);
        return it->second.mVariable;
      }

      const GUISize& GetSize(const std::string& name) const
      {
        auto it = sizes.find(name);
        if(!it->second.mbSetVariable && inherit)
          return inherit->GetSize(name);
        return it->second.mVariable;
      }

      ComponentStyle& SetColor(const std::string& name, const Vec4& color)
      {
        auto it = colors.find(name);
        if(it == colors.end())
        {
          Log::Error("Component doesn't contain color: ", name);
          return *this;
        }
        it->second.mbSetVariable = true;
        it->second.mVariable  = color;
        *it->second.mpVariable = color;
        return *this;
      }

      ComponentStyle& SetTLBR(const std::string& name, const TLBR& tlbr)
      {
        auto it = tlbrs.find(name);
        if(it == tlbrs.end())
        {
          Log::Error("Component doesn't contain TLBR: ", name);
          return *this;
        }
        it->second.mbSetVariable = true;
        it->second.mVariable = tlbr;
        *it->second.mpVariable = tlbr;
        return *this;
      }

      ComponentStyle& SetBool(const std::string& name, bool b)
      {
        auto it = bools.find(name);
        if(it == bools.end())
        {
          Log::Error("Component doesn't contain bool: ", name);
          return *this;
        }
        it->second.mbSetVariable = true;
        it->second.mVariable = b;
        *it->second.mpVariable = b;
        return *this;
      }

      ComponentStyle& SetFloat(const std::string& name, float f)
      {
        auto it = floats.find(name);
        if(it == floats.end())
        {
          Log::Error("Component doesn't contain float: ", name);
          return *this;
        }
        it->second.mbSetVariable = true;
        it->second.mVariable = f;
        *it->second.mpVariable = f;
        return *this;
      }

      ComponentStyle& SetInt(const std::string& name, int i)
      {
        auto it = ints.find(name);
        if(it == ints.end())
        {
          Log::Error("Component doesn't contain int: ", name);
          return *this;
        }
        it->second.mbSetVariable = true;
        it->second.mVariable = i;
        *it->second.mpVariable = i;
        return *this;
      }

      ComponentStyle& SetFont(const std::string& name, const Font& font)
      {
        auto it = fonts.find(name);
        if(it == fonts.end())
        {
          Log::Error("Component doesn't contain font: ", name);
          return *this;
        }
        it->second.mbSetVariable = true;
        it->second.mVariable = font;
        *it->second.mpVariable = font;
        return *this;
      }

      ComponentStyle& SetSize(const std::string& name, const GUISize& size)
      {
        auto it = sizes.find(name);
        if(it == sizes.end())
        {
          Log::Error("Component doesn't contain size: ", name);
          return *this;
        }
        it->second.mbSetVariable = true;
        it->second.mVariable = size;
        *it->second.mpVariable = size;
        return *this;
      }

      void SetStyling()
      {
        for(auto&& color : colors)
        {
          *color.second.mpVariable = GetColor(color.first);
        }
        for(auto&& tlbr : tlbrs)
        {
          *tlbr.second.mpVariable = GetTLBR(tlbr.first);
        }
        for(auto&& f : floats)
        {
          *f.second.mpVariable = GetFloat(f.first);
        }
        for(auto&& i : ints)
        {
          *i.second.mpVariable = GetInt(i.first);
        }
        for(auto&& b : bools)
        {
          *b.second.mpVariable = GetBool(b.first);
        }
        for(auto&& font : fonts)
        {
          *font.second.mpVariable = GetFont(font.first);
        }
        for(auto&& size : sizes)
        {
          float oldSize = size.second.mpVariable->size;
          *size.second.mpVariable = GetSize(size.first);
          size.second.mpVariable->size = oldSize;
        }
      }
  };
}
