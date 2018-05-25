#if 0
variables:
	Renderable2D* lensflare1;
	Renderable2D* lensflare2;
	Renderable2D* lensflare3;
	Renderable2D* lensflare4;
	Renderable2D* lensflare5;
	Renderable2D* lensflare6;

init:
	lensflare1 = new Renderable2D(math::vec2(0,0),math::vec2(400,400),0x66ffffff,new Sprite(TextureManager::get("lensflare1")->getTexId(),0,0,math::vec2(0,0),math::vec2(1,1)),NULL);
	lensflare2 = new Renderable2D(math::vec2(0,0),math::vec2(100,100),0x66ffffff,new Sprite(TextureManager::get("lensflare2")->getTexId(),0,0,math::vec2(0,0),math::vec2(1,1)),NULL);
	lensflare3 = new Renderable2D(math::vec2(0,0),math::vec2(30,30),0x66ffffff,new Sprite(TextureManager::get("lensflare3")->getTexId(),0,0,math::vec2(0,0),math::vec2(1,1)),NULL);
	lensflare4 = new Renderable2D(math::vec2(0,0),math::vec2(90,90),0x66ffffff,new Sprite(TextureManager::get("lensflare3")->getTexId(),0,0,math::vec2(0,0),math::vec2(1,1)),NULL);
	lensflare5 = new Renderable2D(math::vec2(0,0),math::vec2(50,50),0x66ffffff,new Sprite(TextureManager::get("lensflare3")->getTexId(),0,0,math::vec2(0,0),math::vec2(1,1)),NULL);
	lensflare6 = new Renderable2D(math::vec2(0,0),math::vec2(110,110),0x66ffffff,new Sprite(TextureManager::get("lensflare4")->getTexId(),0,0,math::vec2(0,0),math::vec2(1,1)),NULL);
	lensflare1->render = false;
	lensflare2->render = false;
	lensflare3->render = false;
	lensflare4->render = false;
	lensflare5->render = false;
	lensflare6->render = false;

	uilayer->add(lensflare1);
	uilayer->add(lensflare2);
	uilayer->add(lensflare3);
	uilayer->add(lensflare4);
	uilayer->add(lensflare5);
	uilayer->add(lensflare6);

update:
	math::vec3 sunPos = camera->position + math::vec3(-1, 0.5f, 0.3f);
	math::vec3 p = renderer3d->getScreenCoordination(sunPos, Window::getWidth(), Window::getHeight());
	if (p.z > 0.0)
	{
		math::vec2 sun = math::vec2(p.x, p.y);
		math::vec2 vt = math::vec2(Window::getWidth() / 2.0f, Window::getHeight() / 2.0f)-sun;
		lensflare1->setPosition(sun + vt*-0.1f - lensflare1->getSize()/2.0f);
		lensflare2->setPosition(sun - lensflare2->getSize()/2.0f);
		lensflare3->setPosition(sun + vt * 0.8f - lensflare3->getSize()/2.0f);
		lensflare4->setPosition(sun + vt * 1.1f - lensflare4->getSize()/2.0f);
		lensflare5->setPosition(sun + vt * 1.05f - lensflare5->getSize()/2.0f);
		lensflare6->setPosition(sun + vt * 1.4f - lensflare6->getSize()/2.0f);
		uint alpha = 1000000.0f / vt.lengthSQ();
		if (alpha > 100)
			alpha = 100;
		lensflare1->setColor((alpha << 24) | (0x00ffffff & lensflare1->getColor()));
		lensflare2->setColor((alpha << 24) | (0x00ffffff & lensflare1->getColor()));
		lensflare3->setColor((alpha << 24) | (0x00ffffff & lensflare1->getColor()));
		lensflare4->setColor((alpha << 24) | (0x00ffffff & lensflare1->getColor()));
		lensflare5->setColor((alpha << 24) | (0x00ffffff & lensflare1->getColor()));
		lensflare6->setColor((alpha << 24) | (0x00ffffff & lensflare1->getColor()));
	}
	lensflare1->render = p.z > 0.0f;
	lensflare2->render = p.z > 0.0f;
	lensflare3->render = p.z > 0.0f;
	lensflare4->render = p.z > 0.0f;
	lensflare5->render = p.z > 0.0f;
	lensflarVe6->render = p.z > 0.0f;
#endif