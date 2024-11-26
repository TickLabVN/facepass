
#include <opencv2/opencv.hpp>
#include <iostream>

int main()
{
    cv::VideoCapture camera(0); // in linux check $ ls /dev/video0
    if (!camera.isOpened())
    {
        std::cerr << "ERROR: Could not open camera" << std::endl;
        return 1;
    }

    // create a window to display the images from the webcam
    cv::namedWindow("Webcam", cv::WINDOW_AUTOSIZE);

    // Matrix to hold image
    cv::Mat frame;

    // display the frame until you press a key
    while (1)
    {
        // capture the next frame from the webcam
        camera >> frame;
        // show the image on the window
        cv::imshow("Webcam", frame);
        // wait (10ms) for esc key to be pressed to stop
        if (cv::waitKey(10) == 27)
            break;
    }
    return 0;
}