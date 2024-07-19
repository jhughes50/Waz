/*!
* Author:  Jason Hughes
* Date:    July 2024
* About:   A star search for cv::mat
*          searching 
*
* Package: Waz
*/

#include "waz/search.hpp"

AStarSearch::~AStarSearch()
{
    std::cout << "[ASTAR] Destructing" << std::endl;
    for (Node* node : all_nodes_)
    {
        delete node;
    }
}

void AStarSearch::cleanup()
{
    for (Node* node : all_nodes_)
    {   
        delete node;
    }
}

std::vector<cv::Point> AStarSearch::search(const cv::Mat& cost_map, const cv::Point& start, const cv::Point& goal)
{
    std::priority_queue<Node*, std::vector<Node*>, CompareNode> open_nodes;
    cv::Mat visited_nodes = cv::Mat::zeros(cost_map.rows, cost_map.cols, CV_8U);
    
    Node* start_node = new Node(start.x, start.y, 0, 0, nullptr);
    all_nodes_.push_back(start_node);

    start_node->heuristic_cost = std::sqrt(std::pow(goal.x - start.x, 2) + std::pow(goal.y - start.y, 2));
    open_nodes.push(start_node);
    
    while (!open_nodes.empty())
    {
        Node* current = open_nodes.top();
        open_nodes.pop();

        if (current->x == goal.x && current->y == goal.y)
        {
            std::vector<cv::Point> path;
            
            while (current != nullptr)
            {
                path.push_back(cv::Point(current->x, current->y));
                current = current->parent;
            }
            std::reverse(path.begin(), path.end());
            //cleanup();
            return path;
        }

        visited_nodes.at<bool>(current->x, current->y) = true;
        
        for (int i=0; i < SIZE; ++i)
        {
            int new_x = current->x + dx_[i];
            int new_y = current->y + dy_[i];

            if (new_x >= 0 && new_x < cost_map.cols && new_y >= 0 && new_y < cost_map.rows && cost_map.at<uint8_t>(new_x, new_y) != 255 && !visited_nodes.at<bool>(new_x, new_y))
             {
                double new_gc = current->goal_cost + (i < 4 ? 1.0 : std::sqrt(2.0));
                double new_hc = std::sqrt(std::pow(goal.x - new_x, 2) + std::pow(goal.y - new_y, 2));
            
                Node* neighbor = new Node(new_x, new_y, new_gc, new_hc, current);
                all_nodes_.push_back(neighbor);
                open_nodes.push(neighbor);
             }
        }
    }
    
    //cleanup();
    return std::vector<cv::Point>();
}

AStarSearch::Node::Node(int x, int y, double g, double h, Node* p) 
    : x(x), y(y), goal_cost(g), heuristic_cost(h), cost_sum(g+h), parent(p) { }

bool AStarSearch::CompareNode::operator()(const Node* lhs, const Node* rhs)
{
    return lhs->cost_sum > rhs->cost_sum;
}


