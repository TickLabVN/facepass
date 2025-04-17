#include "disable.h"
#include "pam_conf.h"
#include <CLI/App.hpp>

void setup_disable_command(CLI::App &app)
{
    auto module_list = CLI::IsMember({"sudo", "su", "login", "all"});
    CLI::App *disable = app.add_subcommand("disable", "Disable face authentication for modules.");
    disable->add_option("module", "Module to disable face authentication for (sudo, su, login).")
        ->check(module_list)
        ->default_val("login");
    disable->callback([&]() {
        bool result = disable_auth(disable->get_option("module")->as<string>());
        if (result)
            cout << "Face authentication disabled for " << disable->get_option("module")->as<string>() << ".\n";
        else
            cerr << "Failed to disable face authentication for " << disable->get_option("module")->as<string>() << ".\n";
    });
}