
#include <iostream>
#include "matrix.hpp"
#include "quaternion.hpp"
#include "axisAngle.hpp"
#include <cmath>
#include "test.hpp"
#include "testCompatibility.hpp"

int main() {
    TestQuaternion();
    TestMatrix();
    TestAxisAngle();
    TestCompatibility();
    return 0;
}