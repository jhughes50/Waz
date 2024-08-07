/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   module to search through the cost map
*
* Package: Waz
*/

#ifndef SEARCH_HPP
#define SEARCH_HPP

#include <opencv2/opencv.hpp>
#include <queue>
#include <vector>
#include <cmath>
#include <cassert>

class AStarSearch
{
    public:
        AStarSearch() = default;
        ~AStarSearch();

        std::vector<cv::Point> search(const cv::Mat& cost_map, const cv::Point& start, const cv::Point& goal);

        struct Node
        {
            int x, y;
            double goal_cost, heuristic_cost, cost_sum;

            Node* parent;

            Node(int x, int y, double g, double h, Node* parent); 
        };

        struct CompareNode
        {
            CompareNode() = default;
            bool operator()(const Node* lhs, const Node* rhs);
        };

    private:
        
        void cleanup();

        std::vector<Node*> all_nodes_;

        static const int SIZE = 8;
        int dx_[SIZE] = {-1,0,1,0,-1,-1,1,1};
        int dy_[SIZE] = {0,1,0,-1,-1,1,-1,1};
};
#endif
