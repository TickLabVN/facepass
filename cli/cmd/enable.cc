#include "enable.h"


void setup_enable_command(CLI::App &app)
{
    auto module_list = CLI::IsMember({"sudo", "su", "login", "all"});
    CLI::App *enable = app.add_subcommand("enable", "Enable face authentication for modules.");
    enable->add_option("module", "Module to enable face authentication for (sudo, su, login).")
        ->check(module_list)
        ->default_val("login");

    enable->callback([&]() {
        string module = enable->get_option("module")->as<string>();
        PAMConfig config{
            .num_retries = 10,
            .gap = 200,
            .anti_spoof = false
        };
        bool result = enable_auth(module, config);
        if (result)
            cout << "Face authentication enabled for " << module << ".\n";
        else
            cerr << "Failed to enable face authentication for " << module << ".\n";
    });
}