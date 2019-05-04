#pragma once
#include <vector>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <initializer_list>
#include <cmath>
#include <iterator>
#include <optional>
#include "quaternion.hpp"
//Helper functions:
namespace detail
{
	template<typename V1, typename V2, typename F>
	void transform_matrix1(V1 const& v1, V2& v2, F f)
	{
		std::transform(v1.cbegin(), v1.cend(), v2.begin(), f);
	}
	template<typename V1, typename V2, typename F>
	void transform_matrix2(V1 const& v1, V2& v2, F f)
	{
		std::transform(v1.cbegin(), v1.cend(), v2.begin(), f);
	}
}

template<typename T>
class Matrix3{
	private: 
	int N = 3;
	std::vector<T> data;
	public:
    /**
	*  Constructor
    */
	Matrix3():data(9){};
	Matrix3(std::vector<T> vec): data{vec}{}; //init. list from vector 
    Matrix3(std::initializer_list<T> const& il):   data{il}{};
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
	// computes the inverse of a matrix m
		auto m = *this;
		T det = m(0, 0) * (m(1, 1) * m(2, 2) - m(2, 1) * m(1, 2)) -
             m(0, 1) * (m(1, 0) * m(2, 2) - m(1, 2) * m(2, 0)) +
             m(0, 2) * (m(1, 0) * m(2, 1) - m(1, 1) * m(2, 0));

		return det;
	}
	bool isRotation() const {
		return (size() == 9 and static_cast<int>(determinant()) == 1);  //3x3 rotation matrix
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
std::optional<std::vector<T>> operator*(const Matrix3<T> &M, const std::vector<T> &v){
	if(!M.isRotation()){
		return std::nullopt;
	}
	else{
		std::vector<T> result(3);
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
