#include "face_config.h"

int mkdir_p(const string &path)
{
    size_t pos = 0;
    string dir;
    int ret;
    while ((pos = path.find('/', pos)) != string::npos)
    {
        dir = path.substr(0, pos++);
        if (dir.empty())
            continue;
        ret = mkdir(dir.c_str(), 0777);
        if (ret == -1 && errno != EEXIST)
        {
            perror("Failed to create directory");
            return 1;
        }
    }
    ret = mkdir(path.c_str(), 0777);
    if (ret == -1 && errno != EEXIST)
    {
        perror("Failed to create directory");
        return 1;
    }
    return 0;
}

string user_face_path(const string &username)
{
    return string("/home/") + username + "/.config/facepass/faces/face.jpg";
}

vector<string> get_usernames()
{
    vector<string> usernames;
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("/home")) != NULL)
    {
        while ((ent = readdir(dir)) != NULL)
            if (ent->d_type == DT_DIR && ent->d_name[0] != '.')
                usernames.push_back(ent->d_name);
        closedir(dir);
    }
    else
        perror("Could not open directory");
    return usernames;
}

string model_path(const string &username, const ModelType &modelType)
{
    string modelTypeStr;
    switch (modelType)
    {
    case FACE_DETECTION:
        modelTypeStr = "yolov11n-face.torchscript";
        break;
    case FACE_RECOGNITION:
        modelTypeStr = "edgeface_s_gamma_05_ts.pt";
        break;
    case FACE_ANTI_SPOOFING:
        modelTypeStr = "mobilenet_v3_small.onnx";
        break;
    }
    return string("/home/") + username + "/.config/facepass/models/" + modelTypeStr;
}

int setup_config(const string &username)
{
    const string configDir = string(getenv("HOME")) + "/.config/facepass";
    if (mkdir_p(configDir) != 0)
        return 1;
    string faceDir = configDir + "/faces";
    string modelDir = configDir + "/models";

    if (mkdir_p(faceDir) != 0)
        return 1;
    return mkdir_p(modelDir);
}
