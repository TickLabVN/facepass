#include "config.h"
#include <CLI/App.hpp>

void setup_config_command(CLI::App &app)
{
    auto module_list = CLI::IsMember({"sudo", "su", "login"});
    CLI::App *config = app.add_subcommand("config", "Configure face authentication.");
    config->add_option("module", "Module to configure face authentication for (sudo, su, login).")
        ->check(module_list)
        ->default_val("login");
    config->add_option("-r,--retries", "Number of retries before failing.")
        ->default_val(10)
        ->check(CLI::Range(1, 100));
    config->add_option("-d,--delay", "Delay between retries in milliseconds.")
        ->default_val(200)
        ->check(CLI::Range(0, 10000));
    bool anti_spoof = false;
    config->add_flag("-a,--anti-spoof", anti_spoof, "Enable anti-spoofing detection.\nFalse by default because some old cameras may not have enough resolution to detect spoofing.")
        ->default_val(false);
    
    config->callback([&]() {
        string module = config->get_option("module")->as<string>();
        int retries = config->get_option("-r,--retries")->as<int>();
        int delay = config->get_option("-d,--delay")->as<int>();
        
        PAMConfig pam_config{
            .num_retries = retries,
            .gap = delay,
            .anti_spoof = anti_spoof
        };

        bool result = enable_auth(module, pam_config);
        if (result)
            cout << "Face authentication configured for " << module << ".\n";
        else
            cerr << "Failed to configure face authentication for " << module << ".\n";
    });
}