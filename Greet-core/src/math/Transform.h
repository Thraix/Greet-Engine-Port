#pragma once

#include <math/Mat3.h>

namespace Greet {
  class Transform2D
  {
    public:
      Mat3 m_matrix;
    private:

    public:
      Transform2D()
        : m_matrix(Mat3(1))
      {

      }

      Transform2D(Mat3 matrix)

      {
        m_matrix = matrix;
      }

      Transform2D& Init(float diagonal = 1)
      {
        m_matrix = Mat3(diagonal);
        return *this;
      }

      Transform2D& Scale(float x, float y)
      {
        m_matrix *= Mat3::Scale(x, y);
        return *this;
      }

      Transform2D& Scale(const Vec2f& scale)
      {
        m_matrix *= Mat3::Scale(scale);
        return *this;
      }

      Transform2D& Translate(float x, float y)
      {
        m_matrix *= Mat3::Translate(x,y);
        return *this;
      }

      Transform2D& Translate(const Vec2f& translate)
      {
        m_matrix *= Mat3::Translate(translate);
        return *this;
      }

      Transform2D& Rotate(float rad)
      {
        m_matrix *= Mat3::Rotate(rad);
        return *this;
      }

      Transform2D& Shear(float x, float y)
      {
        m_matrix *= Mat3::Shear(x, y);
        return *this;
      }

      Transform2D& Shear(const Vec2f& shear)
      {
        m_matrix *= Mat3::Shear(shear);
        return *this;
      }

      inline const Mat3& GetMatrix() const { return m_matrix; }
  };
}
