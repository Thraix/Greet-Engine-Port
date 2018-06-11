#pragma once

#include <graphics/models/MaterialModel.h>
namespace Greet {


	class EntityModel {
	private:
		MaterialModel* m_model;
	private:
		Vec3 m_position;
		Vec3 m_scale;
		Vec3 m_rotation;
		Mat4 m_transformationMatrix;
		bool m_hasChanged;
	public:
		EntityModel(MaterialModel* model, float x, float y, float z, float sx, float sy, float sz, float rx, float ry, float rz);
		EntityModel(MaterialModel* model, Vec3 position, Vec3 scale, Vec3 rotation);
		
		inline const MaterialModel& GetMaterialModel() const { return *m_model; }
		inline const Mat4& GetTransformationMatrix() const { return m_transformationMatrix; }
		void Update(float timeElapsed);
		void UpdateTransformation();
		void SetPosition(const Vec3& pos) { m_position = pos; m_hasChanged = true; }
		void SetScale(const Vec3& scale) { m_scale = scale; m_hasChanged = true; }
		void SetRotation(const Vec3& rotation) { m_rotation = rotation; m_hasChanged = true; }
		void Move(float dx, float dy, float dz) { m_position.x += dx;m_position.y = dy; m_position.z = dz; m_hasChanged = true; }
		void Scale(float x, float y, float z) { m_scale.x += x;m_scale.y += y; m_scale.z += z; m_hasChanged = true; }
		void Rotate(float x, float y, float z) { m_rotation.x += x;m_rotation.y += y; m_rotation.z += z; m_hasChanged = true; }
	};
}
