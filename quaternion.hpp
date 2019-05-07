#pragma once
#include <array>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <initializer_list>
#include <cmath>
#include "matrix.hpp"
#include "axisAngle.hpp"
//Helper functions:
namespace detail
{
	template<typename V1, typename V2, typename F>
	void transform_quaternion1(V1 const& v1, V2& v2, F f)
	{
		std::transform(v1.cbegin(), v1.cend(), v2.begin(), f);
	}
	template<typename V1, typename V2, typename F>
	void transform_quaternion2(V1 const& v1, V2& v2, F f)
	{
		std::transform(v1.cbegin(), v1.cend(), v2.begin(), f);
	}
}

//Common lambdas:
inline auto add = [](auto const& x, auto const& y){ return x + y; };
inline auto sub = [](auto const& x, auto const& y){ return x - y; };
inline auto sq = [](auto const& x){ return x * x  ; };

template<typename T> //forward declaration
class Matrix3;

template<typename T>
class quaternion{
	private: 
	std::array<T,4> data;
	public:
	quaternion(): data{{static_cast<T>(0.), static_cast<T>(0.), static_cast<T>(0.), static_cast<T>(0.)}} {} //default const 
    /**
	*  Constructor
    */
	quaternion(std::array<T,3> arr): data{arr}{} //init. list from vector 
	quaternion(T _s, T _v1, T _v2, T _v3): data{{_s, _v1, _v2, _v3}}{} //init. list from 4 numbers

	quaternion(T _s, std::array<T,3> _v): data{{_s, _v[0], _v[1], _v[2]}}{} //init. list from scalar + vector
	quaternion( quaternion const& ) = default; //copy const       

	
	quaternion<T>& operator=(quaternion const&) = default;

	//begin and end for compatibility with STL:
	//get components:
	T x() const {
		return data[1];
	}
	T y() const {
		return data[2];
	}
	T z() const {
		return data[3];
	} 
	T w() const {
		return data[0];
	}
	// Get vector part 
	std::array<T,3> vectorPart() const{
		return {x(), y(), z()};
	}

	auto begin() {
		return data.begin();
	}
	auto cbegin() const {
		return data.cbegin();
	}

	auto end() {
		return data.end();
	}

	auto cend() const {
		return data.cend();
	}
	//Conversion functions: (matrix, axis-angle)
	//Matrix conversion is always valid, but the result is not necessarily a rotation matrix
	Matrix3<T> convertToMatrix() const {
		T a11 = -1. + 2*sq(x()) + 2*sq(w()); T a12 = 2*(x()*y() - z()*w());       T a13 = 2*(x()*z() + y()*w());
		T a21 = 2*(x()*y() + z()*w());       T a22 = -1. + 2*sq(y()) + 2*sq(w()); T a23 = 2*(y()*z() - x()*w()); 
		T a31 = 2*(x()*z() - y()*w());       T a32 = 2*(x()*w() + y()*z());       T a33 = -1. + 2*sq(z()) + 2*sq(w());
		Matrix3<T> result({a11, a12, a13, a21, a22, a23, a31, a32, a33});
		return result;
	}

	//conversion to axis-angle representation is only valid when the quaternion is unitary
	//for numerical stability, atan2 function is used
	std::optional<axisAngle<T>> convertToAxisAngle() const {
		if(!isRotation()){
			return std::nullopt;
		}
		else{
			std::array<T,3> axis;
			T alpha = 2*std::atan2(1, w());
			if(alpha == 0){
				axis = {0., 0., 0.};
			}else{
				T s = std::sin(alpha/2.);
				axis = {x()/s, y()/s, z()/s};
			}
			axisAngle<T> result {axis, alpha};
			return result;
		}
	}
	
	quaternion<T> inv() const {
		return {w(), -x(), -y(), -z()};
	}

	double norm() const { 
		return std::sqrt(std::inner_product(data.begin(), data.end(), data.begin(), 0.));
	}
	
	bool isRotation() const {
		return (std::abs(norm() - 1.) < 1e-6);
	}
};

//scalar multiply, to normalize quaternion
template<typename T>
quaternion<T> operator*( T s, const quaternion<T> & a){
	quaternion<T> result ();
	detail::transform_quaternion1(a, result, [s](T x){return s*x;});
	return result;
}
template<typename T>
quaternion<T> operator*( const quaternion<T> & a, T s){
	quaternion<T> result ();
	detail::transform_quaternion1(a, result, [s](T x){return x*s;});
	return result;
}
template<typename T>
quaternion<T> operator/( const quaternion<T> & a, T s){
	quaternion<T> result ();
	detail::transform_quaternion1(a, result, [s](T x){return x/s;});
	return result;
}
template<typename T>
quaternion<T> operator*(const quaternion<T> & a, const quaternion<T> & b){
	T tw = a.w()*b.w() - a.x()*b.x() - a.y()*b.y() - a.z()*b.z();
	T tx = a.w()*b.x() + a.x()*b.w() + a.y()*b.z() - a.z()*b.y();
	T ty = a.w()*b.y() - a.x()*b.z() + a.y()*b.w() + a.z()*b.x();
	T tz = a.w()*b.z() + a.x()*b.y() - a.y()*b.x() + a.z()*b.w();
	return {tw, tx, ty, tz};
}

template<typename T>
std::optional<std::array<T,3>> rotateByQuaternion(const quaternion<T> &q, const std::array<T,3> &r) {
	if(!q.isRotation()){
		return std::nullopt;
	}
	else {
		quaternion<T> Rquat(0., r); 
		quaternion<T> RquatPrime = q*Rquat*q.inv();
		return RquatPrime.vectorPart();
	}
}