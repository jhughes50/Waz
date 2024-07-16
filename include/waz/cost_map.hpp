/* Author:  Jason Hughes
*  Date:    June 2024
*  About:   Cost Map Object influnced by relative depth
*           and semantic segmentation
*  Package: waz
*/

#ifndef COST_MAP_HPP
#define COST_MAP_HPP

#include <string>
#include <map>
#include <Eigen/Dense>

#include "label_map.hpp"
#include "params.hpp"

class CostMap
{
    public:
        //CostMap() = default;
        //CostMap(int width, int height);
        CostMap(std::string path);

        struct CostMapParams : public Params 
        {
            using Params::Params;

            int width, height;
            std::map<int, LabelMap> label_map;

            void setParams() noexcept;
        };

        int getSize();
        int getCostMap();

    private:
        static const int SIZE = 256;
        Eigen::Matrix<uint8_t, SIZE, SIZE> cost_map_;
    
        void searchDepthForObjects(const Eigen::MatrixXf& depth);

        CostMapParams params_;
};
#endif
