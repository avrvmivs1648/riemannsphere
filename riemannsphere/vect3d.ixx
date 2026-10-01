module;
#include<cmath>

export module vect3d;

export namespace vect3d {
	class vect3d {
		long double _x;
		long double _y;
		long double _z;
	public:
		vect3d(long double x, long double y, long double z) :_x(x), _y(y), _z(z) {}
		long double x() {
			return _x;
		}
		long double y() {
			return _y;
		}
		long double z() {
			return _z;
		}
		void setv(long double x, long double y, long double z) {
			_x = x;
			_y = y;
			_z = z;
		}
		long double operator*(vect3d v) {
			return this->_x * v._x + this->_y * v._y + this->_z * v._z;
		}
		vect3d operator*(long double a) {
			return vect3d(a * this->_x, a * this->_y, a * this->_z);
		}
		long double length() {
			return sqrt((*this)*(*this));
		}
		vect3d operator+(vect3d v) {
			return vect3d(this->_x + v._x, this->_y + v._y, this->_z + v._z);
		}
		vect3d operator-(vect3d v) {
			return vect3d(this->_x - v._x, this->_y - v._y, this->_z - v._z);
		}
		vect3d operator/(long double a) {
			return vect3d(this->_x / a, this->_y / a, this->_z / a);
		}
		
		void normalize() {
			*this=*this / this->length();
			*this=*this* (2.0 - this->length());
		}
		vect3d& operator=(const vect3d& v) {
			_x = v._x;
			_y = v._y;
			_z = v._z;
			return *this;
		}
	};

	vect3d cross(vect3d v1, vect3d v2) {
		return vect3d(v1.y() * v2.z() - v1.z() * v2.y(), v1.z() * v2.x() - v1.x() * v2.z(), v1.x() * v2.y() - v1.y() * v2.x());
	}
}