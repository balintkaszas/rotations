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
	void transform_matrix1(V1 const& v1, V2& v2, F f)
	{
		std::transform(v1.cbegin(), v1.cend(), v2.begin(), f);
	}

	template<typename V1, typename V2, typename V3, typename F>
	void transform_matrix2(V1 const& v1, V2 const& v2, V3& v3, F f)
	{
		std::transform(v1.cbegin(), v1.cend(), v2.cbegin(), v3.begin(), f);
	}
}

//Common lambdas:
inline auto add = [](auto const& x, auto const& y){ return x + y; };
inline auto sub = [](auto const& x, auto const& y){ return x - y; };
inline auto sq = [](auto const& x){ return x * x  ; };


template<typename T>
class quaternion{
	private: 
	int s;
	std::vector<T> vec;
	public:
	quaternion(): s{},vec{} {} //default const 
    /**
	*  Constructor
    */
	quaternion(T _s, std::vector<T> _v): s{_s},vec{_v}{} //init. list 1
	quaternion(T _s, T _v1, T _v2, T _v3): s{_s},vec{{_v1, _v2, _v3}}{} //init. list 2
	quaternion( quaternion const& ) = default; //copy const       

	
	quaternion<T>& operator=(quaternion const&) = default;
	quaternion<T>& conjugate();
	quaternion<T>& inv();
	bool isUnitary();
	void normalize();

};

template<typename T>
quaternion<T> operator+( const quaternion<T> & a, const quaternion<T> & b);
template<typename T>
quaternion<T> operator-(const quaternion<T> & a);
template<typename T>
quaternion<T> operator-( const quaternion<T> & a, const quaternion<T> & b);
//scalar
template<typename T>
quaternion<T> operator*( T s, const quaternion<T> & a);

template<typename T>
quaternion<T> operator*( const quaternion<T> & a, T s);

template<typename T>
quaternion<T> operator/( const quaternion<T> & a, T s);


template<typename T>
quaternion<T> operator*(const quaternion<T> & a, const quaternion<T> & b);

template<typename T>
quaternion<T> operator/(const quaternion<T> & a, const quaternion<T> & b);
