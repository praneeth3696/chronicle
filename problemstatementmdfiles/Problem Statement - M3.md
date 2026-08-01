# **Build a Persistent Game World Engine in C++**

> **Theme:** You're building the backend that keeps a massively multiplayer game world alive across server restarts, lag spikes, and concurrent player actions  
> **Language:** C++20

## **The Story**

Your game studio is shipping **Ironhold** — an open-world survival game. The server needs to track players, items, quests, and NPCs across restarts. The naive approach (dump everything to JSON on shutdown) already fails at 500 concurrent players. You've been tasked with building **Chronicle**, Ironhold's world-state engine, from scratch.

Each milestone ships a real, testable feature. By the end, Chronicle will handle thousands of entities, survive crashes, and answer complex world-state queries in microseconds.

**Judging:** Correctness, code quality, milestone completion, bonus features

## **Rules**

1. Standard C++20 STL is allowed  
2. Each milestone must pass its test suite before the next begins  
3. All milestones build a single evolving binary — no rewrites from scratch  
4. The final binary must handle Ironhold's acceptance tests (provided per milestone)

## **Repository Structure**

\`\`\`  
`chronicle/`  
`├── include/`  
`│   ├── application_state.hpp`  
`├── src/`  
`│   ├── main.cpp`  
`├── tests/`  
`├── Makefile`  
`└── README.md`  
\`\`\`

## **Milestone 1 — The Game Debug Console**

**Goal:** Build the interactive terminal that game developers and server admins use to inspect and command the running world.

### **Context**

Every game server ships a debug console — a live shell where you can type commands and get immediate feedback on world state. Ironhold's console is your entry point to Chronicle

### **Tasks**

1. **InputBuffer class**

   * Stores the raw command string and trims leading/trailing whitespace  
   * Tracks command history length (for display purposes)  
2. **Console loop** (`console.cpp`)

   * Continuously display `ironhold>` prompt  
   * Read one line of input  
   * Dispatch to either a *system command* (prefixed with `.`) or a *world command*  
   * Loop until `.quit` is entered  
3. **System commands**

   * `.quit` — flush open state and exit cleanly  
   * `.help` — print a summary of available commands  
   * `.version` — print `Chronicle v0.1 — Ironhold World Engine`  
   * `.status` — print `World: offline` (will update in later milestones)  
   * Return `AppState::UNKNOWN` for unrecognized dot-commands with a hint to try `.help`  
4. **World command stub**

   * Any non-dot input prints `[World command queued: <input>]` as a placeholder

### **Deliverables**

* Running binary that accepts input in a loop  
* `.quit`, `.help`, `.version`, `.status` work correctly  
* Unknown commands show a meaningful error without crashing

### **Acceptance Criteria**

```` ``` ````  
`$ ./chronicle`  
`ironhold> .help`  
  `.quit       Shut down the world engine`  
  `.help       Show this message`  
  `.version    Show engine version`  
  `.status     Show world status`  
