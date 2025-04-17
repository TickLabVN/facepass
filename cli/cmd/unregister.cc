#include "unregister.h"

void setup_unregister_command(CLI::App &app, const std::string &username)
{
    CLI::App *unregister = app.add_subcommand("unregister", "Remove your face from the database.");
    unregister->callback([&]() {
        return remove_face(username);
    });
}