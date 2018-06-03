#include "TPCamera.h"

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
		: TPCamera(position, distance, height, rotation, distanceMin, distanceMax, heightMin, heightMax, 0.5f, 0.01f, 0.005f)
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
		EventDispatcher::AddMouseListener(0, *this);
	}

	TPCamera::~TPCamera()
	{
		EventDispatcher::RemoveMouseListener(*this);
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
			Log::Info("Distance outside of clamp, reclamping.");
			m_distanceWanted = m_distanceMin;
		}
		else if (m_distanceWanted > m_distanceMax)
		{
			Log::Info("Distance outside of clamp, reclamping.");
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

	bool TPCamera::OnPressed(const MousePressedEvent& e){
		if (e.GetButton() == GLFW_MOUSE_BUTTON_1)
		{
			m_mouse1 = true;
			//Mat4 inv = ~m_viewMatrix;
			//Vec3 worldCoord = inv * Vec3(e.getX(), e.getY(), 1.0f);
			//Log::info(worldCoord.x," ", worldCoord.y);
		}
		else if (e.GetButton() == GLFW_MOUSE_BUTTON_3)
		{
			m_mouse3 = true;
		}
		else if (e.GetButton() == GLFW_MOUSE_BUTTON_2)
		{
			m_mouse2 = true;
		}
		return false;
	}

	void TPCamera::OnReleased(const MouseReleasedEvent& e)
	{
		if (e.GetButton() == GLFW_MOUSE_BUTTON_1)
		{
			m_mouse1 = false;
		}
		else if (e.GetButton() == GLFW_MOUSE_BUTTON_2)
		{
			m_mouse2 = false;
		}
		else if (e.GetButton() == GLFW_MOUSE_BUTTON_3)
		{
			m_mouse3 = false;
		}
	}

	void TPCamera::OnMoved(const MouseMovedEvent& e) {
		if (m_mouse3) {
			m_heightWanted += e.GetDY() * m_heightSpeed;
			Math::Clamp(&m_heightWanted, m_heightMin, m_heightMax);
			m_rotationWanted += m_rotationSpeed * e.GetDX();
		}
		if (m_mouse2)
		{
			m_positionWanted.y += e.GetDeltaPosition().y * 0.1f;
		}
		else if (m_mouse1)
		{
			Vec2 dpos = e.GetDeltaPosition();
			dpos.Rotate(m_rotation);
			m_positionWanted.x += dpos.y * m_distanceSpeed * m_distance;
			m_positionWanted.z -= dpos.x * m_distanceSpeed * m_distance;
		}
	}

	void TPCamera::OnScroll(const MouseScrollEvent& e) {
		m_distanceWanted -= e.GetScroll();
		Math::Clamp(&m_distanceWanted, m_distanceMin, m_distanceMax);
	}

}