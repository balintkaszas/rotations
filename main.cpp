
#include <iostream>
#include "matrix.hpp"
#include "quaternion.hpp"
#include "axisAngle.hpp"
#include <cmath>
#include "test.hpp"
#include <string>
#include <fstream>
#include "testCompatibility.hpp"
#include <optional>

typedef std::vector<double> point;
class Points{
    private: 
    std::vector<point> data;
    public:
    Points():data{}{};
    Points(const std::vector<point> &d): data{d}{}; //construct from data mtx
    Points(const std::string & filename) { //construct from file
        std::ifstream myfile(filename);
        data.reserve(1000);
        double x, y, z;
        while (myfile >> x >> y >> z) { //Read a row of the file
            point read {x, y, z};
            data.push_back(read);
        }
    }
    Points rotate(const std::optional<quaternion<double>> &quaternion) const {
        std::vector<point> rotated;
        if(quaternion){
            rotated.reserve(data.size());
            for(auto e : data){
                auto result = rotateByQuaternion(quaternion.value(), e);
                if(result){ // rotate each point, push back to result vector if succesful.
                    rotated.push_back(result.value());
                }
            }
        }
        return Points(rotated);

    }

    Points rotate(const std::optional<Matrix3<double>> &M) const {
        std::vector<point> rotated;
        if(M){
            rotated.reserve(data.size());
            for(auto e : data){
                auto result = M.value()*e;
                if(result){ // rotate each point, push back to result vector if succesful.
                    rotated.push_back(result.value());
                }
            }
        }
        return Points(rotated);
    }
    void writeToFile(const std::string & filename) const {
        std::ofstream output;
        output.open(filename);
        for(auto e : data){
            output << e[0] << " " << e[1] << " " << e[2] << "\n";
        }
    }
};
  

int main() {
    //Basic Test cases:

    TestQuaternion();
    TestMatrix();
    TestAxisAngle();
    TestCompatibility();
    //

    //Rotating an ellipse :
    //defining the quaternions, rotate around y axis by 45 degrees
    axisAngle<double> rot({1./std::sqrt(2), 1./std::sqrt(2), 0.}, 45.);

    Points ellipse("ellipse.dat");
    Points rotated = ellipse.rotate(rot.convertToQuaternion());
    rotated.writeToFile("quat_ellipse.dat");

    return 0;
}