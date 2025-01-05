#include "identify.h"

#include <random>
#include <sstream>

namespace uuid
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

    std::string v4()
    {
        std::stringstream ss;
        int i;
        ss << std::hex;
        for (i = 0; i < 8; i++)
            ss << dis(gen);

        ss << "-";
        for (i = 0; i < 4; i++)
            ss << dis(gen);

        ss << "-4";
        for (i = 0; i < 3; i++)
            ss << dis(gen);

        ss << "-";
        ss << dis2(gen);
        for (i = 0; i < 3; i++)
            ss << dis(gen);
        ss << "-";
        for (i = 0; i < 12; i++)
            ss << dis(gen);
        return ss.str();
    }
}

int scanFace(const string &username, int8_t retries = 5)
{
    printf("Scanning face for %s\n", username.c_str());
    cv::VideoCapture camera(0); // in linux check $ ls /dev/video0
    if (!camera.isOpened())
    {
        std::cerr << "ERROR: Could not open camera" << std::endl;
        return 1;
    }

    string userFacePath = user_face_path(username);
    printf("Loading face from %s\n", userFacePath.c_str());

    cv::Mat preparedFace = cv::imread(userFacePath);
    if (preparedFace.empty())
    {
        std::cerr << "ERROR: Face not found" << std::endl;
        return 1;
    }
    FaceRecognition faceReg(model_path(username, FACE_RECOGNITION));
    FaceDetection faceDetector(model_path(username, FACE_DETECTION));

    while (retries--)
    {
        cv::Mat loginFace;
        camera >> loginFace;

        std::vector<Detection> detectedImages = faceDetector.inference(loginFace);
        cv::Mat face = detectedImages[0].image;

        if (loginFace.empty())
        {
            std::cerr << "ERROR: Could not read frame" << std::endl;
            break;
        }
        MatchResult match = faceReg.match(preparedFace, face);
        if (match.similar)
            return 0;

        string failedFace = debug_path(username) + "/" + uuid::v4() + ".jpg";
        printf("Saving failed face to %s\n", failedFace.c_str());
        int ret = cv::imwrite(failedFace, face);
        if (ret == 0) std::cerr << "ERROR[]" << ret << "]: Could not save failed face" << std::endl;

        printf("Match distance: %f\n", match.dist);
        printf("Face not recognized. Retrying...\n");
        this_thread::sleep_for(chrono::milliseconds(200));
    }
    std::cerr << "ERROR: Face not recognized" << std::endl;
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