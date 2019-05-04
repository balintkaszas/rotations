
#pragma once
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include "quaternion.hpp"
#include "matrix.hpp"

template<typename F>
 bool areEqual(const std::vector<double> &reference, const  F & q, const double precision = 1e-6) {
    
    return (std::equal (q.cbegin(), q.cend(), reference.cbegin(), [=](const auto x, const auto y){return std::abs(x - y) < precision;})) ;
}  

void TestMatrix(){
    int numErrors = 0;
    {
		Matrix3<double> m({1., 2., 3., 4., 5., 6., 7., 8., 9.});
		if(m[0] != 1. or m[1] != 2. or m[2] != 3. or m[3] != 4. or m[4] != 5. or m[5] != 6. or m[6] != 7. or m[7] != 8. or m[8] != 9.) {
            numErrors++;
            std::cout << "initializer list constructor failed (1d indexing) \n";
        }
		if(m(0, 0) != 1. or m(0, 1) != 2. or m(0, 2) != 3. or m(1, 0) != 4. or m(1, 1) != 5. or m(1, 2) != 6. or m(2, 0) != 7. or m(2, 1) != 8. or m(2, 2) != 9.) {
            numErrors++;
            std::cout << "initializer list constructor failed (2d indexing) \n";
        }
        auto itWrite = m.begin();
        auto itRead = m.cbegin();
        for(int j = 1; j < 9 ; ++j){
            if(*itWrite != j or *itRead != j) {
                numErrors++;
                std::cout << "Iterator failed " << *itWrite << " " << j << "\n";
            } 
            std::advance(itWrite, 1);
            std::advance(itRead, 1);
        }
    }
    {
        Matrix3<double> m({1., 2., 3., 4., 5., 6., 7., 8., 9.});
        if(!areEqual({1., 2., 3., 4., 5., 6., 7., 8., 9.}, m) ){
            numErrors ++;
            std::cout << "std::equal failed \n";
        }
    }
    {
        Matrix3<double> m{1., 2., 3.,4,5., 6., 7., 8., 9.};
        if(m.determinant() != 0){
            numErrors ++;
            std::cout << "determinant failed \n";
        }
    }
    {
        Matrix3<double> m{1., 2., 3.,4,5., 6., 7., 8., 9.};
        if(m.isRotation()){
            numErrors ++;
            std::cout << "isRotation() failed \n";
        }
    }
    {
    // Test case: rotation around X axis by 30 degs.
    /*1.0000000,  0.0000000,  0.0000000;
    0.0000000,  0.1542515,  0.9880316;
    0.0000000, -0.9880316,  0.1542515 */

    // quaternion: [ x = 0.6502878, y = 0,  z = 0, w = -0.7596879 ]
        Matrix3<double> m{1., 0., 0., 0. , 0.1542515 , 0.9880316, 0., -0.9880316, 0.1542515};
        auto q = *m.convertToQuaternion();
        if(!areEqual({-0.7596879, 0.6502878, 0., 0.}, q )){
            numErrors++;
            std::cout << "matrix -> quaternion conversion failed \n";
            std::cout << "x: " << q.x() << " y: " << q.y() << " z: " << q.z() << " w: " << q.w() << " \n";
            std::ostream_iterator<double > out_it (std::cout," ");
            std::copy ( q.begin(), q.end(), out_it );
        }
    }
    //Test the rotation operator: M*v. Same matrix as above
    {
        Matrix3<double> m{1., 0., 0., 0. , 0.1542515 , 0.9880316, 0., -0.9880316, 0.1542515};
        std::vector<double> vec{0., 1., 0.}; //v || y, expected : (0, cos(30), sin(30))
        std::optional<std::vector<double>> result = m*vec;
        double s = std::sin(30);
        double c = std::cos(30); 
        if(result and !areEqual({0, c, s}, *result )){
            numErrors++;
            std::cout << "matrix rotation failed: expected components are: \n" << " 0 ,"  << c << ", " << s <<" \n";
        }
    }
}
 

