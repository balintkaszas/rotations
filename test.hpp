
#pragma once
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include "matrix.hpp"

template<typename T>
 bool areEqual(const std::vector<T> &reference, const  matrix<T> & m, const double precision = 1e-10) {
    
    return (m.size() == static_cast<int>( reference.size() )) and std::equal (m.cbegin(), m.cend(), reference.cbegin(), [=](const auto x, const auto y){return std::abs(x - y) < precision;}) ;
}  

 

void TestFunction() {
    int numErrors = 0;
    //Default constructor
    {
        matrix<int> m;
        if( m.size() != 0 or m.dimension() != 0 or m.begin() != m.end()) {
            numErrors++;
            std::cout << "Default constructor failed \n";
        }
    }

    // init. list constructor, indexing and iterators
    // if iterators tests are passed, areEqual() function is enough 
    {
		matrix<double> m(2,{1., 2., 3., 4.});
        if( m.size() != 4 or m.dimension() != 2) {
            numErrors++;
            std::cout << "initializer list constructor failed (size mismatch) \n";
        }
		if(m[0] != 1. or m[1] != 2. or m[2] != 3. or m[3] != 4.) {
            numErrors++;
            std::cout << "initializer list constructor failed (1d indexing) \n";
        }
		if(m(0,0) != 1. or m(0,1) != 2. or m(1,0) != 3. or m(1,1) != 4.) {
            numErrors++;
            std::cout << "initializer list constructor failed (2d indexing) \n";
        }
        auto itWrite = m.begin();
        auto itRead = m.cbegin();
        for(int j = 1; j < 5 ; ++j){
            if(*itWrite != j or *itRead != j) {
                numErrors++;
                std::cout << "Iterator failed " << *itWrite << " " << j << "\n";
            } 
            std::advance(itWrite, 1);
            std::advance(itRead, 1);
        }
    }

    // Testing with std::equal
    {
        matrix <double> m(2,{1., 2., 3., 4.});
        if(!areEqual({1., 2., 3., 4.}, m) ){
            numErrors ++;
            std::cout << "std::equal failed \n";
        }

    }
    //Empty constructor
    {
        matrix<double> m(2);
        if( m.size() != 4 or !areEqual({0., 0., 0., 0.}, m)) {
            numErrors++;
            std::cout << "Empty constructor failed \n";
        }
    }
    //SetSize
    {
        matrix<double> m;
        m.setDim(3);
        if( m.size() != 9 or m.dimension() != 3) {
            numErrors++;
            std::cout << "SetDim failed \n";
        }
    }
    //1d constructor
    {
        matrix<double> m(matrix<double>::Idx1{}, 3, [](const auto x){return 2*x;});
        if( m.size() != 9 or !areEqual({0., 2., 4., 6., 8., 10., 12., 14., 16.}, m)) {
            numErrors++;
            std::cout << "1D function constructor failed \n";
        }
    }

    //2d constructor
    {
        matrix<double> m(matrix<double>::Idx2{}, 3, [](const auto x, const auto y){return 2*x+y;});
        if( m.size() != 9 or !areEqual({0., 1., 2., 2., 3., 4., 4., 5., 6.}, m)) {
            numErrors++;
            std::cout << "2D function constructor failed \n";
        }
    }
    //copy constructor
	{
		matrix<double> a(3,{0., 1., 2., 3., 4., 5., 6., 7., 8. });
		matrix<double> b{a};
        if(a.size() != b.size() or !areEqual({0., 1., 2., 3., 4., 5., 6., 7., 8.}, b)) {
            numErrors++;
            std::cout << "Copy constructor failed \n";      
        }
    }
    // move constructor 
    {
		matrix<double> a(3,{0., 1., 2., 3., 4., 5., 6., 7., 8.});
		matrix<double> b{std::move(a)};
        if(a.size() != 0 or b.size() != 9  or !areEqual({0., 1., 2., 3., 4., 5., 6., 7., 8. }, b)) {
            numErrors++;
            std::cout << "Move constructor failed \n";      
        }
    }

    // assign operators
    {
		matrix<double> a(3,{0., 1., 2., 3., 4., 5., 6., 7., 8.});
		matrix<double> b, c;
        a = a;
        if(a.size() != 9 or !areEqual({0., 1., 2., 3., 4., 5., 6., 7., 8. }, a)) {
            numErrors++;
            std::cout << "Self assignment failed \n";              
        }

        a = std::move(a);
        if(a.size() != 9 or !areEqual({0., 1., 2., 3., 4., 5., 6., 7., 8. }, a)) {
            numErrors++;
            std::cout << "Self move assignment failed \n";              
        } 
        b = a;
        if( b.size() != 9  or !areEqual({0., 1., 2., 3., 4., 5., 6., 7., 8. }, b)) {
            numErrors++;
            std::cout << "Assignment failed \n";      
        }
        c = std::move(a);
        if(a.size() != 0 or c.size() != 9  or !areEqual({0., 1., 2., 3., 4., 5., 6., 7., 8. }, c)) {
            numErrors++;
            std::cout << "Move assignment failed \n";      
        }
    }

    // +=, -=, *=, /= operators

    {
		matrix<double> a(3,{1. , 2. ,  3., 4.,  5.,  6., 7.,  8., 9.});
		matrix<double> b(3,{1., 1., 1. ,1., 1., 1., 1., 1., 1.});
		a += b;
        if(!areEqual({2., 3., 4., 5.,6., 7., 8., 9., 10.}, a)){
            numErrors++;
            std::cout << "+= operator failed \n";   
        }
	}

    {
		matrix<double> a(3,{1. , 2. ,  3., 4.,  5.,  6., 7.,  8., 9.});
		matrix<double> b(3,{1., 1., 1. ,1., 1., 1., 1., 1., 1.});
		a -= b;
        if(!areEqual({0., 1., 2., 3.,4., 5., 6., 7., 8.}, a)){
            numErrors++;
            std::cout << "-= operator failed \n";   
        }
	}

    {
		matrix<double> a(3,{1. , 2. ,  3., 4.,  5.,  6., 7.,  8., 9.});
		a *= 2.5;
        if(!areEqual({2.5, 5., 7.5, 10. ,12.5, 15., 17.5, 20., 22.5}, a)){
            numErrors++;
            std::cout << "*= operator failed \n";   
        }
	}

    {
		matrix<double> a(3,{2.5, 5., 7.5, 10. ,12.5, 15., 17.5, 20., 22.5});
		a /= 2.5;
        if(!areEqual({1. , 2. ,  3., 4.,  5.,  6., 7.,  8., 9.}, a)){
            numErrors++;
            std::cout << "/= operator failed \n";   
        }
	}
    // + operator
    // (const &, const&)
    {
		matrix<double> a(3,{1. , 2. ,  3., 4.,  5.,  6., 7.,  8., 9.});
		matrix<double> b(3,{1., 1., 1. ,1., 1., 1., 1., 1., 1.});

		matrix<double> c = a + b;
        if(!areEqual({2., 3., 4., 5.,6., 7., 8., 9., 10.}, c)){
            numErrors++;
            std::cout << "+ operator (const &, const&) failed \n";   
        }
	}
    // (&&, const&)

    {
		matrix<double> a(3,{1. , 2. ,  3., 4.,  5.,  6., 7.,  8., 9.});
		matrix<double> b(3,{1., 1., 1. ,1., 1., 1., 1., 1., 1.});

		matrix<double> c = std::move(a) + b;
        if(!areEqual({2., 3., 4., 5.,6., 7., 8., 9., 10.}, c)){
            numErrors++;
            std::cout << "+ operator (&&, const&) failed \n";   
        }
	}
    // (const&, &&) 
    {
		matrix<double> a(3,{1. , 2. ,  3., 4.,  5.,  6., 7.,  8., 9.});
		matrix<double> b(3,{1., 1., 1. ,1., 1., 1., 1., 1., 1.});

		matrix<double> c = a + std::move(b);
        if(!areEqual({2., 3., 4., 5.,6., 7., 8., 9., 10.}, c)){
            numErrors++;
            std::cout << "+ operator (const&, &&) failed \n";   
        }
	}

    // (&&, &&) 
    {
		matrix<double> a(3,{1. , 2. ,  3., 4.,  5.,  6., 7.,  8., 9.});
		matrix<double> b(3,{1., 1., 1. ,1., 1., 1., 1., 1., 1.});

		matrix<double> c = std::move(a) + std::move(b);
        if(!areEqual({2., 3., 4., 5.,6., 7., 8., 9., 10.}, c)){
            numErrors++;
            std::cout << "+ operator (&&, &&) failed \n";   
        }
	}

    // - operator
    // (const &, const&)

    {
		matrix<double> a(3,{1. , 2. ,  3., 4.,  5.,  6., 7.,  8., 9.});
		matrix<double> b(3,{1., 1., 1. ,1., 1., 1., 1., 1., 1.});

		matrix<double> c = a - b;
        if(!areEqual({0., 1., 2., 3.,4., 5., 6., 7., 8.}, c)){
            numErrors++;
            std::cout << "- operator (const &, const&) failed \n";   
        }
	}
    
    // (&&, const&)

    {
		matrix<double> a(3,{1. , 2. ,  3., 4.,  5.,  6., 7.,  8., 9.});
		matrix<double> b(3,{1., 1., 1. ,1., 1., 1., 1., 1., 1.});

		matrix<double> c = std::move(a) - b;
        if(!areEqual({0., 1., 2., 3.,4., 5., 6., 7., 8.}, c)){
            numErrors++;
            std::cout << "- operator (&&, const&) failed \n";   
        }
	}

    // (const&, &&)

    {
		matrix<double> a(3,{1. , 2. ,  3., 4.,  5.,  6., 7.,  8., 9.});
		matrix<double> b(3,{1., 1., 1. ,1., 1., 1., 1., 1., 1.});

		matrix<double> c = a - std::move(b);
        if(!areEqual({0., 1., 2., 3.,4., 5., 6., 7., 8.}, c)){
            numErrors++;
            std::cout << "- operator (const&, &&) failed \n";   
        }
	}

    // (&&, &&)

    {
		matrix<double> a(3,{1. , 2. ,  3., 4.,  5.,  6., 7.,  8., 9.});
		matrix<double> b(3,{1., 1., 1. ,1., 1., 1., 1., 1., 1.});

		matrix<double> c = std::move(a) - std::move(b);
        if(!areEqual({0., 1., 2., 3.,4., 5., 6., 7., 8.}, c)){
            numErrors++;
            std::cout << "- operator (&&, &&) failed \n";   
        }
	}

    // * operator (const &, scalar)

    {
		matrix<double> a(3,{1. , 2. ,  3., 4.,  5.,  6., 7.,  8., 9.});
		matrix<double> c = a*2.5;
        if(!areEqual({2.5, 5., 7.5, 10. ,12.5, 15., 17.5, 20., 22.5}, c)){
            numErrors++;
            std::cout << "* operator (const&, scalar) failed \n";   
        }
	}
    // * operator (&&, scalar)

    {
		matrix<double> a(3,{1. , 2. ,  3., 4.,  5.,  6., 7.,  8., 9.});
		matrix<double> c = std::move(a)*2.5;
        if(!areEqual({2.5, 5., 7.5, 10. ,12.5, 15., 17.5, 20., 22.5}, c)){
            numErrors++;
            std::cout << "* operator (&&, scalar) failed \n";   
        }
	}
    // * operator (scalar, const&)
    {
		matrix<double> a(3,{1. , 2. ,  3., 4.,  5.,  6., 7.,  8., 9.});
		matrix<double> c = 2.5*a;
        if(!areEqual({2.5, 5., 7.5, 10. ,12.5, 15., 17.5, 20., 22.5}, c)){
            numErrors++;
            std::cout << "* operator (scalar, const&) failed \n";   
        }
	}
    // * operator (scalar, &&)
    {
		matrix<double> a(3,{1. , 2. ,  3., 4.,  5.,  6., 7.,  8., 9.});
		matrix<double> c = 2.5*std::move(a);
        if(!areEqual({2.5, 5., 7.5, 10. ,12.5, 15., 17.5, 20., 22.5}, c)){
            numErrors++;
            std::cout << "* operator (scalar, &&) failed \n";   
        }
	}

    // / operator (const&, scalar)
    {
		matrix<double> a(3,{2.5, 5., 7.5, 10. ,12.5, 15., 17.5, 20., 22.5});
		matrix<double> c = a / 2.5;
        if(!areEqual({1. , 2. ,  3., 4.,  5.,  6., 7.,  8., 9.}, c)){
            numErrors++;
            std::cout << "/ (const &, scalar) operator failed \n";   
        }
    }

    // / operator (&&, scalar)
    {
		matrix<double> a(3,{2.5, 5., 7.5, 10. ,12.5, 15., 17.5, 20., 22.5});
		matrix<double> c = std::move(a) / 2.5;
        if(!areEqual({1. , 2. ,  3., 4.,  5.,  6., 7.,  8., 9.}, c)){
            numErrors++;
            std::cout << "/ (&&, scalar) operator failed \n";   
        }
    }

    // Matrix multiply (const &, const &)
    {
		matrix<double> a(3,{1. , 2. ,  3., 4.,  5.,  6., 7.,  8., 9.});
        matrix<double> b(3, {2,4, 1., 4., 5., 9.2, 1., 1., 1.});
        matrix<double> c = a * b;
        if(!areEqual({13. , 17. ,  22.4, 34.,  47.,  56., 55.,  77., 89.6}, c)){
            numErrors++;
            std::cout << "Matrix multiplication (const&, const&) operator failed \n";   
        }
    }
    // Matrix multiply (&&, const &)
    {
		matrix<double> a(3,{1. , 2. ,  3., 4.,  5.,  6., 7.,  8., 9.});
        matrix<double> b(3, {2,4, 1., 4., 5., 9.2, 1., 1., 1.});
        matrix<double> c = std::move(a) * b;
        if(!areEqual({13. , 17. ,  22.4, 34.,  47.,  56., 55.,  77., 89.6}, c)){
            numErrors++;
            std::cout << "Matrix multiplication (&&, const&) operator failed \n";   
        }
    }

    // Matrix multiply (const&, &&)
    {
		matrix<double> a(3,{1. , 2. ,  3., 4.,  5.,  6., 7.,  8., 9.});
        matrix<double> b(3, {2,4, 1., 4., 5., 9.2, 1., 1., 1.});
        matrix<double> c = a * std::move(b);
        if(!areEqual({13. , 17. ,  22.4, 34.,  47.,  56., 55.,  77., 89.6}, c)){
            numErrors++;
            std::cout << "Matrix multiplication (const&, &&) operator failed \n";   
        }
    }

    // Matrix multiply (&&, &&)
    {
		matrix<double> a(3,{1. , 2. ,  3., 4.,  5.,  6., 7.,  8., 9.});
        matrix<double> b(3, {2,4, 1., 4., 5., 9.2, 1., 1., 1.});
        matrix<double> c = std::move(a) * std::move(b);
        if(!areEqual({13. , 17. ,  22.4, 34.,  47.,  56., 55.,  77., 89.6}, c)){
            numErrors++;
            std::cout << "Matrix multiplication (&&, &&) operator failed \n";   
        }
    }
    //Excercise 6, << operator

	{

		matrix<double> m(3,{1., 2., 3., 4., 5. ,6., 7., 8., 9.});
		std::stringstream ss;
		ss << m;
		std::string result = ss.str();
		if(result != "1 2 3 \n4 5 6 \n7 8 9 \n\n") { 
            numErrors ++ ;
            std::cout << "<< operator failed \n";
        }
	}

    // >> operator
    {

		matrix<double> m;
		std::stringstream ss;
		ss <<  "1 2 3 \n4 5 6 \n7 8 9 \n" ;
        ss >> m;
		if(!areEqual({1, 2, 3, 4, 5, 6, 7, 8, 9}, m) ) { 
            numErrors ++ ;
            std::cout << ">> operator failed \n";
        }
	}
    std::cout << "Number of errors is " << numErrors << "\n"; 
}