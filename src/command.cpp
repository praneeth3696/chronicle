#include "command.hpp"
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

ParseResult parseCommand(const string& input, Command& outCmd, string& errorMsg) {
    stringstream ss(input);
    string action, target;

    if (!(ss >> action)) {
        errorMsg = "Error: Invalid command format.";
        return ParseResult::SYNTAX_ERROR;
    }

    if (action == "SPAWN") {
        if (!(ss >> target) || target != "PLAYER") {
            errorMsg = "Error: Invalid SPAWN format.";
            return ParseResult::SYNTAX_ERROR;
        }

        string rawId, username, email;
        if (!(ss >> rawId >> username >> email)) {
            errorMsg = "Error: Invalid SPAWN format.";
            return ParseResult::SYNTAX_ERROR;
        }

        string extra;
        if (ss >> extra) {
            errorMsg = "Error: Invalid SPAWN format.";
            return ParseResult::SYNTAX_ERROR;
        }

        long long id = 0;
        bool validId = true;
        try {
            size_t pos = 0;
            id = stoll(rawId, &pos);
            if (pos != rawId.length() || id <= 0) validId = false;
        } catch (...) {
            validId = false;
        }

        if (!validId) {
            errorMsg = "Error: ID must be positive.";
            return ParseResult::SYNTAX_ERROR;
        }

        if (username.length() > 32) {
            errorMsg = "Error: username too long (max 32 chars).";
            return ParseResult::SYNTAX_ERROR;
        }

        if (email.length() > 255) {
            errorMsg = "Error: email too long (max 255 chars).";
            return ParseResult::SYNTAX_ERROR;
        }

        outCmd.type = CommandType::SPAWN_PLAYER;
        outCmd.playerData = {id, username, email};
        return ParseResult::SUCCESS;
    }
    else if (action == "LIST") {
        if (!(ss >> target) || target != "PLAYERS") {
            errorMsg = "Error: Invalid LIST format.";
            return ParseResult::SYNTAX_ERROR;
        }

        string extra;
        if (ss >> extra) {
            errorMsg = "Error: Invalid LIST format.";
            return ParseResult::SYNTAX_ERROR;
        }

        outCmd.type = CommandType::LIST_PLAYERS;
        return ParseResult::SUCCESS;
    }

    errorMsg = "Error: Unknown world command.";
    return ParseResult::SYNTAX_ERROR;
}

ExecResult execute(const Command& cmd, WorldState& state) {
    if (cmd.type == CommandType::SPAWN_PLAYER) {
        if (!state.addPlayer(cmd.playerData)) {
            cout << "Error: Player ID already exists.\n";
            return ExecResult::ERROR;
        }
        cout << "Spawned.\n";
        return ExecResult::SUCCESS;
    }
    else if (cmd.type == CommandType::LIST_PLAYERS) {
        vector<Player> sorted = state.getPlayersSorted();
        for (const auto& p : sorted) {
            cout << "[" << p.id << "] " << p.username << " <" << p.email << ">\n";
        }
        cout << sorted.size() << " entities.\n";
        return ExecResult::SUCCESS;
    }

    cout << "Error: Unknown command execution.\n";
    return ExecResult::ERROR;
}

void handleWorldCommand(const string& input, WorldState& state) {
    Command cmd;
    string errorMsg;
    ParseResult pr = parseCommand(input, cmd, errorMsg);
    if (pr == ParseResult::SYNTAX_ERROR) {
        cout << errorMsg << "\n";
        return;
    }

    execute(cmd, state);
}
