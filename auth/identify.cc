#include "identify.h"

namespace uuid
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    static std::uniform_int_distribution<> dis(0, 15);
    static std::uniform_int_distribution<> dis2(8, 11);

    std::string v4()
    {
        std::stringstream ss;
        ss << std::hex;

        for (int i = 0; i < 8; i++)
            ss << dis(gen);

        ss << "-";
        for (int i = 0; i < 4; i++)
            ss << dis(gen);

        ss << "-4";
        for (int i = 0; i < 3; i++)
            ss << dis(gen);

        ss << "-";
        ss << dis2(gen);
        for (int i = 0; i < 3; i++)
            ss << dis(gen);

        ss << "-";
        for (int i = 0; i < 12; i++)
            ss << dis(gen);

        return ss.str();
    }
}

namespace
{
    bool save_failed_face(const std::string &username, const cv::Mat &face)
    {
        std::string failedFacePath = debug_path(username) + "/" + uuid::v4() + ".jpg";
        if (!cv::imwrite(failedFacePath, face))
        {
            std::cerr << "ERROR: Could not save failed face to " << failedFacePath << std::endl;
            return false;
        }
        return true;
    }

    bool process_anti_spoofing(FaceAntiSpoofing &faceAs, cv::Mat &face)
    {
        SpoofResult spoofCheck = faceAs.inference(face);
        if (spoofCheck.spoof)
        {
            std::cerr << "ERROR: Spoof detected, score: " << spoofCheck.score << std::endl;
            return false;
        }
        return true;
    }
    void sleep_for(int ms)
    {
        this_thread::sleep_for(chrono::milliseconds(ms));
    }
}

int scan_face(const std::string &username, int8_t retries, const int gap, bool anti_spoofing)
{
    cv::VideoCapture camera(0, cv::CAP_V4L2); // in Linux check $ ls /dev/video0
    if (!camera.isOpened())
    {
        std::cerr << "ERROR: Could not open camera" << std::endl;
        return PAM_AUTH_ERR;
    }

    std::string userFacePath = user_face_path(username);
    cv::Mat preparedFace = cv::imread(userFacePath);
    if (preparedFace.empty())
    {
        std::cerr << "ERROR: Face not register for user " << username << std::endl;
        return PAM_AUTH_ERR;
    }

    FaceRecognition faceReg(model_path(username, FACE_RECOGNITION));
    FaceDetection faceDetector(model_path(username, FACE_DETECTION));
    std::unique_ptr<FaceAntiSpoofing> faceAs = nullptr;

    if (anti_spoofing)
    {
        faceAs = std::make_unique<FaceAntiSpoofing>(model_path(username, FACE_ANTI_SPOOFING));
    }

    bool success = false;
    while (retries--)
    {
        cv::Mat loginFace;
        camera >> loginFace;
        if (loginFace.empty())
        {
            std::cerr << "ERROR: Could not read frame" << std::endl;
            break;
        }

        std::vector<Detection> detectedImages = faceDetector.inference(loginFace);
        if (detectedImages.empty())
        {
            std::cerr << "ERROR: No face detected" << std::endl;
            sleep_for(gap);
            continue;
        }

        cv::Mat face = detectedImages[0].image;
        if (anti_spoofing && !process_anti_spoofing(*faceAs, face))
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(gap));
            sleep_for(gap);
            continue;
        }

        MatchResult match = faceReg.match(preparedFace, face);
        if (match.similar)
        {
            success = true;
            break;
        }

        save_failed_face(username, face);
        sleep_for(gap);
    }

    return success ? PAM_SUCCESS : PAM_AUTH_ERR;
}
