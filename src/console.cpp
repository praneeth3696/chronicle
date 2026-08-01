#include "console.hpp"
#include "application_state.hpp"
#include "command.hpp"
#include "entity.hpp"
#include "input_buffer.hpp"
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Handles system dot-commands (prefixed with '.')
AppState handleDotCommand(const string &cmd, WorldState &worldState) {
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
    cout << "Chronicle v0.1 \u2014 Ironhold World Engine\n";
    return AppState::SUCCESS;
  }
  if (cmd == ".status") {
    if (worldState.hasPager()) {
      Pager *p = worldState.getPager();
      cout << "World: online \u2014 " << p->get_filename() << " ("
           << p->get_num_entities() << " entities, " << p->get_num_pages()
           << " pages)\n";
    } else {
      cout << "World: offline\n";
    }
    return AppState::SUCCESS;
  }

  cout << "Unknown command. Try .help for a summary of available commands.\n";
  return AppState::UNKNOWN;
}

// Main interactive REPL console loop
void console(const string &world_file) {
  string rawInput;
  vector<string> history;
  WorldState worldState(world_file);
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
      AppState state = handleDotCommand(trimmedInput, worldState);
      if (state == AppState::QUIT) {
        break;
      }
    } else {
      handleWorldCommand(trimmedInput, worldState);
    }
  }

  // Flush dirty binary pages to disk before shutdown
  worldState.close();
  buffer.processAndLog(history, "ironhold_clean.log");
}