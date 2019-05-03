
#include <iostream>
#include "quaternion.hpp"
#include <cmath>



int main() {
    quaternion<int> q{1, 2, 3, 4};
    quaternion<int> a;
    a = q;
    return 0;
}