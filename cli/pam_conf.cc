#include "pam_conf.h"

void trim_string(string &str)
{
    // Trim leading whitespace
    str.erase(0, str.find_first_not_of(" \t"));
    // Trim trailing whitespace
    str.erase(str.find_last_not_of(" \t") + 1);
}

int count_module_above_pam_deny()
{
    ifstream file("/etc/pam.d/common-auth");
    if (!file.is_open())
    {
        cerr << "Error: Unable to open /etc/pam.d/common-auth for reading.\n";
        return -1;
    }

    string line;
    int count = 0;
    while (getline(file, line))
    {
        // Trim leading and trailing whitespace
        trim_string(line);

        // Skip comments and empty lines
        if (line.empty() || line[0] == '#')
            continue;

        if (line.find("pam_deny.so") != string::npos)
            break;

        if (line.find("auth") != string::npos)
            count++;
    }

    file.close();
    return count;
}

string get_pam_config_file_path(string module)
{
    string cfg_file;
    if (module == "login")
    {
        // Display Manager  | Commonly Used With    | PAM Config File
        // GDM              | GNOME                 | /etc/pam.d/gdm-password
        // SDDM             | KDE Plasma            | /etc/pam.d/sddm
        // LightDM          | XFCE, MATE, Budgie    | /etc/pam.d/lightdm
        // LXDM             | LXDE                  | /etc/pam.d/lxdm
        // XDM              | Basic X environments  | /etc/pam.d/xdm
        // Entrance         | Enlightenment (E17+)  | /etc/pam.d/entrance
        const vector<string> pam_files = {
            "/etc/pam.d/gdm-password",
            "/etc/pam.d/sddm",
            "/etc/pam.d/lightdm",
            "/etc/pam.d/lxdm",
            "/etc/pam.d/xdm",
            "/etc/pam.d/entrance"};
        for (const auto &pam_file : pam_files)
        {
            struct stat buffer;
            if (stat(pam_file.c_str(), &buffer) != 0)
                continue;
            cfg_file = pam_file;
            break;
        }
    }
    else if (module == "sudo")
        cfg_file = "/etc/pam.d/sudo";
    else if (module == "su")
        cfg_file = "/etc/pam.d/su";
    return cfg_file;
}

bool disable_auth(const string &module)
{
    string cfg_file_path = get_pam_config_file_path(module);
    if (cfg_file_path.empty())
    {
        cerr << "Error: Unable to determine PAM configuration file for module: " << module << "\n";
        return false;
    }
    fstream cfg_file(cfg_file_path);
    if (!cfg_file.is_open())
    {
        cerr << "Error: Unable to open " << cfg_file_path << " for reading.\n";
        return false;
    }

    string file_content;
    string line;

    // Read the file and check if the PAM configuration already exists
    while (getline(cfg_file, line))
    {
        trim_string(line);
        if (line.find("libfacepass_pam.so") != string::npos)
        {
            if (line[0] != '#')
                line = "# " + line;
        }

        file_content.append(line);
        file_content.append("\n");
    }

    // Write the modified content back to the file
    cfg_file << file_content;
    cfg_file.close();

    cout << "Successfully disabled face authentication for " << module << ".\n";
    return true;
}

bool enable_auth(const string &module, const PAMConfig &config)
{
    string cfg_file_path = get_pam_config_file_path(module);
    if (cfg_file_path.empty())
    {
        cerr << "Error: Unable to determine PAM configuration file for module: " << module << "\n";
        return false;
    }

    fstream cfg_file(cfg_file_path);
    if (!cfg_file.is_open())
    {
        cerr << "Error: Unable to open " << cfg_file_path << " for reading.\n";
        return false;
    }

    string file_content;
    string line;
    bool config_exists = false;

    // Read the file and check if the PAM configuration already exists
    const int module_count = count_module_above_pam_deny();

    while (getline(cfg_file, line))
    {
        trim_string(line);
        if (line.find("libfacepass_pam.so") != string::npos)
        {
            char pam_config_line[256];
            config_exists = true;
            sprintf(pam_config_line,
                    "auth [success=%d default=ignore] libfacepass_pam.so retries=%d retry_delay=%d%s",
                    module_count, config.num_retries,
                    config.gap, config.anti_spoof ? " anti_spoof" : "");
            line = pam_config_line;
        }

        file_content.append(line);
        file_content.append("\n");
    }

    bool success = true;
    if (!config_exists)
    {
        // Insert the PAM configuration line before "@include common-auth"
        size_t pos = file_content.find("@include common-auth");
        if (pos != string::npos)
        {
            char pam_config_line[256];
            sprintf(pam_config_line,
                    "auth [success=%d default=ignore] libfacepass_pam.so retries=%d retry_delay=%d%s\n",
                    module_count, config.num_retries,
                    config.gap, config.anti_spoof ? " anti_spoof" : "");
            file_content.insert(pos, pam_config_line);
        }
        else
        {
            cerr << "Error: Invalid PAM configuration file: '@include common-auth' not found.\n";
            success = false;
        }
    }

    // Write the modified content back to the file
    cfg_file << file_content;
    cfg_file.close();

    return success;
}

bool is_auth_enabled(const string &module)
{
    string cfg_file_path = get_pam_config_file_path(module);
    if (cfg_file_path.empty())
    {
        cerr << "Error: Unable to determine PAM configuration file for module: " << module << "\n";
        return false;
    }

    ifstream cfg_file(cfg_file_path);
    if (!cfg_file.is_open())
    {
        cerr << "Error: Unable to open " << cfg_file_path << " for reading.\n";
        return false;
    }

    string line;
    bool result = false;
    while (getline(cfg_file, line))
    {
        trim_string(line);
        if (line.find("libfacepass_pam.so") != string::npos)
        {
            result = line[0] != '#';
            break;
        }
    }

    cfg_file.close();
    return result;
}