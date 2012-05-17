#ifndef POINT3D_H
#define POINT3D_H

#include <qglobal.h>
#include <qmath.h>

/**
 * Struct to store information of a point in 3D.
 */
struct Point3D
{
  /**
   * x.
   */
  double _x;

  /**
   * y.
   */
  double _y;

  /**
   * z.
   */
  double _z;

  /**
   * Constructor.
   */
  Point3D() : _x(0), _y(0), _z(0) {}

  /**
   * Constructor.
   *
   * @param x The x.
   * @param y The y.
   * @param z The z.
   */
  Point3D(double x, double y, double z) :
      _x(x), _y(y), _z(z) {}

  /**
   * Add another point.
   *
   * @param p Another point.
   * @return Result point.
   */
  Point3D operator+(const Point3D &p) const
  {
    return Point3D(*this) += p;
  }

  /**
   * Minus another point.
   *
   * @param p Another point.
   * @return Result point.
   */
  Point3D operator-(const Point3D &p) const
  {
    return Point3D(*this) -= p;
  }

  /**
   * Multiply a number.
   *
   * @param f The number.
   * @return Result point.
   */
  Point3D operator*(double f) const
  {
    return Point3D(*this) *= f;
  }

  /**
   * Add another point to this one.
   *
   * @param p Another point.
   * @return Result point.
   */
  Point3D &operator+=(const Point3D &p)
  {
    _x += p._x;
    _y += p._y;
    _z += p._z;
    return *this;
  }

  /**
   * Minus another point to this one.
   *
   * @param p Another point.
   * @return Result point.
   */
  Point3D &operator-=(const Point3D &p)
  {
    _x -= p._x;
    _y -= p._y;
    _z -= p._z;
    return *this;
  }

  /**
   * Multiply a number to this one.
   *
   * @param f The number.
   * @return Result point.
   */
  Point3D &operator*=(double f)
  {
    _x *= f;
    _y *= f;
    _z *= f;
    return *this;
  }

  /**
   * @param p Another point.
   * @return Whether this eqauls another one.
   */
  bool operator==(const Point3D &p)
  {
    if((_x == p._x) && (_y == p._y) && (_z == p._z))
      return true;
    return false;
  }

  /**
   * @return Radius of the point.
   */
  double radius() const
  {
    return qSqrt(_x * _x + _y * _y + _z * _z);
  }

  /**
   * @return Normalized point.
   */
  Point3D normalize() const
  {
    double r = 1.0 / radius();
    return Point3D(_x * r, _y * r, _z * r);
  }

  /**
   * @return The value user chooses.
   *
   * @param index The index user chooses.
   */
  double &operator[](unsigned int index)
  {
    Q_ASSERT(index < 3);
    return (&_x)[index];
  }

  /**
   * @return The value user chooses.
   *
   * @param index The index user chooses.
   */
  const double &operator[](unsigned int index) const
  {
    Q_ASSERT(index < 3);
    return (&_x)[index];
  }
};

inline double dot(const Point3D &a, const Point3D &b)
{
  return a._x * b._x + a._y * b._y + a._z * b._z;
}

inline Point3D cross(const Point3D &a, const Point3D &b)
{
  return Point3D(a._y * b._z - a._z * b._y,
                 a._z * b._x - a._x * b._z,
                 a._x * b._y - a._y * b._x);
}

#endif // POINT3D_H
