#Arcade Physics
An arcade physics library

##Why Arcade Physics?
If you want an accurate physics simulation, Box2D and Chipmunk already exist. This project uses pixels as a distance unit and milliseconds as a time unit, making integration with pixel-perfect 2D games relatively painless.

##State of the Project
The project is currently in its infancy.

##Roadmap to 1.0
I want to release a 1.0 version as soon as possible to begin providing a stable API, but a few things need to happen first. First, I want to complete the items on this ToDo list:

Version 0.1: 

- [x] Basic vector and line math
- [x] Implementation of basic shapes such as AABB, circles, and polygons
- [ ] A universal shape handle
- [x] Rotated polygons
- [x] Simple rectangle polygon constructors

Version 0.2:

- [ ] An object structure that stores a collision shape and arbitrary data
- [ ] A tilemap to store static objects
- [ ] A quadtree to store dynamic objects

Version 0.3:

- [ ] A world to integrate quadtree(s) and tilemap(s)
- [ ] Object groups that allow simple definitions of interaction behavior

Version 1.0:

- [ ] Test coverage on large majority of code paths
- [ ] In-depth documentation

Second, I want to use the project in production to see the sticking points in the API and smoothe it over before freezing it.

##Branch Structure
Each new feature or modification should be in its own branch. Non-master branches are free to fail tests or fail to compile entirely. The master branch must pass all tests and must compile with no warnings or error in the latest version of Rust. Tags are to be used for releases only, and as such should be treated with the same level of care as the master branch.
