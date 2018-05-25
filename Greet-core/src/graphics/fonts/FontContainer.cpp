#include "FontContainer.h"
#include "Font.h"

namespace Greet{

	FontContainer::FontContainer(const std::string& filename, const std::string& name)
	: m_filename(filename),m_data(nullptr),m_datasize(0), m_name(name)
	{
	}
	
	FontContainer::FontContainer(const byte* data, uint datasize, const std::string& name)
	: m_filename(""),m_data(data),m_datasize(datasize), m_name(name)
	{
		
	}

	FontContainer::~FontContainer()
	{
		auto it = m_fonts.begin();
		while(it != m_fonts.end())
		{
			delete (*it);
			it++;
		}
		m_fonts.clear();
	}

	Font* FontContainer::GetSize(uint size)
	{
		for (auto it = m_fonts.begin();it!=m_fonts.end();it++)
		{
			if ((*it)->GetSize() > size)
			{
				Font* font = new Font(this,size);
				m_fonts.insert(it,font);
				return font;
			}
			if ((*it)->GetSize() == size)
				return *it;
		}
		Font* font = new Font(this,size);
		m_fonts.push_back(font);
		return font;
	}

}