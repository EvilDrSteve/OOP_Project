# Architecture

This document describes the high-level structure of the Diner Dash clone built for
the OOP course. It is intended as a reader's map — if you want to know *where* a
behaviour lives before diving into the code, start here.

## Overview

The game is a 2D top-down restaurant sim written in C++17 against SFML. The player
acts as a waiter: drag customers from the entrance queue onto empty tables, take
their orders to the kitchen, deliver the cooked food back, collect payment, and
clean up dirty dishes. Rounds are time-limited; the score is a mix of tables
served minus tables lost.

## Module Map

Both `include/` and `src/` are split into six parallel subfolders. Anything in
`include/<folder>/` has a matching `src/<folder>/` source file (except enums and
the header-only `Inventory`).

| Folder      | Contents                                                                 | Role                                                                                     |
| ----------- | ------------------------------------------------------------------------ | ---------------------------------------------------------------------------------------- |
| `core/`     | `Game`, `State`                                                          | Application bootstrap and the abstract base for every screen.                            |
| `states/`   | `MainMenuState`, `GameState`, `EndState`                                 | Concrete screens pushed onto `Game`'s state stack.                                       |
| `entities/` | `Character`, `Player`, `Chef`, `Customer`, `CustomerCharacter`, `CustomerState` | Anything that is animated and/or has behaviour that changes per frame.           |
| `world/`    | `Grid`, `Table`, `Kitchen`, `Sink`                                       | The playfield and the fixed props the player interacts with.                             |
| `systems/`  | `CustomerQueue`, `Order`, `Inventory`                                    | Non-rendered (or mostly non-rendered) support systems that track game state.             |
| `ui/`       | `Button`, `EndScreen`                                                    | Reusable UI widgets and the post-round summary panel.                                    |

The Makefile compiles every `.cpp` under `src/` recursively and adds each
subfolder to the `-I` include path, so existing `#include "Foo.hpp"` lines
continue to work without the subfolder prefix.

## Class Hierarchy

```
State                         Character
  |                              |
  +-- MainMenuState              +-- Player
  +-- GameState                  +-- Chef
  +-- EndState                   +-- CustomerCharacter

Customer  (owns a std::vector<CustomerCharacter*>)
Grid      (owns Player, Tables, Kitchen, Sink, CustomerQueue, Chefs)
```

`State` and `Character` are the only abstract bases. `Customer` is a plain class
that composes one or more `CustomerCharacter` sprites — a diner group with more
than one seat is rendered as multiple characters around a single `Customer`
object that owns the shared patience/order state.

## Main Loop

```
main.cpp
  Game game;
  game.run();
      while running:
          updateClock()           // dt from sf::Clock
          update()                // states.top()->update(dt) + updateInputs(dt)
          render() / lateRender() // draw world, then overlays
```

`Game` keeps a `std::stack<State*>`. `States` can push a sibling (`MainMenuState`
pushes `GameState` on Play; `GameState` pushes `EndState` when the round ends)
and pop themselves from `endState()`.

## Customer State Machine

Defined in [include/entities/CustomerState.hpp](include/entities/CustomerState.hpp).

```
IN_QUEUE  ->  BEING_SEATED  ->  SEATED  ->  READY_TO_ORDER
                                                |
                                                v
                                          ORDER_TAKEN
                                                |
                                                v
                                          FOOD_DELIVERED (eating)
                                                |
                                                v
                                          READY_TO_PAY  ->  PAID  ->  LEFT
```

Each transition is driven either by a timer inside `Customer::update` (e.g.
`SEATED` -> `READY_TO_ORDER` after `menuReadTime`) or by a Player interaction
(`takeOrder`, `receiveFood`, `payBill`, `clearDishes`). A separate
`CustomerIndicator` enum drives the speech-bubble sprite above the customer's
head. If `patienceTimer` runs out before the player reaches them, the customer
becomes `ANGRY` and is counted as a lost table.

## Order Lifecycle

Defined in [include/systems/Order.hpp](include/systems/Order.hpp).

```
PENDING    // Player carries the written order
   |
   v
COOKING    // Dropped at Kitchen; Kitchen::update advances cookProgress
   |
   v
READY      // Ready to pick up from Kitchen
   |
   v
DELIVERED  // Placed on the customer's Table
```

`Order` holds back-pointers to its `Table` and `Customer` so the `Grid` can
resolve "deliver this food" into a concrete destination.

## Key Collaborators (the Grid)

[include/world/Grid.hpp](include/world/Grid.hpp) is the central hub. It owns
the tile map, the `Player`, every `Table`, the `CustomerQueue`, the `Kitchen`
and the `Sink`, and it performs A*-style pathfinding on its own `Node` grid.

The typical flow for a single player action:

1. Mouse press -> `Grid::handleMousePressed` picks an entity (table / kitchen
   / sink / queued customer).
2. `Grid::movePlayerToTable` (or `Kitchen` / `Sink`) runs `findPath` and calls
   `Player::setDestination(path, PlayerAction::...)`.
3. `Player::update` walks the path; when it arrives, `onReachDestination` fires
   and `Grid` calls the matching `Player::interactWithTable/Kitchen/Sink`.
4. That interaction mutates the `Customer`, `Kitchen` or `Sink` and typically
   updates the `Player`'s `Inventory`.

The `Grid` is intentionally the highest-traffic class in the project; it
aggregates references to nearly every other module.

## Asset Layout

`assets/` contains the SFML-loadable content: character sprite sheets
(`SERENE_VILLAGE_REVAMPED`), interior tilesets (`Interiors 5_...`,
`Modern tiles_Free`), individual prop art (`Kitchen_02.png`, `sink.png`,
`Table-*.png`), UI art (`Assets_UI.png`, `play_button.png`,
`replay_button.png`, `mainmenu_button.png`), the `fonts/` folder and the
background-music track `bgMusic.mp3`. Paths are currently referenced as
string literals from inside the classes that load them.

## Known Limitations

- **`Grid` is a god class** (~700 lines). Pathfinding, input handling,
  rendering of the floor, money/score bookkeeping and player routing all live
  together. A natural refactor would split it into `Pathfinder`,
  `InputRouter` and `ScoreKeeper` collaborators. Out of scope for this pass.
- **Raw `new`/`delete`** throughout the entity and state code. Moving to
  `std::unique_ptr` would clarify ownership but again is a future task.
- **Hard-coded asset paths** inside constructors — a config file or central
  `AssetLibrary` would make asset swaps easier.

## Building and Running

```
make           # builds build/game
./build/game   # launches the menu
make clean     # removes obj/ and build/
```

Requires a working `g++` with C++17 and the SFML development packages
(`sfml-graphics`, `sfml-window`, `sfml-system`, `sfml-audio`).
