
#pragma once
#include <iostream>
#include <cmath>
#include <sstream>
#include <string>
#include "quaternion.hpp"
#include "matrix.hpp"
#include "test.hpp"
#include <iomanip>

void TestCompatibility(){
    int numErrors = 0;
    // Testing compatibility of matrix-quaternion structures:
    // 
    // Start from v = (0., 1., 0.) || y, and rotate by 15 degrees and then 20 degrees around x axis.
    // Matrix :
    {
    //M1 = 
    /*[  1.0000000,  0.0000000,  0.0000000;
    0.0000000, -0.7596879, -0.6502879;
    0.0000000,  0.6502879, -0.7596879 ]
    //M2 = [  1.0000000,  0.0000000,  0.0000000;
    0.0000000,  0.4080821, -0.9129453;
    0.0000000,  0.9129453,  0.4080821 ]*/

    // quaternion: [ x = 0.6502878, y = 0,  z = 0, w = -0.7596879 ]
        Matrix3<double> m1{1., 0., 0., 0. , -0.7596879 , -0.6502879, 0., 0.6502879, -0.7596879};
        Matrix3<double> m2{1., 0., 0., 0. , 0.4080821 , -0.9129453, 0., 0.9129453, 0.4080821};
        Matrix3<double> mComposite = m1*m2;
        std::vector<double> v{0., 1., 0.};
        std::optional<std::vector<double>> one = m1*v;
        std::optional<std::vector<double>> result1 = m2*(one.value());
        std::optional<std::vector<double>> result2 = mComposite*v;
        if(!areEqual(result1.value(), result2.value())){
            numErrors++;
            std::cout << "Matrix composition failed. \n";
        }

    //rotation by 35 degrees:
    /*[  1.0000000,  0.0000000,  0.0000000;
        0.0000000, -0.9036922,  0.4281827;
        0.0000000, -0.4281827, -0.9036922 ]*/
        Matrix3<double> m{1., 0., 0., 0. , -0.9036922 , 0.4281827, 0., -0.4281827, -0.9036922};
        std::optional<std::vector<double>> two = m*v;
        if(!areEqual(two.value(), result2.value())){
            numErrors++;
            std::cout << "Matrix composition (35degs) failed. \n";
        }

        //Same with the quaternion: 
        quaternion<double> q { 0.21944, -0.975626, 0., 0.};
        std::optional<std::vector<double>> three  = rotateByQuaternion(q, v);
        if(!areEqual(*three, *result2)){
            numErrors++;
            std::cout << "Matrix composition (not equal to quaternion) failed. \n";
        }

    }

}
