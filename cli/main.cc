#include <stdio.h>
#include <string.h>

enum Command
{
    ADD = 0,
    REMOVE = 1,
};

int main(int argc, char **argv)
{
    Command cmd;
    if (argc != 2)
    {
        printf("Invalid number of arguments.\n");
        return 1;
    }

    if (strcmp(argv[1], "add") == 0)
        cmd = ADD;
    else if (strcmp(argv[1], "remove") == 0)
        cmd = REMOVE;
    else
    {
        printf("Invalid command.\n");
        return 1;
    }
    
    return 0;
}