
#pragma once
#include <iostream>
#include <cmath>
#include "quaternion.hpp"
#include "matrix.hpp"
#include "axisAngle.hpp"
#include <iterator>

template<typename F>
 bool areEqual(const std::vector<double> &reference, const  F & q, const double precision = 1e-6) {
    
    return (std::equal (q.cbegin(), q.cend(), reference.cbegin(), [=](const auto x, const auto y){return std::abs(x - y) < precision;})) ;
}  

void TestAxisAngle(){
    int numErrors = 0;
    {
        axisAngle<double> a;
        if(a.getAngle() != 0 ){
            numErrors++;
            std::cout << "default constructor failed \n";
        }
        axisAngle<double> b({1., 3.,2.}, 12.);
        if(b.getAngle() != 12 or b.x() != 1. or b.y() != 3. or b.z() != 2. ){
            numErrors++;
            std::cout << "constructor failed \n";
        }

        axisAngle<double> d ({1., 3.,2.}, 12.);
        if(d.getAngle() != 12 or d.x() != 1. or d.y() != 3. or d.z() != 2. ){
            numErrors++;
            std::cout << "constructor failed2 \n";
        }
    }
    {   //conversion to matrix:
        axisAngle<double> a({1., 0., 0.}, 30.); 

        std::optional<Matrix3<double>> converted = a.convertToMatrix(); //because of std::optional

        if(!converted){
            std::cout << "No result";
        }
        if(!areEqual({1., 0., 0., 0. , 0.1542515 , 0.9880316, 0., -0.9880316, 0.1542515}, *converted) ){
            numErrors ++;
            std::cout << "axis-angle -> matrix conversion failed \n";
        }
    }
    {   //conversion to quaternion:
        axisAngle<double> a({1., 0., 0.}, 30.); 

        std::optional<quaternion<double>> converted = a.convertToQuaternion(); //because of std::optional

        if(!converted){
            std::cout << "No result";
        }
        if(!areEqual({-0.7596879, 0.6502878, 0., 0.}, *converted) ){
            numErrors ++;
            std::cout << "axis-angle -> quaternion conversion failed \n";
        }
    }

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
        auto q = m.convertToQuaternion();
        if(q){
            auto val = q.value();
            if(!areEqual({-0.7596879, 0.6502878, 0., 0.}, val )){
                numErrors++;
                std::cout << "matrix -> quaternion conversion failed \n";
                std::cout << "x: " << val.x() << " y: " << val.y() << " z: " << val.z() << " w: " << val.w() << " \n";
                std::ostream_iterator<double > out_it (std::cout," ");
                std::copy ( val.begin(), val.end(), out_it );
            }
        }
    }
    //Test the rotation operator: M*v. Same matrix as above
    {
        Matrix3<double> m{1., 0., 0., 0. , 0.1542515 , 0.9880316, 0., -0.9880316, 0.1542515};
        std::vector<double> vec{0., 1., 0.}; //v || y, expected : (0, cos(30), sin(30))
        std::optional<std::vector<double>> result = m*vec;
        if(result){
            auto value = result.value();
            double s = std::sin(30);
            double c = std::cos(30); 
            if(result and !areEqual({0, c, s}, value )){
                numErrors++;
                std::cout << "matrix rotation failed: expected components are: \n" << " 0 ,"  << c << ", " << s <<" \n";
            }
        }
    }
    //Test matrix multiplication
    {
		Matrix3<double> a{1. , 2. ,  3., 4.,  5.,  6., 7.,  8., 9.};
        Matrix3<double> b{2,4, 1., 4., 5., 9.2, 1., 1., 1.};
        Matrix3<double> c = a * b;
        if(!areEqual({13. , 17. ,  22.4, 34.,  47.,  56., 55.,  77., 89.6}, c)){
            numErrors++;
            std::cout << "Matrix multiplication  operator failed \n";   
        }
    }
    //Test matrix -> axis-angle conversion:
    {
        Matrix3<double> m{1., 0., 0., 0. , 0.1542515 , 0.9880316, 0., -0.9880316, 0.1542515};
        std::optional<axisAngle<double>> res = m.convertToAxisAngle();
        if(res){
            auto a = res.value();
            if(a.getAngle() != 30 and a.x() != 1. and a.y() != 0 and a.z() != 0){
                numErrors++;
                std::cout << "matrix -> axis-angle conversion failed.\n";
            }
        }else{
            std::cout << "No result \n";
        }
    }
}
 

void TestQuaternion() {
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
    //Testing quaternion to axis-angle conversion:
    {
        quaternion<double> q{-0.7596879, 0.6502878, 0., 0.};
        std::optional<axisAngle<double>> res = q.convertToAxisAngle();
        if(res){
            auto a = res.value();
            if(a.getAngle() != 30 and a.x() != 1. and a.y() != 0 and a.z() != 0){
                numErrors++;
                std::cout << "quaternion -> axis-angle conversion failed.\n";
            }
        }
        else{
            std::cout << "No result \n";
        }
    }

}
