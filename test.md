chronicle/
├── CMakeLists.txt                 # Modern C++ build configuration (or Makefile)
├── Makefile                       # Provided build script
├── README.md                      # Project documentation and setup instructions
│
├── include/                       # Public Header files (.hpp / .h)
│   ├── application_state.hpp      # AppState enum / transition definitions
│   ├── console.hpp                # Console loop & command processing loop
│   ├── input_buffer.hpp           # InputBuffer class definition[cite: 1]
│   ├── command_handler.hpp        # Command dispatches (dot-commands vs world commands)
│   ├── world_engine.hpp           # Core Engine manager / World state tracker
│   ├── entity.hpp                 # Player, NPC, Item, and Quest entity definitions
│   ├── persistence_manager.hpp    # File I/O, state saving, crash recovery logic
│   └── spatial_index.hpp          # Spatial hashing / fast query data structures
│
├── src/                           # Source implementation files (.cpp)[cite: 1]
│   ├── main.cpp                   # Application entry point[cite: 1]
│   ├── console.cpp                # Interactive terminal loop (`ironhold>`) implementation[cite: 1]
│   ├── input_buffer.cpp           # InputBuffer trimming and history tracking logic[cite: 1]
│   ├── command_handler.cpp        # Processing `.quit`, `.help`, `.version`, `.status`, etc.[cite: 1]
│   ├── world_engine.cpp           # Game world state management
│   ├── entity.cpp                 # Entity behaviors and logic
│   └── persistence_manager.cpp    # Binary/chunked persistence & logging mechanisms
│
├── tests/                         # Test suites for each milestone[cite: 1]
│   ├── test_milestone1.cpp        # Console, dot-commands, and buffer tests
│   ├── test_milestone2.cpp        # Entity state and tracking tests
│   ├── test_persistence.cpp       # Crash recovery & persistence tests
│   └── test_performance.cpp       # Microsecond benchmark & concurrency tests
│
└── data/                          # Runtime persistence storage (created on launch/save)
    └── world_state.bin            # Persistent state storage file