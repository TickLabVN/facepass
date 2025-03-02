#include <stdio.h>
#include <string.h>
#include "add_face.h"
#include "remove_face.h"
#include "face_config.h"
#include "CLI/App.hpp"
#include "CLI/CLI.hpp"

int main(int argc, char **argv)
{
    CLI::App app{"Facepass CLI"};
    app.require_subcommand(1);


    CLI::App *add = app.add_subcommand("add", "Add your face to the database.");
    CLI::App *remove = app.add_subcommand("remove", "Remove your face from the database.");
    
    char *user = getenv("USER");
    if (user == nullptr) {
        fprintf(stderr, "Failed to get the current username.\n");
        return 1;
    }
    string username = string(user);
    setup_config(username);

    add->callback([&]() {
        return add_face(username);
    });
    remove->callback([&]() {
        return remove_face(username);
    });
    CLI11_PARSE(app, argc, argv);

    return 0;
}