# Graphic Tests in C++

## Project Overview

This project aims to conduct various tests for graphics using C++. The primary focus is on creating visual effects and interactive graphical elements using the SFML (Simple and Fast Multimedia Library). The project will contain multiple test files, each demonstrating different aspects of graphics programming.

## File Description

### `main.cpp`

This file contains the initial implementation of a simple graphics program using SFML. The program creates a window displaying a grid of circles that react dynamically to mouse movements.

#### Key Features:

- **Window Creation:** Sets up a render window with a fixed frame rate limit.
- **Mouse Interaction:** Detects and handles mouse drag events to update the position of circles.
- **Dynamic Circle Rendering:** Calculates the position and size of circles based on their distance from the center and applies a logarithmic movement towards the center.
- **Visual Effects:** Adjusts the transparency of circles based on their radius using a mathematical function.

#### Code Highlights:

- **Libraries Used:** 
  ```cpp
  #include <SFML/Graphics.hpp>
  #include <iostream>
  ```
- **Constants Defined:**
  ```cpp
  #define ROW 10
  #define COL 10
  #define MAX_SPHERE_D 360.0f
  ```
- **Main Function:**
  The `main()` function sets up the window and enters the main event loop, handling events such as closing the window, mouse button presses, and mouse movements. It calculates new positions for the circles and draws them accordingly.

## Setup and Compilation

1. **Install SFML:**
   Ensure SFML is installed on your system. You can download it from [SFML's official website](https://www.sfml-dev.org/download.php).

2. **Compile the Program:**
   Use the following command to compile `main.cpp`:
   ```sh
   g++ -o drawer main.cpp -lsfml-graphics -lsfml-window -lsfml-system
   ```

3. **Run the Program:**
   Execute the compiled program:
   ```sh
   ./drawer
   ```

## Future Work

This project will be expanded to include additional test files that demonstrate various other graphics programming techniques and effects using SFML and possibly other libraries. Stay tuned for updates!
