#ifndef COMMAND_HPP
#define COMMAND_HPP

#include "entity.hpp"
#include <string>
#include <vector>

enum class CommandType {
    SPAWN_PLAYER,
    LIST_PLAYERS,
    UNKNOWN
};

enum class ParseResult {
    SUCCESS,
    SYNTAX_ERROR
};

enum class ExecResult {
    SUCCESS,
    ERROR
};

struct Command {
    CommandType type = CommandType::UNKNOWN;
    Player playerData;
};

ParseResult parseCommand(const std::string& input, Command& outCmd, std::string& errorMsg);
ExecResult execute(const Command& cmd, WorldState& state);

void handleWorldCommand(const std::string& input, WorldState& state);

#endif // COMMAND_HPP
