#pragma once

#include <math/Maths.h>
//#include <graphics/models/EntityModel.h>
//#include <graphics/renderers/Renderer3D.h>

namespace Greet {
	class Camera {

	public:
		virtual const Vec3& GetRotationVector() const = 0;
		virtual const Mat4& GetViewMatrix() const = 0;
		virtual void Update(float timeElapsed) {};
	};


}