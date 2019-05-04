#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <initializer_list>
#include <cmath>
#include <ostream>
#include <fstream>
#include <iterator>
#include "matrix.hpp"
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
	std::vector<T> data;
	public:
	quaternion(): data{{static_cast<T>(0.), static_cast<T>(0.), static_cast<T>(0.), static_cast<T>(0.)}} {} //default const 
    /**
	*  Constructor
    */
	quaternion(std::vector<T> vec): data{vec}{} //init. list from vector 
	quaternion(T _s, T _v1, T _v2, T _v3): data{{_s, _v1, _v2, _v3}}{} //init. list from 4 numbers
	quaternion( quaternion const& ) = default; //copy const       

	
	quaternion<T>& operator=(quaternion const&) = default;

	//begin and end for compatibility with STL:
	//get methods:
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
	std::vector<T> vectorPart(){
		return {*this.x(), *this.y(), *this.z()};
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
	
	Matrix3<T> convertToMatrix() const {
		auto q = *this;
		T a11 = -1. + 2*sq(q.x()) + 2*sq(q.w()); T a12 = 2*(q.x()*q.y() - q.z()*q.w());   T a13 = 2*(q.x()*q.z() + q.y()*q.w());
		T a21 = 2*(q.x()*q.y() + q.z()*q.w());   T a22 = -1. + 2*sq(q.y()) + 2*sq(q.w()); T a23 = 2*(q.y()*q.z() - q.x()*q.w()); 
		T a31 = 2*(q.x()*q.z() - q.y()*q.w());   T a32 = 2*(q.x()*q.w() + q.y()*q.z());   T a33 = -1. + 2*sq(q.z()) + 2*sq(q.w());
		return {a11, a12, a13, a21, a22, a23, a31, a32, a33};
	}


	quaternion<T> inv() const {
		return {this->w(), -this->x(), -this->y(), -this->z()};
	}

	double norm() const { 
		return std::sqrt(std::inner_product(data.begin(), data.end(), data.begin(), 0.));
	}
	
	bool isRotation() const {
		return std::abs(this->norm() - 1.) < 1e-15;
	}
	//Rotation by a quaternion: 
	std::optional<std::vector<T>> operator()(const std::vector<T> & );
};

template<typename T>
quaternion<T> operator+( const quaternion<T> & a, const quaternion<T> & b){
	quaternion<T> result ();
	detail::transform_quaternion2(a, b, result, add);
	return result;
	}
template<typename T>
quaternion<T> operator-(const quaternion<T> & a, const quaternion<T> & b){
	quaternion<T> result ();
	detail::transform_quaternion2(a, b, result, sub);
	return result;
	}
template<typename T>
quaternion<T> operator-(const quaternion<T> & a){//Unitary negation
	quaternion<T> result ();
	detail::transform_quaternion1(a, result, [](T x){return -x;});
	return result;
} 
//scalar
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

