#pragma once

#if 0
#include <graphics/renderers/EntityModel3D.h>
#include <graphics/models/MeshFactory.h>

class Tree
{
public:
	Tree(Greet::BatchRenderer3D* renderer, float x, float y, float z)
	{
		using namespace Greet::graphics;
		using namespace Greet::model;
		Shader* shader = Shader::fromFile("res/shaders/3dshader.vert","res/shaders/3dshader.frag");
		Material* material = new Material(shader,NULL);
		material->setReflectivity(0.0f);
		material->setShineDamper(0.0f);
		Mesh* tetra1 = MeshFactory::tetrahedron(0,0,0,1);
		Mesh* tetra2 = MeshFactory::tetrahedron(0,0,0,1);
		Mesh* tetra3 = MeshFactory::tetrahedron(0,0,0,1);
		uint colors1[4];
		for (int i = 0;i < 4;i++)
			colors1[i] = 0xff00ee00;
		uint colors2[4];
		for (int i = 0;i < 4;i++)
			colors2[i] = 0xff00aa00;
		uint colors3[4];
		for (int i = 0;i < 4;i++)
			colors3[i] = 0xff007700;
		tetra1->addAttribute(MESH_COLORS_LOCATION,4,colors1);
		tetra2->addAttribute(MESH_COLORS_LOCATION,4,colors2);
		tetra3->addAttribute(MESH_COLORS_LOCATION,4,colors3);
		MaterialModel* modelMaterial1 = new MaterialModel(tetra1,*material);
		MaterialModel* modelMaterial2 = new MaterialModel(tetra2,*material);
		MaterialModel* modelMaterial3 = new MaterialModel(tetra3,*material);

		EntityModel* top = new EntityModel(*modelMaterial1,x,y+1.25f,z,0.667f,1.0f,0.667f,0,0,0);
		EntityModel* middle = new EntityModel(*modelMaterial2,x,y+0.75f,z,1,1.5f,1,0,0,0);
		EntityModel* bottom = new EntityModel(*modelMaterial3,x,y,z,1.5f,2,1.5f,0,0,0);
		renderer->submit(top);
		renderer->submit(middle);
		renderer->submit(bottom);
	}

	~Tree()
	{
	}
};

#endif