`ironhold> .version`  
`Chronicle v0.1 — Ironhold World Engine`  
`ironhold> spawn player 1 alice`  
`[World command queued: spawn player 1 alice]`  
`ironhold> .quit`  
`$`  
```` ``` ````

## **Milestone 2 — Entity Commands and the World State**

**Goal:** Parse `SPAWN` and `LIST` commands and execute them against an in-memory world state.

### **Context**

The first entity type Chronicle must track is **Player** — the most critical world entity. Players need to be spawned (created), and the game server needs to list all active players at any time. This milestone ships Chronicle's first working world commands.

### **Fixed Entity Type (this milestone only)**

`Player:`  
  `id        (unsigned integer, unique)`  
  `username  (up to 32 characters)`  
  `email     (up to 255 characters)`

All data lives in memory of this milestone. No persistence yet — a server restart wipes the world.

### **Tasks**

1. **Entity and Command types**  
2. **Command parser** (`command.cpp`)  
   * Parse `SPAWN PLAYER <id> <username> <email>` (space-delimited)  
   * Parse `LIST PLAYERS` (full world scan, no filters yet)  
   * Return `ParseResult::SYNTAX_ERROR` with a clear message on malformed input  
   * Validate: id must be a positive integer, username ≤ 32 chars, email ≤ 255 chars

```` ``` ````  
`ParseResult = SUCCESS | SYNTAX_ERROR`  
```` ``` ````

3. **In-memory WorldState**  
   * WorldState currently tracks the list of active players  
4. **Command executor**  
   * `execute(const Command&, WorldState&)` → `ExecResult`

### **Deliverables**

* SPAWN and LIST work against the in-memory world state  
* Validation errors are reported clearly

### **Acceptance Criteria**

```` ``` ````  
`ironhold> SPAWN PLAYER 1 alice alice@ironhold.gg`  
`Spawned.`  
`ironhold> SPAWN PLAYER 2 bob bob@ironhold.gg`  
`Spawned.`  
`ironhold> LIST PLAYERS`  
`[1] alice <alice@ironhold.gg>`  
`[2] bob <bob@ironhold.gg>`  
`2 entities.`  
`ironhold> SPAWN PLAYER -1 bad email`  
`Error: ID must be positive.`  
`ironhold> SPAWN PLAYER 3 areallylongnamethatexceedsthirtytwochars x@x.com`  
`Error: username too long (max 32 chars).`  
```` ``` ````

## **Milestone 3 — The Persistent World**

**Goal:** Serialize entity records to fixed-size binary pages and persist them to disk so the world survives server restarts.

### **Context**

Right now, every time the **Ironhold** server restarts, the entire world vanishes. That's unacceptable. Chronicle needs to persist world state to disk — but not naively (writing JSON is too slow at scale). Real game engines and high-performance servers pack data into fixed-size *pages* that map directly to disk blocks, so reads and writes happen at hardware-friendly boundaries.

### **Tasks**

1. **Binary entity layout** (`entity.cpp`)

   ```` ``` ````

`Pack each PlayerRecord into exactly 291 bytes:`

 `Offset  Size  Field`  
`0       4     id         (uint32_t, little-endian)`  
`4       32    username   (null-padded to 32 bytes)`  
`36      255   email      (null-padded to 255 bytes)`  
```` ``` ````

2. **Page layout constants**

`PAGE_SIZE        = 4096 bytes`  
`ENTITIES_PER_PAGE = PAGE_SIZE / 291   // = 14`  
`MAX_PAGES        = 100`  
`MAX_ENTITIES     = ENTITIES_PER_PAGE * MAX_PAGES  // = 1400`

1. **Pager class** (`pager.cpp`)  
    The Pager is the bridge between memory and disk. It holds a pool of 4KB pages and loads them from the save file on demand.

   * Pages load lazily from disk on first access  
   * `close()` flushes all in-memory pages to the save file  
2. **Update WorldState to use Pager**

   * `WorldState` owns a `Pager` and accepts a filename on construction  
   * `entity_slot(uint32_t index)` returns a pointer into the correct page at the right offset  
   * On open: restore `count` from file size ÷ entity size  
   * On `.quit`: flush all pages via `pager.close()`

**Open world via CLI argument**

 $ ./chronicle ironhold.world  
 File is created if it doesn't exist (fresh world). The `.status` system command should now print:

 World: online — ironhold.world (42 entities, 3 pages)

3. 

### **Deliverables**

* Spawned entities survive server restarts  
* Pages load only when accessed (lazy loading)  
* Clean flush on `.quit`

### **Acceptance Criteria**

$ ./chronicle ironhold.world  
ironhold\> SPAWN PLAYER 1 alice alice@ironhold.gg  
Spawned.  
ironhold\> .quit

$ ./chronicle ironhold.world  
ironhold\> LIST PLAYERS  
\[1\] alice \<alice@ironhold.gg\>  
1 entities.  
ironhold\> .status  
World: online — ironhold.world (1 entities, 1 pages)  
ironhold\> .quit

