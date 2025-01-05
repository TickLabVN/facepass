#include "identify.h"

int scanFace(const string& username, int8_t retries = 5)
{
    cv::VideoCapture camera(0); // in linux check $ ls /dev/video0
    if (!camera.isOpened())
    {
        std::cerr << "ERROR: Could not open camera" << std::endl;
        return 1;
    }

    cv::Mat preparedFace = cv::imread(user_face_path(username));
    if (preparedFace.empty())
    {
        std::cerr << "ERROR: Face not found" << std::endl;
        return 1;
    }
    FaceRecognition matcher(model_path(username, FACE_RECOGNITION));
    while (retries--)
    {
        cv::Mat loginFace;
        camera >> loginFace;
        if (loginFace.empty())
        {
            std::cerr << "ERROR: Could not read frame" << std::endl;
            break;
        }
        MatchResult match = matcher.match(preparedFace, loginFace);
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
    const int8_t maxRetries = 5;
    vector<string> usernames = get_usernames();
    for (const string &username : usernames)
        if (scanFace(username, maxRetries) == 0)
            return PAM_SUCCESS;

    return PAM_AUTH_ERR;
}