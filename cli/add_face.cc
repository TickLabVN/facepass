#include "add_face.h"

int capture_face(cv::Mat &frame)
{
    cv::VideoCapture camera(0, cv::CAP_V4L2); // in linux check $ ls /dev/video0

    int retries = 0;
    while (!camera.isOpened())
    {
        if (retries > 5)
        {
            std::cout << "Failed to open camera." << std::endl;
            return 1;
        }
        std::cout << "Trying to open camera..." << std::endl;
        camera.open(0);
        sleep(1);
        retries++;
    }
    printf("Camera opened.\n");

    while (1)
    {
        // capture the next frame from the webcam
        camera >> frame;
        // show the image on the window
        cv::imshow("Facepass", frame);
        // wait (10ms) for esc key to be pressed to stop
        if (cv::waitKey(10) == 27)
            break;
    }
    return 0;
}
