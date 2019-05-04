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
#include "quaternion.hpp"

template<typename T>
class Matrix3;
template<typename T>
class quaternion;

template<typename T>
class axisAngle{
	private: 
	std::vector<T> axis; // axis coordinates
	T angle;
	public:
	axisAngle(): axis(3), angle{0}{} //default const 
    /**
	*  Constructor
    */
	axisAngle(std::vector<T> vec, T a): axis{vec}, angle{a} {}; //init.  from vector + angle 
	axisAngle(T _v1, T _v2, T _v3, T _v4): axis{{_v1, _v2, _v3}}, angle{_v4} {}; //init.  from 4 numbers
	axisAngle(std::initializer_list<T> const& il, T a):   axis{il}, angle{a} {}; //init from init list + number
	
	axisAngle( axisAngle const& ) = default; //copy const       

	
	axisAngle<T>& operator=(axisAngle const&) = default;

	T x() const {
		return axis[0];
	}
	T y() const {
		return axis[1];
	}
	T z() const {
		return axis[2];
	} 
	T getAngle() const {
		return angle;
	}

	bool isRotation() const {
		return (std::inner_product(axis.begin(), axis.end(), axis.begin(), 0.)!=0 and getAngle() != 0);  // if axis, angle is not zero.
	}

	//conversion functions: 

	std::optional<Matrix3<T>> convertToMatrix() const {
		if(!isRotation()){
			return std::nullopt;
		}
		else{

			T c = std::cos(getAngle());
			T s = std::sin(getAngle());
			T C = 1. - c;
			T a11 = x()*x()*C + c; T a12 = x()*y()*C - z()*s; T a13 = x()*z()*C + y()*s;
			T a21 = y()*x()*C + z()*s; T a22 = y()*y()*C + c; T a23 = y()*z()*C - x()*s;
			T a31 = z()*x()*C - y()*s; T a32 = z()*y()*C + x()*s; T a33 = z()*z()*C + c;
			Matrix3<T> result{a11, a12, a13, a21, a22, a23, a31, a32, a33}; 
			return result;
		}
	}

	std::optional<quaternion<T>> convertToQuaternion() const {
		if(!isRotation()){
			return std::nullopt;
		}
		else{
			T c = std::cos(getAngle()/2.);
			T s = std::sin(getAngle()/2.);
			quaternion<double> result  {c, x()*s, y()*s, z()*s}; 
			return result;
		}
	}


};