/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   A matrix class to hold the output of 
*          the segformer model
*
* Package: Waz
*/
i
#ifndef CLASS_MATRIX_HPP
#define CLASS_MATRIX_HPP

#include <string>
#include <map>
#include <Eigen/Dense>
#include <torch/script.h>

class ClassMatrix
{
    public:
        ClassMatrix() = default;
        ClassMatrix(std::map<int, std::string> id2label);

        std::vector<> operator[](int id);
        std::vector<float> operator[](std::string label);

    private:
        std::map<int, std::string> id2label_;
        std::map<std::string, int> label2id_;

};
#endif
