#include <iostream>
#include <string>
#include <vector>
#include "console.hpp"
#include "input_buffer.hpp"
#include "command.hpp"
#include "entity.hpp"
#include "application_state.hpp"

using namespace std;

AppState handleDotCommand(const string& cmd) {
    if (cmd == ".quit") {
        return AppState::QUIT;
    }
    if (cmd == ".help") {
        cout << "  .quit       Shut down the world engine\n";
        cout << "  .help       Show this message\n";
        cout << "  .version    Show engine version\n";
        cout << "  .status     Show world status\n";
        return AppState::SUCCESS;
    }
    if (cmd == ".version") {
        cout << "Chronicle v0.1 — Ironhold World Engine\n";
        return AppState::SUCCESS;
    }
    if (cmd == ".status") {
        cout << "World: offline\n";
        return AppState::SUCCESS;
    }

    cout << "Unknown command. Try .help for a summary of available commands.\n";
    return AppState::UNKNOWN;
}

void console() {
    string rawInput;
    vector<string> history;
    WorldState worldState;
    InputBuffer buffer;

    while (true) {
        cout << "ironhold> ";
        if (!getline(cin, rawInput)) {
            break;
        }

        string trimmedInput = InputBuffer::trim(rawInput);
        if (trimmedInput.empty()) {
            continue;
        }

        history.push_back(trimmedInput);

        if (trimmedInput[0] == '.') {
            AppState state = handleDotCommand(trimmedInput);
            if (state == AppState::QUIT) {
                break;
            }
        } else {
            handleWorldCommand(trimmedInput, worldState);
        }
    }

    buffer.processAndLog(history, "ironhold_clean.log");
}