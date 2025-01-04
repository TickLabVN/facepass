#include <stdio.h>
#include <string.h>
#include "add_face.h"
#include "CLI/App.hpp"
#include "CLI/CLI.hpp"

int main(int argc, char **argv)
{
    CLI::App app{"Facepass CLI"};
    CLI::App *add = app.add_subcommand("add", "Add your face to the database.");
    CLI::App *remove = app.add_subcommand("remove", "Remove your face from the database.");

    add->callback([&]() {
        return add_face();
    });
    remove->callback([&]() {
        printf("Remove face.\n");
        return 1;
    });
    CLI11_PARSE(app, argc, argv);

    return 0;
}