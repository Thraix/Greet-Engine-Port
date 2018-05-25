#include <graphics/gui/Border.h>


namespace Greet {

	Border::Border(const Vec2& position,const Vec2& size, const std::string& texturename, const uint& borderSize, const Vec2& uv0, const Vec2& uv1, const Vec2& uv2, const Vec2& uv3)
	: m_position(position), m_size(size), m_color(0xffffffff), data(new BorderData(texturename,borderSize,uv0,uv1,uv2,uv2))
	{

	}

	Border::Border(const Vec2& position, const Vec2& size, BorderData* data)
	: m_position(position), m_size(size), m_color(0xffffffff), data(data)
	{

	}

	Border::~Border()
	{
		delete data;
	}

	void Border::Submit(Renderer2D* renderer) const
	{

		Vec2 borderSize(data->borderSize,data->borderSize);
		Vec2 size(m_size-data->borderSize*2);
		// TODO: fix these errors
		// 0,0
		//renderer->submit(m_position,borderSize,data->texture->getTexID(),data->uv0,data->uv1-data->uv0,0xffffffff);
		// 1,0
		//renderer->submit(Vec2(m_position.x+data->borderSize,m_position.y),Vec2(size.x,borderSize.y),data->texture->getTexID(),Vec2(data->uv1.x,data->uv0.y),Vec2(data->uv2.x-data->uv1.x,data->uv1.y-data->uv0.y),0xffffffff);
		// 2,0
		//renderer->submit(Vec2(m_position.x+size.x+borderSize.x,m_position.y),borderSize,data->texture->getTexID(),Vec2(data->uv2.x,data->uv0.y),Vec2(data->uv3.x-data->uv2.x,data->uv1.y-data->uv0.y),0xffffffff);
		// 0,1
		//renderer->submit(Vec2(m_position.x,m_position.y+data->borderSize),Vec2(data->borderSize,size.y),data->texture->getTexID(),Vec2(data->uv0.x,data->uv1.x),Vec2(data->uv1.x-data->uv0.x,data->uv2.x-data->uv1.x),0xffffffff);
		// 1,1
		//renderer->submit(Vec2(m_position.x+data->borderSize,m_position.y+data->borderSize),Vec2(size.x,size.y),data->texture->getTexID(),Vec2(data->uv1.x,data->uv1.y),Vec2(data->uv2.x-data->uv1.x,data->uv2.y-data->uv1.y),0xffffffff);
		// 2,1
		//renderer->submit(Vec2(m_position.x+size.x+borderSize.x,m_position.y+data->borderSize),Vec2(data->borderSize,size.y),data->texture->getTexID(),Vec2(data->uv2.x,data->uv1.y),Vec2(data->uv3.x-data->uv2.x,data->uv2.y-data->uv1.y),0xffffffff);
		// 0,2
		//renderer->submit(Vec2(m_position.x,m_position.y+size.y+borderSize.y),borderSize,data->texture->getTexID(),Vec2(data->uv0.x,data->uv2.x),Vec2(data->uv1.x-data->uv0.x,data->uv3.x-data->uv2.x),0xffffffff);
		// 1,2
		//renderer->submit(Vec2(m_position.x+data->borderSize,m_position.y+size.y+borderSize.y),Vec2(size.x,borderSize.y),data->texture->getTexID(),Vec2(data->uv1.x,data->uv2.y),Vec2(data->uv2.x-data->uv1.x,data->uv3.y-data->uv2.y),0xffffffff);
		// 2,2
		//renderer->submit(Vec2(m_position.x+size.x+borderSize.x,m_position.y+size.y+borderSize.y),borderSize,data->texture->getTexID(),Vec2(data->uv2.x,data->uv2.y),Vec2(data->uv3.x-data->uv2.x,data->uv3.y-data->uv2.y),0xffffffff);
	}

	bool Border::Update(float timeElapsed)
	{
		return false;
	}

	void Border::SetPosition(const Vec2& position)
	{
		m_position = position;
	}

	void Border::SetSize(const Vec2& size)
	{
		m_size = size;
	}

}
