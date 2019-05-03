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
	quaternion<T>& conjugate();
	quaternion<T>& inv();
	bool isUnitary() const;
	void normalize();
	bool isValid() const {
		return (static_cast<int>(data.size()) == 4);
	}
	//begin and end for compatibility with STL:
	//get methods:
	T x(){
		return data[0];
	}
	T y(){
		return data[1];
	}
	T z(){
		return data[2];
	} 
	T w(){
		return data[3];
	}
	//set methods:

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
quaternion<T> operator*(const quaternion<T> & a, const quaternion<T> & b);

template<typename T>
quaternion<T> operator/(const quaternion<T> & a, const quaternion<T> & b);