void TestFunction() {
    int numErrors = 0;
    //Default constructor
    {
        quaternion<int> q;
        if( q.x() != 0 or q.y() != 0 or q.z() != 0 or q.w() != 0 ){
            numErrors++;
            std::cout << "Default constructor failed \n";
        }
        
    }
    {
        quaternion<int> q;
        if(q.norm() != 0){
            numErrors++;
            std::cout << "default norm failed \n";    
        }
    }

    {
        quaternion<int> q{1, 2, 3, 4}; // sqrt(1 + 4 + 9 + 16)
        if(q.norm() != std::sqrt(30)){
            numErrors++;
            std::cout << q.norm() << " vs " << std::sqrt(30) << " norm failed \n";    
        }
    }

    {   //Rotation by 30 degs around x axis: (1, 0, 0)
        double c = std::cos(15.);
        double s = std::sin(15.);
        quaternion<double> q{c, s, 0, 0}; 
        if(!q.isRotation()){
            numErrors++;
            std::cout << "isRotation failed \n";    
        }
        if(!areEqual({-0.7596879, 0.6502878, 0., 0.}, q)){
            numErrors++;
            std::cout << "rotation representation failed \n";
        }
    }
    //quaternion multiply:

    {
        //a  = 1 0 1 0
        //b = 1 0.5 0.5 0.75
        //ab =  0.5000    1.2500    1.5000    0.2500
        quaternion<double> a {1., 0., 1., 0.};
        quaternion<double> b {1., 0.5, 0.5, 0.75};
        quaternion<double> res = a*b;
        if(!areEqual({0.5, 1.25, 1.5, 0.25}, res)){
            numErrors++;
            std::cout << "Quaternion multiplication failed \n";
            std::ostream_iterator<double > out_it (std::cout," ");
            std::copy ( res.begin(), res.end(), out_it );
        }

    }
    //Rotate by quaternion:
    {
    // quaternion: [ x = 0.6502878, y = 0,  z = 0, w = -0.7596879 ]
    // r = [0., 1., 0.]
        quaternion<double> q{-0.7596879, 0.6502878, 0., 0.};
        std::vector<double> r {0., 1., 0.};
        std::optional<std::vector<double>> result = rotateByQuaternion(q, r); 
        double s = std::sin(30);
        double c = std::cos(30);
        if(result){
            if(!areEqual({0, c, s}, *result )){
                numErrors++;
                std::cout << "Wrong result in rotateByQuaternion \n";
            }
        }
        else{
            numErrors++;
            std::cout << "std::optional error \n";
        }
    }   
    //Testing optional 
    {
    // quaternion: [ x = 99, y = 123,  z =110, w = -0.7596879 ]
    // r = [0., 1., 0.]
        quaternion<double> q{-0.7596879, 99., 123., 110.};
        std::vector<double> r {0., 1., 0.};
        std::optional<std::vector<double>> result = rotateByQuaternion(q, r); 
        if(result){
            numErrors++;
            std::cout << "Std::optional error \n";
        }
    } 

    {
    // Test case: rotation around X axis by 30 degs.
    /*1.0000000,  0.0000000,  0.0000000;
    0.0000000,  0.1542515,  0.9880316;
    0.0000000, -0.9880316,  0.1542515 */

    // quaternion: [ x = 0.6502878, y = 0,  z = 0, w = -0.7596879 ]
        quaternion<double> q{-0.7596879, 0.6502878, 0., 0.};
        auto m = q.convertToMatrix();

        if(!areEqual({1., 0., 0., 0. , 0.1542515 , 0.9880316, 0., -0.9880316, 0.1542515}, m )){
            numErrors++;
            std::cout << "quaternion -> matrix conversion failed \n";
            std::ostream_iterator<double > out_it (std::cout," ");
            std::copy ( m.begin(), m.end(), out_it );
        }
    }
    // quaternion multiplication:
    {

    }

}
/*
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
}*/