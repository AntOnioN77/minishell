//BORRA ESTA FUNCION ANTES DE ENTREGAR
void print_tree(t_tree *node, int depth) {
    if (!node)
        return;
    
    for (int i = 0; i < depth; i++)
        printf("  ");
        
    switch(node->type) {
        case PIPE:
            printf("PIPE\n");
            print_tree(((t_pipe *)node)->left, depth + 1);
            print_tree(((t_pipe *)node)->rigth, depth + 1);
            break;
        case TASK:
            printf("TASK: %s\n", ((t_task *)node)->pathname);
            break;
    }
}