# Chronicle — Ironhold World Engine

Chronicle is a high-performance C++20 world-state engine built for the open-world survival game **Ironhold**. It tracks players, entities, and world state while surviving server restarts and providing microsecond-level query responses.

---

## Repository Structure

```
chronicle/
├── include/
│   ├── application_state.hpp   # System/App state definitions (AppState enum)
│   ├── command.hpp             # Command definitions, parser, & executor interfaces
│   ├── console.hpp             # Interactive debug console declarations
│   ├── entity.hpp              # Player entity struct & WorldState state manager
│   └── input_buffer.hpp        # InputBuffer whitespace trimming & history logger
├── src/
│   ├── command.cpp             # Command parsing, validation, & execution
│   ├── console.cpp             # REPL console loop & dot-command handlers
│   ├── input_buffer.cpp        # InputBuffer helper implementations
│   └── main.cpp                # Application entry point
├── tests/                      # Automated milestone test scripts
├── Makefile                    # Build configuration
└── README.md                   # Engine documentation
```

---

## Milestone Progress

### Milestone 1 — The Game Debug Console (Completed)
- **Interactive REPL**: Displays `ironhold> ` prompt accepting user commands.
- **System Dot-Commands**:
  - `.quit`: Cleanly flushes history log and exits the engine.
  - `.help`: Displays summary of available engine commands.
  - `.version`: Displays `Chronicle v0.1 — Ironhold World Engine`.
  - `.status`: Displays world status (`World: offline`).
  - Graceful handling of unknown dot-commands returning `AppState::UNKNOWN` with `.help` guidance.
- **Input Buffer**: Trims leading/trailing whitespace and maintains session history.

### Milestone 2 — Entity Commands and In-Memory World State (Completed)
- **`Player` Entity Structure**:
  - `id`: Positive integer (unique per player).
  - `username`: String (up to 32 characters).
  - `email`: String (up to 255 characters).
- **In-Memory `WorldState`**:
  - Encapsulates player collection.
  - Enforces player ID uniqueness.
  - Returns active players sorted in ascending order by `id`.
- **Command Parser & Executor (`ParseResult` & `ExecResult`)**:
  - **`SPAWN PLAYER <id> <username> <email>`**: Validates positive ID, username length (<= 32), email length (<= 255), and ID uniqueness. Returns `Spawned.` on success or descriptive error.
  - **`LIST PLAYERS`**: Lists all active players sorted by ID ascending, followed by entity count (`<count> entities.`).
  - Structured `parseCommand` and `execute` API returning `ParseResult` (`SUCCESS` | `SYNTAX_ERROR`) and `ExecResult` (`SUCCESS` | `ERROR`).

---

## Build & Usage Instructions

### Building Chronicle
Requires a C++20 compliant compiler (`g++` or `clang++`).

```bash
make clean && make
```

### Running the Engine
```bash
./chronicle
```

### Example Session
```text
ironhold> .version
Chronicle v0.1 — Ironhold World Engine
ironhold> SPAWN PLAYER 2 bob bob@ironhold.gg
Spawned.
ironhold> SPAWN PLAYER 1 alice alice@ironhold.gg
Spawned.
ironhold> LIST PLAYERS
[1] alice <alice@ironhold.gg>
[2] bob <bob@ironhold.gg>
2 entities.
ironhold> SPAWN PLAYER -1 bad email
Error: ID must be positive.
ironhold> SPAWN PLAYER 3 areallylongnamethatexceedsthirtytwochars x@x.com
Error: username too long (max 32 chars).
ironhold> .quit
```

---

## Running Automated Tests

Run the milestone test suite:

```bash
cd tests
for t in *.sh; do bash "$t" ../chronicle; done
```
