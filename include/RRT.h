#include <vector>
#include <iostream>
#include <mutex>
#include <thread>
#include <math.h>
#include <map>
#include <iterator>
#include <random>
#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <limits> 
#include <stack> 

struct Rectangle
{
    int x1, x2, x3, x4;
    int y1, y2, y3, y4;
    
    int w, h;
    Rectangle(int x1, int y1, int x3, int y3)
    {
        w = x3 - x1;
        h = y3 - y1;
        this->x1 = x1;
        this->y1 = y1;
        x2 = x1 + w;
        y2 = y1;
        this->x3 = x3;
        this->y3 = y3;
        x4 = x1;
        y4 = y1 + h;
    }
};

struct point
{
    double x, y;
    point(){};
    point(double x, double y){this->x = x; this->y = y;}
};

struct Node
{
    point p;
    std::map<Node*, double> children;
    Node *parent = nullptr;
    int number_children;
    double node_cost = 0.0;
    Node(point p_)
    {
        p  = p_;
    };

    ~Node()
    {
        for (auto child : children)
        {
            std::cout << "deleted node" <<p.x <<"," <<p.y<< std::endl;
            delete child.first;
        }
    }
};


class RRT
{
private:
    Node *root;
    std::mutex mu;
    int max_x, max_y;
    point end_p;
    double del_q;
    double radius;
    double check_cost(point& p1, point& p2);
    std::vector<Rectangle> obstacles;
    bool check_colision_with_rectangle(point& p1, point& p2, Rectangle& rect);
    bool check_collision_with_all_obstacles(point& p1, point p2);
    double euc_distance(point& p1, point& p2);
    std::vector<std::vector<point>> lines;
    bool doIntersect(point p1, point q1, point p2, point q2);
    int orientation(point p, point q, point r);
    std::stack<point> best_line;
    void set_solution_line(Node* end);
    bool result_reached(point p);
    
public:
    RRT(int width, int height);
    ~RRT();
    bool add_node_to_tree(point p, Node *parent, double distance);// has mutex
    void get_closest_point(double &best_distance, Node* node ,Node* &result, point& new_p);
    void get_new_point(point& p);
    void set_del_q(double x);
    void set_end_point(int x, int y, double radius);
    void set_start_point(int x, int y);
    void set_obstacles(std::vector<Rectangle> obst){obstacles = obst;}
    Node* getroot(){ return root;}
    void WriteOutput(Node *node, point path[], int pathLen, std::ofstream& file);
    void OpenFile(int total_points);
    void WriteLine(point path[], int len, std::ofstream& file);
    void set_lines(point path[], int len);
    std::vector<std::vector<point>> get_lines(){return lines;}
    void clear_lines(){lines.clear();}
    void print_stack(std::stack<point> line);
};

