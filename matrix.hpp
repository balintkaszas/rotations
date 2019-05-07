#pragma once
#include <array>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <initializer_list>
#include <cmath>
#include <optional>
#include "quaternion.hpp"
#include "axisAngle.hpp"


template<typename T>
class Matrix3{
	private: 
	int N = 3;
	std::array<T,9> data;
	public:
    /**
	*  Constructor
    */
	Matrix3():data{}{};
	Matrix3(std::array<T,9> vec): data{vec}{}; //1 vector
	Matrix3( Matrix3 const& ) = default; //copy const       
	
	T& operator[]( int i ) { 
		return data[i];
	}
    T const& operator[]( int i ) const { //read only
		return data[i];
	}
    T& operator()(int i, int j) {
		return data[N*i+j];
	}
    T const& operator()(int i, int j) const { //read only 
 		return data[N*i+j]; 
	}
	
	Matrix3<T>& operator=(Matrix3 const&) = default;
	int size() const {
		return static_cast<int>(data.size());
	}
	T determinant() const {
		//Determinant computed from components
		auto m = *this;
		T det = m(0, 0) * (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) -
             m(0, 1) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
             m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0));

		return det;
	}
	bool isRotation() const {
		return (std::abs(determinant() - 1.) < 1e-6);  //3x3 rotation matrix
	}

	//Conversion functions: 
	std::optional<quaternion<T>> convertToQuaternion() const {
		if(isRotation()) {
			return std::nullopt; //Not a rotation matrix
		}
		else{
			auto m = *this;
			T x = 0.5 * std::sqrt(1 + m(0, 0) - m(1, 1) - m(2, 2));
			T y = (m(0, 1) + m(1, 0)) / (4 * x);
			T z = (m(0, 2) + m(2, 0)) / (4 * x);
			T w = (m(2, 1) - m(1, 2)) / (4 * x);
			quaternion<T> result {w, x, y, z};
			return result;
		}
	}
	//converting to axis-angle representation:
	std::optional<axisAngle<T>> convertToAxisAngle() const {
		if(!isRotation()) {
			return std::nullopt; //Not a rotation matrix
		}
		else{
			auto m = *this;
			std::array<T,3> axis {m(2,1) - m(1, 2), m(0, 2) - m(2, 0), m(1, 0) - m(0, 1)}; //the lenght of this is 2*sin(\alpha)
			T s2 = std::sqrt(std::inner_product(axis.begin(), axis.end(), axis.begin(), 0.));

			std::transform(axis.begin(), axis.end(), axis.begin(), [s2](T x){return x / s2;}); //Normed axis
			T angle = std::asin(s2/2.);
			axisAngle<T> result(axis, angle);
			return result;
		}
	}


	//STL compatibility
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

};
//Rotating a vector: 
template<typename T>
std::optional<std::array<T,3>> operator*(const Matrix3<T> &M, const std::array<T,3> &v){
	if(!M.isRotation()){
		return std::nullopt;
	}
	else{
		std::array<T,3> result;
		for(int i = 0; i < 3; ++i){
			T sum = 0.0;
			for(int j = 0; j < 3; ++j){
				sum += M(i,j)*v[j];
			}
			result[i] = sum;
		}
		return result;
	}
}


//Matrix multiplication
template<typename T>
Matrix3<T> operator*( Matrix3<T> const& m1, Matrix3<T> const& m2 ) { 
    Matrix3<T> result;
        for(int i=0;i<3;i++) {
            for(int j=0;j<3;j++) {
                T sum = 0.0;
                for(int k=0;k<3;k++) {
                    sum += m1(i,k)*m2(k,j);
                }
                result(i,j) = sum;
        	}
		}
	return result;
}
