# ⚽ PixFooty

A real-time 2D pixel-art football game built in C++, featuring physics-based ball mechanics, player control, and AI opposition.

Video Demonstration:



https://github.com/user-attachments/assets/9738317f-aef8-455d-9b4a-2f5ba7e132f6


---

## 📌 Overview & Motivations

PixFooty is a single-player 2D football simulation developed to explore real-time systems, object-oriented design, and physics modelling in C++.  
The project focuses on clean architecture, structured debugging, and implementing responsive gameplay mechanics from scratch.

As a sports person and passionate football fan, majority of my gaming experience has been sports based games such as FIFA and Football Manager, so I’m sticking with a sports related game.
The opportunity to combine my academic strengths and interests of software development with my extra-cirricular interest in football is an element that will make this project a great experience.
The game that has inspired my idea, is a mobile game called Head Soccer. The user selects players of different nationalities against AI players of increasing difficulty.

---

## 🎮 Features

- ⚽ Real-time 2D ball physics (velocity, acceleration, collision response)
- 🧍 Player movement with directional control and momentum
- 🤖 CPU-controlled opponent with basic decision logic
- 🥅 Goal detection and scoring system
- 🎨 Pixel-art rendering system
- 🧠 Deterministic update loop
- 🛠️ Modular and extensible architecture

---

## 🧠 Technical Highlights

### Object-Oriented Design
- Encapsulated game entities (`Player`, `Ball`, `GameState`, `InputHandler`)
- Separation of logic, physics, and rendering
- Reusable and maintainable class structure

### Physics & Simulation
- Discrete-time update loop
- Position updates using velocity integration
- Collision detection and response modelling
- Boundary and constraint handling

### Debugging & Development
- Structured debugging approach
- Iterative testing and refinement
- Emphasis on clean, readable C++ implementation

### Embedded Integration
- Hardware-based user input implementation with buttons and an LCD screen
- Consideration of memory and performance constraints

---

## 🛠️ Technologies Used

- C++
- Object-Oriented Programming
- Real-time simulation principles

---

## Improvements for next time

- LCD with higher resolution for improved fps, collision mechanics and characters
- Varying difficulty modes
- Character choice

---

## 🏗️ Core Game Loop

```cpp
while (gameRunning) {
    processInput();
    updatePhysics(deltaTime);
    handleCollisions();
    renderFrame();
}



