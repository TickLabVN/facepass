#include <stdio.h>
#include <string.h>
#include "add_face.h"

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("Invalid number of arguments.\n");
        return 1;
    }

    if (strcmp(argv[1], "add") == 0)
    {
        cv::Mat screenshot;
        int result = capture_face(screenshot);
        if (result != 0)
        {
            printf("Failed to capture face.\n");
            return 1;
        }
        cv::Mat face = detect_face(screenshot);
        cv::imwrite("face.jpg", face);
        return 0;
    }
    else if (strcmp(argv[1], "remove") == 0)
        return 0;

    printf("Invalid command.\n");
    return 1;
}