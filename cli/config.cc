#include "config.h"

string get_config_path()
{
    // $HOME/.config
    string home = getenv("HOME");
    home.append("/.config/facepass");
    return home;
}
