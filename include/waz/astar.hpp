/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   A second attempt at astar
*
* Package: Waz
*/

#ifndef ASTAR_HPP
#define ASTAR_HPP

#include <opencv2/opencv.hpp>
#include <queue>
#include <vector>
#include <map>
#include <cmath>

class AStarSearch
{

    public:

        AStarSearch() = default;

        struct Node
        {
            int row, col;
            double f, g, h;
            Node* parent;

            Node(int r, int c, double g, double h, Node* p);
            bool operator>(const Node& other) const;
        };

        std::vector<cv::Point> search(const cv::Mat& cost_map, const cv::Point& start, const cv::Point& goal);
    private:

        double heuristicCost(int row_1, int col_1, int row_2, int col_2);
        std::vector<cv::Point> getNeighbors(const cv::Point& point, int rows, int cols);
};
#endif
