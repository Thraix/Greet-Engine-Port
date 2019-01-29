#pragma once

#include <set>
#include <Greet.h>
#include "Cube.h"

#define VMC_GRID_SIZE 100

#include "ModelExport.h"
#include "GridRenderer3D.h"
#include "Cube.h"
#include "Ray.h"
#include "tools/ToolBox.h"
#include <sstream>
#include "GridRenderer3D.h"

namespace vmc {
	class Grid : public Greet::Layer3D
	{
	private:
		GridRenderer3D* renderer;
		//Greet::ColorPicker* m_colorPicker;
		std::set<Cube, CubeCompare> m_grid;
		std::set<Cube, CubeCompare> m_gridVisible;
		std::vector<Cube> m_ray;
		uint m_color;

		ToolBox toolBox;

		bool hasAdjacent;
		bool hasSelected;
		Cube selected;
		Cube adjacent;

		bool renderAxis;
	public:
		static const uint GRID_SIZE;
	public:
		Grid();

    void OnEvent(Greet::Event& event) override;
		void OnPressed(Greet::KeyPressEvent& e);
		bool OnPressed(Greet::MousePressEvent& e);
		void OnReleased(Greet::MouseReleaseEvent& e);

		void Update(float timeElapsed) override;
		// Render the grid
		void Render() const override;

		void Remove(uint x, uint y, uint z);
		void Remove(const Cube& cube);
		void Add(uint x, uint y, uint z, uint color);
		void Add(Cube cube, bool setColor=false);
		void CheckSurroundingCubes(const Cube& cube);
		bool CubeVisible(uint x, uint y, uint z);
		bool CubeVisible(const Cube& cube);


		uint GetColor() const { return m_color; }
		void SetColor(uint color) { m_color = color; }
		GridRenderer3D* GetRenderer() { return renderer; }
		//Greet::ColorPicker* GetColorPicker() const { return m_colorPicker; };

		bool HasAdjacent() const { return hasAdjacent; }
		bool HasSelected() const { return hasSelected; }
		const Cube& GetAdjacentCube() const { return adjacent; }
		const Cube& GetSelectedCube() const { return selected; }

		void SaveModel(const std::string& filename);
		void LoadModel(const std::string& filename);
		void ExportModel(const std::string& filename);

    void WindowResize(Greet::WindowResizeEvent& event) override;



	};
}
