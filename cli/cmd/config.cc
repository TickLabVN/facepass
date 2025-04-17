#include "config.h"

void setup_config_command(CLI::App &app)
{
    auto module_list = CLI::IsMember({"sudo", "su", "login"});
    CLI::App *config = app.add_subcommand("config", "Configure face authentication.");

    auto auth_module = std::make_shared<std::string>();
    auto retries = std::make_shared<int>();
    auto delay = std::make_shared<int>();
    auto anti_spoof = std::make_shared<bool>();

    config->add_option("module", *auth_module, "Module to configure face authentication for (sudo, su, login).")
        ->check(module_list)
        ->default_val("login");
    auto retries_option = config->add_option("-r,--retries", *retries, "Number of retries before failing.")
                              ->check(CLI::Range(1, 100));
    auto delay_option = config->add_option("-d,--delay", *delay, "Delay between retries in milliseconds.")
                            ->check(CLI::Range(0, 10000));
    auto anti_spoof_flag = config->add_flag("--anti-spoof", *anti_spoof, "Enable anti-spoofing detection.\nFalse by default because some old cameras may not have enough resolution to detect spoofing.");
    config->require_option(1, 5);

    config->callback(
        [auth_module, retries, delay, anti_spoof,
         retries_option, delay_option, anti_spoof_flag]()
        {
            if (*auth_module != "login")
            {
                cerr << "Face authentication for " << *auth_module << " is not supported yet.\n";
                return;
            }
            if(!is_auth_enabled(*auth_module))
            {
                cerr << "Face authentication for " << *auth_module << " is not enabled yet.\n";
                cerr << "Please use 'facepass-cli enable " << *auth_module << "'.\n";
                return;
            }
            auto pam_config = load_current_config(*auth_module);
            // Error occurred while loading the config
            if (pam_config == NULL)
                return;

            if (retries_option->count() > 0)
                pam_config->num_retries = *retries;
            if (delay_option->count() > 0)
                pam_config->gap = *delay;
            if (anti_spoof_flag->count() > 0)
                pam_config->anti_spoof = *anti_spoof;
            bool result = enable_auth(*auth_module, *pam_config);
            if (result)
                cout << "Face authentication configured for " << *auth_module << ".\n";
            else
                cerr << "Failed to configure face authentication for " << *auth_module << ".\n";
            delete pam_config;
        });
}