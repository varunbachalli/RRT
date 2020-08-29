#include <RRT.h>

RRT::RRT(int width, int height)
{
    this->max_y = width;
    this->max_x = height;
    srand(time(NULL));
}

void RRT::set_start_point(int x, int y)
{
    point p;
    p.x = x;
    p.y = y;
    root = new Node(p);
}

RRT::~RRT()
{
    delete root;
}

void RRT::set_end_point(int x, int y, double radius)
{
    end_p.x = x;
    end_p.y = y;
    this->radius = radius;
}

void RRT::set_del_q(double x)
{
    del_q = x;
}

void RRT::get_new_point(point &p)
{
    double fx = (double)rand() / RAND_MAX;
    double fy = (double)rand() / RAND_MAX;
    p.x = fx * max_x;
    p.y = fy * max_y;
}

bool onSegment(point p, point q, point r)
{
    if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
        q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y))
        return true;

    return false;
}

// To find orientation of ordered triplet (p, q, r).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
int RRT::orientation(point p, point q, point r)
{
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/
    // for details of below formula.
    int val = (q.y - p.y) * (r.x - q.x) -
              (q.x - p.x) * (r.y - q.y);

    if (val == 0)
        return 0; // colinear

    return (val > 0) ? 1 : 2; // clock or counterclock wise
}

bool RRT::doIntersect(point p1, point q1, point p2, point q2)
{
    // Find the four orientations needed for general and
    // special cases
    int o1 = orientation(p1, q1, p2);
    int o2 = orientation(p1, q1, q2);
    int o3 = orientation(p2, q2, p1);
    int o4 = orientation(p2, q2, q1);

    // General case
    if (o1 != o2 && o3 != o4)
        return true;

    // Special Cases
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1
    if (o1 == 0 && onSegment(p1, p2, q1))
        return true;

    // p1, q1 and q2 are colinear and q2 lies on segment p1q1
    if (o2 == 0 && onSegment(p1, q2, q1))
        return true;

    // p2, q2 and p1 are colinear and p1 lies on segment p2q2
    if (o3 == 0 && onSegment(p2, p1, q2))
        return true;

    // p2, q2 and q1 are colinear and q1 lies on segment p2q2
    if (o4 == 0 && onSegment(p2, q1, q2))
        return true;

    return false; // Doesn't fall in any of the above cases
}
bool RRT::check_colision_with_rectangle(point &p1, point &p2, Rectangle &rect)
{
    point r1;
    r1.x = rect.x1;
    r1.y = rect.y1;

    point r2;
    r2.x = rect.x2;
    r2.y = rect.y2;

    if (doIntersect(r1, r2, p1, p2))
    {
        return true;
    }
    point r3;
    r3.x = rect.x3;
    r3.y = rect.y3;

    if (doIntersect(r2, r3, p1, p2))
    {
        return true;
    }
    point r4;
    r4.x = rect.x4;
    r4.y = rect.y4;
    if (doIntersect(r3, r4, p1, p2))
    {
        return true;
    }
    if (doIntersect(r4, r1, p1, p2))
    {
        return true;
    }
    return false;
}
bool RRT::check_collision_with_all_obstacles(point &p1, point p2)
{
    double distance = euc_distance(p1, p2);
    if (distance > del_q)
    {
        p2.x = p1.x + (p2.x - p1.x) * del_q / distance;
        p2.y = p1.y + (p2.y - p1.y) * del_q / distance;
    }
    for (Rectangle rect : obstacles)
    {
        if (check_colision_with_rectangle(p1, p2, rect))
        {
            return true;
        }
    }
    return false;
}
double RRT::euc_distance(point &p1, point &p2)
{
    return sqrt(pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2));
}
double RRT::check_cost(point &p, point &newp)
{
    if (check_collision_with_all_obstacles(p, newp))
    {
        return std::numeric_limits<double>::max();
    }
    return euc_distance(p, newp);
}

void RRT::set_solution_line(Node *end)
{
    while (end->parent != nullptr)
    {
        best_line.push(end->p);
        end = end->parent;
    }

    print_stack(best_line);
}

void RRT::print_stack(std::stack<point> s)
{
    std::cout << "best line is " << std::endl;
    while (!s.empty()) 
    { 
        std::cout << s.top().x << ", " << s.top().y << std::endl; 
        s.pop(); 
    } 
   std::cout << '\n'; 
}

bool RRT::result_reached(point p)
{
    if (pow(p.x - end_p.x, 2) + pow(p.y - end_p.y, 2) < pow(radius, 2))
    {
        std::cout << "result reached you can stop now" << std::endl;
        return true;
    }
    return false;
}

bool RRT::add_node_to_tree(point p, Node *parent, double distance)
{
    bool result = false;
    if (distance > del_q)
    {
        p.x = parent->p.x + (p.x - parent->p.x) * del_q / distance;
        p.y = parent->p.y + (p.y - parent->p.y) * del_q / distance;
        Node *new_node = new Node(p);
        parent->children.push_back(new_node);
        new_node->node_cost = del_q + parent->node_cost;
        new_node->parent = parent;
        if (result_reached(p))
        {
            result = true;
            set_solution_line(new_node);
        }
    }
    else
    {
        Node *new_node = new Node(p);
        parent->children.push_back(new_node);
        new_node->node_cost = distance + parent->node_cost;
        new_node->parent = parent;
        if (result_reached(p))
        {
            result = true;
            set_solution_line(new_node);
        }
    }
    parent->number_children++;
    return result;
}

void RRT::get_closest_point(double &best_distance, Node *node, Node *&result, point &new_p)
{
    double current_distance_to_node = check_cost(node->p, new_p);
    if (best_distance > current_distance_to_node)
    {
        result = node;
        best_distance = current_distance_to_node;
    }
    if (node->children.size() > 0)
    {
        auto itr = node->children.begin();
        while (itr != node->children.end())
        {
            get_closest_point(best_distance, *itr, result, new_p);
            ++itr;
        }
    }
}
