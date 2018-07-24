#include "TPCamera.h"

#include <graphics/Window.h>

namespace Greet {

	TPCamera::TPCamera()
		: TPCamera(Vec3(0,0,0), 1, 0, 0)
	{
	}

	TPCamera::TPCamera(Vec3 position, float distance, float height, float rotation)
		:TPCamera(position, distance, height, rotation, 1, 100, -1,1)
	{
	
	}

	TPCamera::TPCamera(Vec3 position, float distance, float height, float rotation, float distanceMin, float distanceMax, float heightMin, float heightMax)
		: TPCamera(position, distance, height, rotation, distanceMin, distanceMax, heightMin, heightMax, 1000, 10, 10)
	{
	}

	TPCamera::TPCamera(Vec3 position, float distance, float height, float rotation, float distanceMin, float distanceMax, float heightMin, float heightMax, float rotationSpeed, float heightSpeed, float distanceSpeed) 
		: m_position(position), m_distance(distance), m_height(height), m_rotation(rotation), m_rotationSpeed(rotationSpeed), m_heightSpeed(heightSpeed), m_distanceSpeed(distanceSpeed)
	{
		m_rotationWanted = m_rotation;
		m_distanceWanted = m_distance;
		m_heightWanted = m_height;
		m_positionWanted = m_position;
		m_rotationVector.z = 0;
		SetDistanceClamp(distanceMin, distanceMax);
		SetHeightClamp(heightMin, heightMax);
		CalculateInformation();

    // Default input controls
    inputCameraRotate = "mouseMiddle";
    inputCameraZoom = "mouseScroll";
    inputCameraXZ = "mouseLeft";
    inputCameraY = "mouseRight";
    inputCameraMove = "mousePos";
	}

	TPCamera::~TPCamera()
	{
	}

	void TPCamera::Update(float timeElapsed)
	{
		float factor1 = (timeElapsed * 10.0f); // 0.33 ish
		float factor2 = (timeElapsed * 30.0f);
		m_rotation += (m_rotationWanted - m_rotation) * factor1;
		m_position += (m_positionWanted - m_position) * factor2;
		m_distance += (m_distanceWanted - m_distance) * factor1;
		m_height += (m_heightWanted - m_height) * factor2;
		CalculateInformation();
	}

	const Vec3& TPCamera::GetRotationVector() const
	{ 
		return m_rotationVector;
	}

	const Mat4& TPCamera::GetViewMatrix() const
	{ 
		return m_viewMatrix;
	}

	void TPCamera::SetPosition(Vec3 pos)
	{
		m_positionWanted = pos;
	}

	void TPCamera::SetHeight(float height)
	{
		m_heightWanted = height;
		Math::Clamp(&m_heightWanted, m_heightMin, m_heightMax);
		if (m_heightWanted != height)
		{
			Log::Warning("Height outside of clamp, clamping.");
		}
	}

	void TPCamera::SetRotation(float rotation)
	{
		//m_rotation = rotation;
		m_rotationWanted = rotation; // smooth transition
	}

	void TPCamera::SetDistance(float distance)
	{
		m_distanceWanted = distance;
		Math::Clamp(&m_distanceWanted, m_distanceMin, m_distanceMax);
		if (m_distanceWanted != distance)
		{
			Log::Warning("Distance outside of clamp, clamping.");
		}
	}

	void TPCamera::SetDistanceClamp(float min, float max)
	{
		if (min > max)
		{
			Log::Error("Distance clamp: min greater than max.");
			return;
		}
		m_distanceMin = min < 0 ? 0 : min;
		m_distanceMax = max < 0 ? 0 : max;
		if (m_distanceWanted < m_distanceMin)
		{
			Log::Warning("Distance outside of clamp, reclamping.");
			m_distanceWanted = m_distanceMin;
		}
		else if (m_distanceWanted > m_distanceMax)
		{
			Log::Warning("Distance outside of clamp, reclamping.");
			m_distanceWanted = m_distanceMax;
		}
	}

	void TPCamera::SetHeightClamp(float min, float max)
	{
		if (min > max)
		{
			Log::Error("Height clamp: min greater than max.");
			return;
		}
		Math::Clamp(&min, -1, 1);
		Math::Clamp(&max, -1, 1);
		m_heightMin = min;
		m_heightMax = max;
		if (m_heightWanted < m_heightMin)
		{
			Log::Warning("Height outside of clamp, reclamping.");
			m_heightWanted = m_heightMin;
		}
		else if (m_heightWanted > m_heightMax)
		{
			Log::Warning("Height outside of clamp, reclamping.");
			m_heightWanted = m_heightMax;
		}
	}

	void TPCamera::CalculateRotationVector()
	{
		m_rotationVector.x = Math::ToDegrees(asin(m_height));
		m_rotationVector.y = m_rotation;
	}

	void TPCamera::CalculateViewMatrix()
	{
		m_viewMatrix = Mat4::TPCamera(m_position, m_distance, m_height, m_rotation);
	}

	void TPCamera::CalculateInformation()
	{
		CalculateRotationVector();
		CalculateViewMatrix();
	}

	void TPCamera::Move(const Vec2& delta) {
		if (m_mouse3) {
			m_heightWanted -= delta.y * m_heightSpeed;
			Math::Clamp(&m_heightWanted, m_heightMin, m_heightMax);
			m_rotationWanted += m_rotationSpeed * delta.x;
		}
		if (m_mouse2)
		{
			m_positionWanted.y += delta.y * 0.1f;
		}
		else if (m_mouse1)
		{
			Vec2 deltaR = Vec2(delta).Rotate(-m_rotation);
			m_positionWanted.x -= deltaR.y * m_distanceSpeed * m_distance * (Window::GetHeight()/(float)Window::GetWidth());
			m_positionWanted.z -= deltaR.x * m_distanceSpeed * m_distance;
		}
	}

  void TPCamera::Zoom(float delta)
  {
		m_distanceWanted -= delta;
		Math::Clamp(&m_distanceWanted, m_distanceMin, m_distanceMax);
  }

  InputControlRequest TPCamera::OnInputChanged(const InputControl* control)
  {
    if(control->GetName() == inputCameraXZ) 
    {
      m_mouse1 = control->GetValue(0) > 0.5;
    }
    else if(control->GetName() == inputCameraY)
    {
      m_mouse2 = control->GetValue(0) > 0.5;
    }
    else if(control->GetName() == inputCameraRotate)
    {
      m_mouse3 = control->GetValue(0) > 0.5;
    }
    else if(control->GetName() == inputCameraMove)
    {
      Move(control->GetDeltaAsVec2());
    }
    else if(control->GetName() == inputCameraZoom)
    {
      Zoom(control->GetDelta(0));
    }
  
    return m_mouse1 | m_mouse2 | m_mouse3 ? InputControlRequest::DONE : InputControlRequest::NOTHING;
  }

}
