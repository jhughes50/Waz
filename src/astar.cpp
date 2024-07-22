/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   A second attempt at astar
*
* Package: Waz
*/

#include "waz/astar.hpp"

AStarSearch::Node::Node(int r, int c, double g, double h, Node* p) : row(r), col(c), g(g), h(g), f(g+h), parent(p) { }

bool AStarSearch::Node::operator>(const Node& other) const
{
    return f > other.f;
}

double AStarSearch::heuristicCost(int row_1, int col_1, int row_2, int col_2)
{
    return std::sqrt((row_2 - row_1) * (row_2 - row_1) + (col_2 - col_1) * (col_2 - col_1));
}

std::vector<cv::Point> AStarSearch::getNeighbors(const cv::Point& point, int rows, int cols)
{
    std::vector<cv::Point> neighbors;

    for (int dr = -1; dr <= 1; ++dr)
    {
        for (int dc = -1; dc <= 1; ++ dc)
        {
            if (dr == 0 && dc == 0) continue;
            int nr = point.y + dr;
            int nc = point.x + dc;
            if (nr >= 0 && nr < rows && nc >= 0 && nc < cols) neighbors.emplace_back(cv::Point(nc, nr));
        }
    }

    return neighbors;
}

std::vector<cv::Point> AStarSearch::search(const cv::Mat& cost_map, const cv::Point& start, const cv::Point& goal)
{
    std::priority_queue<Node, std::vector<Node>, std::greater<Node>> open_set;
    std::set<std::pair<int, int>> closed_set;
    std::map<std::pair<int, int>, Node*> all_nodes;

    Node* start_node = new Node(start.y, start.x, 0, heuristicCost(start.y, start.x, goal.y, goal.x), nullptr);
    open_set.push(*start_node);
    all_nodes[std::make_pair(start.y,start.x)] = start_node;

    while (!open_set.empty())
    {
        Node current = open_set.top();
        open_set.pop();

        if (current.row == goal.y && current.col == goal.x)
        {
            std::vector<cv::Point> path;
            for (Node* node = &current; node != nullptr; node=node->parent)
            {
                path.emplace_back(node->col, node->row);
            }
            std::reverse(path.begin(), path.end());
            
            for (auto& node : all_nodes) delete node.second;
            
            return path;
        }

        closed_set.insert(std::make_pair(current.row, current.col));

        for (const auto& neighbor : getNeighbors(cv::Point(current.col, current.row), cost_map.rows, cost_map.cols))
        {
            std::pair<int, int> n_pair = std::make_pair(neighbor.y, neighbor.x);

            if (closed_set.count(n_pair) > 0 || cost_map.at<uchar>(neighbor) == 255) continue;

            double new_g = current.g + heuristicCost(current.row, current.col, neighbor.y, neighbor.x);
            
            if (all_nodes.count(n_pair) == 0 || new_g < all_nodes[n_pair]->g)
            {
                Node* new_node = new Node(neighbor.y, neighbor.x, new_g, heuristicCost(neighbor.y, neighbor.x, goal.y, goal.x), all_nodes[std::make_pair(current.row, current.col)]);
                open_set.push(*new_node);
                all_nodes[n_pair] = new_node;
            }
        }
    }

    for (auto& node : all_nodes) delete node.second;
    return {};
}
