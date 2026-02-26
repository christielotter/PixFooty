# ⚽ PixFooty

A real-time 2D pixel-art football game built in C++, featuring physics-based ball mechanics, player control, and AI opposition.

---

## 📌 Overview

PixFooty is a single-player 2D football simulation developed to explore real-time systems, object-oriented design, and physics modelling in C++.  

The project focuses on clean architecture, structured debugging, and implementing responsive gameplay mechanics from scratch.

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

### Embedded Integration (if applicable)
- Hardware-based user input implementation
- Consideration of memory and performance constraints

---

## 🛠️ Technologies Used

- C++
- Object-Oriented Programming
- Git Version Control
- Real-time simulation principles

---

## 🏗️ Core Game Loop

```cpp
while (gameRunning) {
    processInput();
    updatePhysics(deltaTime);
    handleCollisions();
    renderFrame();
}
