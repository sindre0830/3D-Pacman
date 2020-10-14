# Setup
If this is a fresh install you can skip these steps and build the program.
If you already had this downloaded and you just updated the project (git pull) you will need to follow these steps:

1. Close Visual Studio
2. Delete .vs and out folder
3. Open Visual Studio and let it set everything up

The reason for this is that the images that we modified during development won't get recognized by CMake and thus not seen in the application

# Class structure

```
GeometricShape
|
+--Wall
|
+--Pellet
|
+--Number
|
+--Gameover
|
+--Character
   |
   +--Pacman
   |
   +--Ghost
```

- **GeometricShape** is a root class and only has the tools for a simple geometric shape.
   - **Wall** is a leaf class and has all the tools to draw static walls and corners to the screen.
   - **Pellet** is a leaf class and has all the tools to draw static pellets and magical pellets to the screen.
   - **Number** is a leaf class and has all the tools to draw one dynamic digit to the screen and update it according to score.
   - **Gameover** is a leaf class and has all the tools to draw static "GAME OVER" to the screen.
   - **Character** is an internal class and has all the tools for texture and position translation.
      - **Pacman** is a leaf class and has all the tools to draw a dynamic Pacman to the screen
      - **Ghost** is a leaf class and has all the tools to draw a dynamic ghost to the screen
