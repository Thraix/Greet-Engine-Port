#pragma once

#include <graphics/models/MaterialModel.h>
namespace Greet {

  class Renderer3D;

	class EntityModel {
	private:
		const MaterialModel* m_model;
	private:
		Vec3 m_position;
		Vec3 m_scale;
		Vec3 m_rotation;
		Mat4 m_transformationMatrix;
		bool m_hasChanged;
	public:
		EntityModel(const MaterialModel* model, float x, float y, float z, float sx, float sy, float sz, float rx, float ry, float rz);
		EntityModel(const MaterialModel* model, Vec3 position, Vec3 scale, Vec3 rotation);
    EntityModel(const MaterialModel* model);

    void PreRender(const Renderer3D* renderer) const;
    void Render(const Renderer3D* renderer) const;
    void PostRender(const Renderer3D* renderer) const;
		
		inline const MaterialModel& GetMaterialModel() const { return *m_model; }
    void SetMaterialModel(const MaterialModel* model) { m_model = model;}
		inline const Mat4& GetTransformationMatrix() const { return m_transformationMatrix; }
		virtual void Update(float timeElapsed);
		void UpdateTransformation();
		void SetPosition(const Vec3& pos) { m_position = pos; m_hasChanged = true; }
		void SetScale(const Vec3& scale) { m_scale = scale; m_hasChanged = true; }
		void SetRotation(const Vec3& rotation) { m_rotation = rotation; m_hasChanged = true; }
		void Move(float dx, float dy, float dz) { m_position.x += dx;m_position.y = dy; m_position.z = dz; m_hasChanged = true; }
		void Scale(float x, float y, float z) { m_scale.x += x;m_scale.y += y; m_scale.z += z; m_hasChanged = true; }
		void Rotate(float x, float y, float z) { m_rotation.x += x;m_rotation.y += y; m_rotation.z += z; m_hasChanged = true; }

    const Vec3& GetPosition() const { return m_position;}
    const Vec3& GetScale() const { return m_scale;}
    const Vec3& GetRotation() const { return m_rotation;}
	};
}
