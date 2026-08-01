# Chronicle — Ironhold World Engine

Chronicle is a high-performance C++20 world-state engine built for the open-world survival game **Ironhold**. It tracks players, entities, and world state while surviving server restarts through hardware-friendly 4KB binary disk pages, providing microsecond-level query responses.

---

## Repository Structure

```
chronicle/
├── include/
│   ├── application_state.hpp   # System/App state definitions (AppState enum)
│   ├── command.hpp             # Command definitions, parser, & executor interfaces
│   ├── console.hpp             # Interactive debug console declarations
│   ├── entity.hpp              # Player entity struct, binary serialization, & Pager-backed WorldState
│   ├── input_buffer.hpp        # InputBuffer whitespace trimming & history logger
│   └── pager.hpp               # Binary Page (4KB) & Pager persistence manager header
├── src/
│   ├── command.cpp             # Command parsing, validation, & execution
│   ├── console.cpp             # REPL console loop & dot-command handlers
│   ├── input_buffer.cpp        # InputBuffer helper implementations
│   ├── main.cpp                # Application entry point & CLI argument parser
│   └── pager.cpp               # Binary page caching, lazy loading, & disk flush implementation
├── tests/                      # Milestone test scripts
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

### Milestone 3 — The Persistent World (Completed)

- **Binary Entity Layout (`PlayerRecord`)**:
  - Each player record is packed into exactly **291 bytes**:
    - `Offset 0, Size 4`   : `id` (32-bit Little-Endian `uint32_t`).
    - `Offset 4, Size 32`  : `username` (null-padded to 32 bytes).
    - `Offset 36, Size 255`: `email` (null-padded to 255 bytes).
- **Page & Pager Engine Architecture**:
  - **Page Size**: Hardware-aligned 4KB pages (`PAGE_SIZE = 4096 bytes`).
  - **Entities per Page**: `ENTITIES_PER_PAGE = 4096 / 291 = 14` entities per page (22 bytes tail padding).
  - **Engine Limits**: Up to `MAX_PAGES = 100` pages (`MAX_ENTITIES = 1400` max world entities).
  - **Lazy Loading**: Pages load into RAM on demand when an entity index within that page is accessed.
  - **Clean Flush on Shutdown**: Calling `.quit` flushes all dirty pages to the save file.
- **CLI Storage Initialization & Updated `.status`**:
  - Engine launch accepts storage file via CLI argument: `./chronicle <file.world>`.
  - Automatically creates file if it does not exist.
  - Dynamic `.status` output:
    - Online: `World: online — ironhold.world (42 entities, 3 pages)`
    - Offline: `World: offline` (when launched without world file argument).

---

## Build & Usage Instructions

### Building Chronicle

Requires a C++20 compliant compiler (`g++` or `clang++`).

```bash
make clean && make
```

### Running the Engine with World Storage

```bash
./chronicle ironhold.world
```

### Example Session Across Server Restarts

**Session 1 — Spawning Entities & Saving World:**
```text
$ ./chronicle ironhold.world
ironhold> SPAWN PLAYER 1 alice alice@ironhold.gg
Spawned.
ironhold> SPAWN PLAYER 2 bob bob@ironhold.gg
Spawned.
ironhold> .status
World: online — ironhold.world (2 entities, 1 pages)
ironhold> .quit
$
```

**Session 2 — Restarting Engine & Restoring World:**
```text
$ ./chronicle ironhold.world
ironhold> LIST PLAYERS
[1] alice <alice@ironhold.gg>
[2] bob <bob@ironhold.gg>
2 entities.
ironhold> .status
World: online — ironhold.world (2 entities, 1 pages)
ironhold> .quit
$
```

---

## Running Automated Tests

Run the milestone test suite:

```bash
cd tests
for t in *.sh; do bash "$t" ../chronicle; done
```
