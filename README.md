# Guide

### Setup
If this is a fresh install you can skip these steps and build the program.
If you already had this downloaded and you just updated the project (git pull) you will need to follow these steps:

1. Close Visual Studio
2. Delete .vs and out folder
3. Open Visual Studio and let it set everything up

The reason for this is that the images that we modified during development won't get recognized by CMake and thus not seen in the application

### How to use the application

When you open the game you will be greeted by a menu screen where you can choose between level 0 and 1. You will have to press a number on the keyboard to select a level and continue. 
Once you have selected a level the game will start. Here you move around with either the arrow keys or WASD. 
By pressing 1, 2, or 3 you can change the perspective. 1 = first person, 2 = 2D (i.e assignment 1) and 3 = third person.

### Known issues

Check the issue board for non-closed issues.

# Assignment details

### Introduction

In this assignment, each group is going to program a 3D version of the Pacman
game developed in Assignment 1. Similarly to Assignment 1, the software need to
be developed using C++ and OpenGL. In addition to the key concepts used in
Assignment 1, the core concepts needed to achieve this are:

- 3D Object Loading.
- 3D Geometry.
- 3D Transformations.
- Lighting.

As in Assignment 1, the game should follow the conventional gameplay principles
of character navigation in a maze, collection of pellets along the way and
avoiding ghosts.

### Evaluation

The *deadline* for the assignment is *November 17th 11:59 pm* (strict).

This assignment is worth *30%* (all the requirements) of the total grade for the
course with up to 5% extra points (optional features).

To be considered for its evaluation, the program must configure (if relevant),
build and run properly in machines with:

- Recent compiler (Visual studio 2019 or GCC > 9.0).
- Recent OpenGL (>= 4.3).
- If relevant, recent CMake (>= 3.12)
- Recent GLFW (>= 3.3.2)
- Recent GLEW (>= 2.1.0)
- C++ >= 11.

In addition to functionality, we will put considerable weight on the professionalism of your solution.

- Modularity: Code should be organized by functionality and reusability, and make use of header files to specify the externally available functionality. Be clear about your decisions, especially whenever your code design deviates from the above-mentioned principles (e.g., for performance/memory reasons, etc.).
- Versioning: Use proper version practice. Commit early and often. You will work in teams, which makes frequent commits important to avoid merge conflicts.
- Code as documentation: Make your code readable and add comments to clarify its function and purpose.
This assignment is worth **30% of your grade** (with up to 5% additional bonus points).

### Starting the Assignment

To start the development, *fork* (not clone) this repository into an own GitLab area and work on that. Please ensure that all members of your group and the teaching team has access to the forked repository. This way, we can easily distribute additional code or modifications if necessary.

Before submission, please ensure:

 - your repository URL is correct (and visible),
 - your group allocations are up to date at the time of submission
 - your code compiles and runs. We will not mark code that does not compile! You
   should provide instructions on how to run in the README file that is part of
   the forked repository.


Your program has to provide the following features:

- Read the provided map layout and render it on the screen. Walls should have textures. Feel free to use any texture of your choice.
- Have a Pacman player character that is controlled using the conventional *WASD* keyboard mapping. Pacman should not be able to run through walls.
- Include collectible pellets. Once collected, the items should disappear from the map.
- Have moving ghosts. A collision between Pacman and the Ghosts terminates the game.
- At least one character type (Pacman, ghosts, collectibles) needs to be loaded from a model (e.g., .obj file).
- Basic lighting model.
- You can choose to implement the game from a first-person perspective. In this case you will need to display a separate 2D projection of the grid for navigation purposes. Alternatively, you can implement a third-person perspective, in which case you do not require the 2D grid representation.

### Optional Features

To increase your marks you can add some additional features:

- Advanced lighting/reflection models
- Switching between perspectives (e.g., first-person, third-person, 2D).
- Use of 3D models for multiple character types/walls, or use of advanced models.
- Write your own object loader.
- Make the camera movable.
- Introduce multiple levels.
- Any other feature that you think adds an edge to your game (e.g., increases the fun factor, or usability, or aesthetics (see Marking Consideration section below).

Examples on Pacman 3D:

- [[https://www.youtube.com/watch?v=vIrFJK6NhCE][https://www.youtube.com/watch?v=vIrFJK6NhCE]]
- [[https://www.youtube.com/watch?v=IujqngIEdb0]]


### Marking Consideration

Since the assignment leaves you a great amount of freedom of concentration on tasks and features that are of interest to you, we will give you the choice to put more weight on either of the three categories *Architecture/Framework*, *Gameplay/Usability*, *Aesthetics/Quality of Graphics*. Consider this when planning your project.
