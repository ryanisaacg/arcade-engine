#Arcade Engine
A 2D game engine written in pure C for writing arcade-style games. Arcade-style games feature things like pixel-based physics rather than a unit-based physics engine, pixel-perfect cameras, and pixel-based scaling techniques. 

##Current Featurelist
- Simulation
	- Basic vector and line math for 2D
	- Basic matrix manipulation for a 3x3 matrix
	- Basic shapes such as Axis-Aligned Bounding Boxes (Rects), circles, and polygons
	- A single interface to the three shape types (Shape)
	- Collision and intersection
	- A QuadTree to store dynamic physical entities
	- A TileMap to store static physical entities
	- A World to combine the QuadTree and TileMap and make geometric queries to both
	- Object groups that govern which objects interact
- Graphics
	- A windowing module backed by GLFW
	- OpenGL texture loading backed by SOIL
	- Batched OpenGL drawing using the shader pipeline
	- Support for custom shaders and custom batches
	- Support for drawing only part of a single texture image
	- Support for basic animations
	- Make `ArcadeObject`s drawable

##Roadmap
- [ ] Draw the spatial maps
- [ ] Integrate Window and World into a single Game struct that makes creation easier
- [ ] Add functions to load "Tiled" files into the TileMap
- [ ] Add the notion of a discrete level
- [ ] Add level restart and switch functions
- [ ] Add the ability to restart, save, load, or close the game
- [ ] Add window scaling
- [ ] Define or research a texture atlas format
	- [ ] Create a texture atlas packaging tool
	- [ ] Implement loading this texture atlas
- [ ] Previous-frame input checking
- [ ] Doxygen or equivalent documentation
- [ ] Test coverage of existing modules
- [ ] Simple HTML5 game build support

##Why C?
Adequate solutions to the problems solved by this library exist in other language or are trivial to implement, and I've found most other languages unsuitable for game development. Stack allocation is a must, which rules out many higher-level languages. Additionally, distribution without requiring the end user install a Virtual Machine or Runtime.
