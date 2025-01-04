#include "identify.h"

int scanFace()
{
    cv::VideoCapture camera(0); // in linux check $ ls /dev/video0
    if (!camera.isOpened())
    {
        std::cerr << "ERROR: Could not open camera" << std::endl;
        return 1;
    }

    string configPath = get_config_path();
    cv::Mat preparedFace = cv::imread(configPath + "/faces/face.jpg");
    if (preparedFace.empty())
    {
        std::cerr << "ERROR: Face not found" << std::endl;
        return 1;
    }
    FaceRecognition faceRecognitionModel(configPath + "/weights/edgeface_s_gamma_05_ts.pt");

    int8_t retries = 5;
    while (retries--)
    {
        cv::Mat loginFace;
        camera >> loginFace;
        if (loginFace.empty())
        {
            std::cerr << "ERROR: Could not read frame" << std::endl;
            break;
        }
        MatchResult match = faceRecognitionModel.match(preparedFace, loginFace);
        if (match.similar) return 0;
        
        this_thread::sleep_for(chrono::milliseconds(200));
    }
    return 1;
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

    printf("Welcome %s\n", pUsername);
    retval = scanFace();
    if (retval != PAM_SUCCESS)
        return PAM_AUTH_ERR;

    return PAM_SUCCESS;
}