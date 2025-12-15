# Donkey Kong — C++ Console Game

A C++ Windows Console implementation of the classic arcade game "Donkey Kong."
Guide Mario through various levels, jump over barrels, avoid ghosts, and climb ladders to rescue Princess Pauline.

---

## Features
- **Dynamic Level Loading:** Automatically detects and loads level layouts from `.screen` text files.
- **Physics-Based Movement:** Implements jumping arcs, gravity, and ladder climbing mechanics.
- **Entity AI:**
  - Barrels roll, fall off ledges, and explode upon impact or timeout.
  - Ghosts wander platforms and randomly change directions.
- **Combat System:** Mario can pick up a hammer (`p`) to destroy barrels and ghosts for points.
- **Game Loop:** Includes a main menu, pause functionality, win/loss states, and score tracking.
- **Rendering:** Custom `gotoxy` implementation for flicker-free console rendering.

---

## Project structure
Donkey_Kong/
├─ main.cpp // Application entry point
├─ TheGame.cpp/h // Main game manager: loop, collision checks, entity management
├─ Board.cpp/h // Map management: loads levels, handles rendering, tracks positions
├─ Mario.cpp/h // Player logic: state machine for jumping, moving, and dying
├─ Barrel.cpp/h // Enemy logic: rolling algorithms, falling physics, and explosions
├─ Ghost.cpp/h // Enemy logic: wandering AI behavior
├─ Point.cpp/h // Helper class for coordinate management (x, y) and drawing
├─ Menu.cpp/h // UI: Main menu, instructions, pause screen, and win/lose messages
├─ Utils.cpp/h // System helpers: `gotoxy`, `hideCursor`, console settings
├─ dkong_*.screen // Level layout files (ASCII art maps)

---

## Gameplay
1. **Objective:** Guide Mario (`@`) to reach Pauline (`$`) at the top of the map.
2. **Movement:** Use keys to move left, right, climb ladders, or jump over obstacles.
3. **Hazards:**
   - **Barrels (`O`):** Thrown by Donkey Kong; they roll across floors and fall down ladders.
   - **Ghosts (`x`):** Wander the platforms; contact results in a lost life.
   - **Falls:** Falling from a height greater than 5 blocks is fatal.
4. **Power-ups:** Locate the Hammer (`p`) to temporarily gain the ability to smash enemies.
5. **Controls:**
   - **w / W:** Jump or Climb Up
   - **x / X:** Climb Down
   - **a / A:** Move Left
   - **d / D:** Move Right
   - **s / S:** Stay in place
   - **p / P:** Pick up / Use Hammer
   - **ESC:** Pause Game
   - **9:** Exit

---

## Implementation notes

### `TheGame`
- Acts as the central controller. It initializes the `Board` and `Mario`, manages the `std::vector` lists for `Barrels` and `Ghosts`, and runs the main game loop (`run()`).
- Handles high-level logic like checking collisions (`checkCollisions()`) and transitioning between levels (`handleWinLevel()`).

### `Board`
- Responsible for parsing `.screen` files using `std::filesystem`.
- Converts ASCII characters into game objects (detects positions of Mario, Donkey Kong, etc.) and stores the static map layout.

### `Mario`
- Encapsulates player state, including `isJumping`, `jumpPhase`, and `life`.
- `handleUpAndJump(...)` calculates valid jump trajectories to ensure Mario doesn't clip through floors.

### `Barrel` & `Ghost`
- **Barrels:** Implement specific movement logic to follow floors (`=`, `>`, `<`) and fall when reaching edges or ladders. They have an "explode" state triggered after falling a certain distance or hitting walls.
- **Ghosts:** Use a random number generator (`std::mt19937`) to decide when to change direction, giving them unpredictable movement.

### `Point`
- A utility class used by all entities to manage specific `(x, y)` coordinates, draw their symbol to the console, and erase their previous position to prevent "trails."

---

## Screenshots
<img width="1001" height="651" alt="Screenshot 2025-12-15 104349" src="https://github.com/user-attachments/assets/95afa852-5192-4948-8b20-3b10938bbab7" />
<img width="1002" height="630" alt="Screenshot 2025-12-15 104405" src="https://github.com/user-attachments/assets/efdefb5c-3767-4265-9543-2f660a0bcd95" />
<img width="998" height="640" alt="Screenshot 2025-12-15 104419" src="https://github.com/user-attachments/assets/499608ed-a37a-4401-8679-afd357813ff5" />
<img width="417" height="597" alt="Screenshot 2025-12-15 104628" src="https://github.com/user-attachments/assets/ebd309aa-1fec-4962-b78d-f62f48a51104" />
<img width="414" height="571" alt="Screenshot 2025-12-15 104502" src="https://github.com/user-attachments/assets/197a96a7-6e31-4310-9236-a739861da8ab" />

## Future improvements
- Implement a high-score table saved to a file.
- Add color output for different entities (e.g., Red Mario, Brown Barrels).
- Create a level editor to allow users to design their own `.screen` maps easily.
