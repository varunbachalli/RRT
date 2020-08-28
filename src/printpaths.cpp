#include <RRT.h>

void RRT::WriteLine(point path[], int len, std::ofstream& file)
{
    // std::cout << "new line " << std::endl;
    file <<"new line" << std::endl;
    int i;
    for (i = 0; i < len; i++)
    {
        // std::cout << path[i].x << "," << path[i].y << "\n";
        file << path[i].x << "," << path[i].y << "\n";
    }
}

void RRT::set_lines(point path[],int len)
{
    std::vector<point> line(path,path+len);
    lines.push_back(line);
}

void RRT::WriteOutput(Node *node, point path[], int pathLen, std::ofstream& file)
{
    if (node == NULL)
        return;

    /* append this node to the path array */
    path[pathLen].x = node->p.x;
    path[pathLen].y = node->p.y;
    pathLen++;

    /* it's a leaf, so print the path that led to here */
    if (node->children.size() == 0)
    {
        WriteLine(path, pathLen, file);
        set_lines(path , pathLen); 
    }
    else
    {
        /* otherwise try all subtrees */
        for (auto itr = node->children.begin(); itr != node->children.end(); ++itr)
        {
            WriteOutput(itr->first, path, pathLen, file);
        }
    }
}

void RRT::OpenFile(int total_points)
{
    std::ofstream file;
    file.open("output.txt");
    Node* start = this->root;
    file << start->p.x << "," << start->p.y << "," << this->end_p.x << "," << this->end_p.y << "\n";
    point* path = new point[total_points];
    WriteOutput(start, path, 0, file);
    file.close();
    delete[] path;
}