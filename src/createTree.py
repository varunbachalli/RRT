def create_tree(height, nodes_per_node):
    r = 0
    for i in range(height):
        r += nodes_per_node**i
    f = open("tree structure.txt", "w")
    for i in range(r):
        f.write(f"TreeNode* n{i} = new TreeNode({i}.0,{i}.0);\n")

    for i in range(r-1):
        f.write(f"n{int((i)/3)}->children.insert(std::pair<TreeNode*")
        f.write(f", double>(n{i+1},euclidean_distance(n{i+1}->x, n{i+1}->y, n{int((i)/3)}->x, n{int((i)/3)}->y)));\n")
    f.close()
create_tree(3,3)