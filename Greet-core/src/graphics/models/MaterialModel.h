#pragma once

#include <graphics/models/Material.h>
#include <graphics/models/Mesh.h>

namespace Greet {

  class Renderer3D;

	class MaterialModel
	{
	private:
		static uint s_current_id;
		const Mesh* m_mesh;
		const Material* m_material;
		uint m_id;
	public:
		MaterialModel(const Mesh* mesh, const Material* material);
		virtual ~MaterialModel();

    void PreRender(const Renderer3D* renderer, const Mat4& transformationMatrix) const;
    void Render(const Renderer3D* renderer, const Mat4& transformationMatrix) const;
    void PostRender(const Renderer3D* renderer, const Mat4& transformationMatrix) const;

    void PreRender(const Renderer3D* renderer) const;
    void Render(const Renderer3D* renderer) const;
    void PostRender(const Renderer3D* renderer) const;

		inline const Material& GetMaterial() const { return *m_material; }
		inline const Mesh& GetMesh() const { return *m_mesh; }
		friend bool operator < (const MaterialModel & lhs, const MaterialModel & rhs)
		{
			uint idl = lhs.GetMaterial().GetTexture().GetTexId();
			uint idr = rhs.GetMaterial().GetTexture().GetTexId();
			if (idl < idr) { return true; }
			if (idl > idr) { return false; }
			return (idl < idr);
		}

		inline uint GetId() const { return m_id; }

		bool operator==(const MaterialModel& second) const;
	};
}
