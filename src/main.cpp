#include "RRT.h"
#include "gui.h"

int main()
{
    gui g;
    if (!g.get_user_input())
    {
        std::cout << "input stage not completed" << std::endl;
        return 1;
    }

    int x, y;
    g.get_window_dimensions(x, y);
    RRT obj(x, y);

    g.get_start_point(x, y);
    obj.set_start_point(x, y);

    double r;
    g.get_end_point(x, y, r);
    obj.set_end_point(x, y, r);

    g.get_del_q(r);
    obj.set_del_q(r);

    obj.set_obstacles(g.get_obstacles());
    bool start_looping = true;
    int total_iterations = 0;
    while(start_looping)
    {   
        std::cout << "how many iterations do you want to run?" << std::endl;
        std::string iterations;
        std::cin >> iterations;
        int iter = std::stoi(iterations);
        total_iterations+= iter;
        point p;
        for (int i = 0; i < iter; ++i)
        {
            obj.get_new_point(p);
            double distance = std::numeric_limits<double>::max();
            Node *root = obj.getroot();
            Node *result = obj.getroot();
            obj.get_closest_point(distance, root, result, p);
            std::cout << result->p.x << "," << result->p.y << std::endl;
            if(obj.add_node_to_tree(p, result, distance))
            {
                start_looping = false;
                break;
            };
        }
        obj.OpenFile(total_iterations);
        g.plot_results(obj.get_lines());
        std::cout << "keep going? " << std::endl;
        std::cin >> iterations;
        if(iterations == "N")
        {
            break;
            start_looping = false;
        }
    }
    return 0;
}