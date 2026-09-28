# R-Type project specification

This document organizes the R-Type v4.2-dev assignment into requirements and
delivery goals. The project is a networked horizontal shoot-'em-up built on a
game engine designed by the team.

## Deliverables and schedule

| Delivery | Deadline from project start | Expected result |
| --- | --- | --- |
| Part 1 | 4 weeks | Playable networked prototype and core engine architecture |
| Part 2 | 7 weeks total | A more mature game with significant work in one or more advanced tracks |

The deliverables include two C++ programs, `r-type_server` and `r-type_client`.
Both deliveries are evaluated in a defense. Software engineering,
documentation, and accessibility are continuous requirements evaluated at
both defenses.

**Requirement language:** **MUST** is mandatory, **SHOULD** is expected where
feasible, and **MAY** is optional. Part 2 topics are choices unless a selected
topic states a specific requirement.

## Common requirements

### Build, platforms, and workflow

- **MUST** use CMake and a package manager for third-party dependencies:
  Conan, vcpkg, or CMake CPM.
- **MUST** build and run without installing project dependencies or headers
  system-wide. Standard C++ compilers and SDKs, plus low-level platform
  libraries such as OpenGL or X11, are acceptable exceptions. Do not vendor
  complete dependency source trees as a substitute for package management.
- **MUST** run both client and server on Linux.
- **SHOULD** support Windows with Microsoft Visual C++ as well, including
  cross-play between Windows and Linux clients. macOS and WSL do not satisfy
  this cross-platform goal.
- **MUST** use a defined Git workflow: feature branches, issues, merge
  requests, meaningful commits, and tags for milestones.
- CI for building, testing, linting, or deployment is beneficial. Cache
  dependencies or use prepared images so CI does not download everything on
  each commit. Avoid dedicating a full-time team member to CI.
- Use formatting and static analysis tools such as `clang-format` and
  `clang-tidy`. The repository conventions are in
  [docs/coding-style.md](docs/coding-style.md) and
  [docs/contributing.md](docs/contributing.md).

### Documentation

All documentation **MUST** be in English and accessible through structured,
linked, navigable online pages rather than only PDF or Word files. Provide an
outline, useful search, and search-engine indexing where practical. Include:

- A concise, practical README covering the purpose, supported platforms,
  requirements, build and run instructions, license, authors or contacts, and
  useful links.
- Developer documentation explaining the architecture, subsystem boundaries,
  code layout, and common development tasks. Include diagrams, overviews,
  tutorials, contribution guidelines, and coding conventions where useful.
  Generated API documentation alone is insufficient.
- A technical and comparative study explaining technology choices, including
  algorithms and data structures, storage and persistence, security and data
  integrity, and how security will be monitored over time.
- A protocol specification detailed enough that another developer **SHOULD**
  be able to implement a compatible client from the documentation alone.
  Describe packets, commands, encodings, and error handling.

### Accessibility

The game and its documentation **MUST** consider physical and motor, audio and
visual, and mental and cognitive disabilities. Document the solution provided
for each category; the brief leaves the specific features to the team.

## Part 1: playable prototype

### Architecture and game engine

- **MUST** separate rendering, networking, and game logic into visible,
  decoupled subsystems or layers.
- Define reusable abstractions for entities, coordinates, and communication
  between systems. Rendering should depend only on the data it needs, such as
  appearance and position.
- An entity-component-system design or mediator pattern is recommended, but
  neither is required.
- **MUST** have a graphical client and a central server with final authority
  over game state and rules. Each player uses a separate client on the network.

### Server

- **MUST** implement gameplay logic and send relevant events and updates to
  clients: spawns, movement, destruction, firing, player deaths, and other
  players' actions.
- **MUST** be multithreaded and continue advancing the game independently of
  whether clients send messages.
- **MUST** survive a client crash and notify the other clients in that game.
  More generally, bad client behavior must not stop the server.
- **MAY** use Asio or operating-system sockets. If using raw sockets, wrap
  them in suitable abstractions.

### Client

- **MUST** render the game and handle player input while the server retains
  authority over consequential gameplay events.
- **MAY** predict local movement or projectiles, but server decisions must
  resolve the final state, especially deaths, pickups, and damage.
- **MAY** use SFML, SDL, Raylib, or a comparable focused library for graphics,
  audio, and input. Existing game engines and libraries that provide too much
  of the game architecture are forbidden.

### Network protocol

- **MUST** use a documented binary protocol.
- **MUST** use UDP for all in-game communication, including entities, movement,
  and events. A separate TCP channel is tolerable only for a specific,
  well-justified purpose.
- **MUST** validate packet sizes, types, ranges, and allocation limits.
  Malformed or hostile packets must not crash either program, exhaust memory,
  or compromise the server.

### Gameplay and presentation

- **MUST** deliver a playable game with player ships, waves of Bydo enemies,
  missiles, and a scrolling space starfield.
- **MUST** make scrolling, entity behavior, and game time depend on timers
  rather than CPU speed.
- **MUST** support movement with the arrow keys.
- **MUST** spawn monsters randomly on the right side of the screen.
- **MUST** make the four players in a game visually distinguishable.
- Consider basic sound design. The brief identifies player and enemy missiles,
  obstacles, and destructible tiles as elements of the R-Type presentation.

## Part 2: advanced topics

Part 2 calls for significant work in one or more of the following tracks. It
does not require every topic below. Select a scope the team can implement,
measure, document, and demonstrate.

### Track 1: reusable game engine

- Separate the engine from R-Type game rules and assets so R-Type consumes it
  as a library. A second, different sample game is the strongest proof of
  reuse; it can be a small demonstration rather than a full game.
- Consider modularity at compile time, link time, or through runtime plugins.
- Possible engine subsystems include rendering, physics and collisions, audio,
  input devices, event or message passing, asset loading and caching, and
  scripting.
- Possible developer tools include a console and configurable variables,
  metrics and profiling, and scene or asset editors.

### Track 2: advanced networking

- Multi-instance server features may include parallel games, lobbies or
  rooms, identity and sessions, chat, configurable rules, scoreboards, and
  administration tools.
- Improve transmission efficiency with compact data types, serialization,
  quantization or bit packing, general-purpose compression, or delta
  snapshots.
- Handle UDP packet loss, reordering, and duplication. If this topic is
  selected, critical messages such as joining a game or player death **MUST**
  be delivered reliably, for example through acknowledgements and retries.
- Improve real-time consistency and responsiveness with techniques such as
  interpolation, client prediction and server reconciliation, lag
  compensation, input delay, or rollback. Demonstrate actual measurements of
  bandwidth use and behavior under latency.

### Track 3: gameplay and content creation

- Expand the game with varied enemies, stages, bosses, weapons such as the
  Force, configurable rules or modes, and sound design. Evaluate both the
  quality of the result and the ease of adding more content.
- **SHOULD** provide clear APIs or data-driven tools for adding content and
  behaviors without extensive engine changes. Options include plugins,
  scripting, standard asset formats, or dedicated editors.
- Document content creation with tutorials and how-to guides so designers can
  use the tools without deep knowledge of engine internals.
