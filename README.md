# README

A 2D game engine written in pure C for writing arcade-style games. Arcade-style games feature things like pixel-based physics rather than a unit-based physics engine, pixel-perfect cameras, and pixel-based scaling techniques. 

## Current Featurelist

- Simulation
	- Basic vector and line math for 2D
	- Basic matrix manipulation for a 3x3 matrix
	- Basic shapes such as Axis-Aligned Bounding Boxes (Rects), circles, and polygons
	- Changes to Polygons are cached to speed up usage
	- A single interface to the three shape types (Shape)
	- Collision and intersection
	- A QuadTree to store dynamic physical entities
	- A TileMap to store static physical entities
	- A World to combine the QuadTree and TileMap and make geometric queries to both
	- Object groups that govern which objects interact
	- A Level object which manages loading and storing a game state
	- A Game to combine Levels, a World, and a Window
- Graphics
	- Use SDL2's Renderer API for hardware-accelerated graphics
	- Support for drawing only part of a single texture image
	- Support for basic animations
	- Automatically draw objects in the World struct
	- Automatically draw tilemaps in the World struct
	- A Camera that can be configured to letterbox or stretch
- Input
	- Keyboard and mouse support
	- Previous-frame keyboard and mouse state polling

##Roadmap
- [ ] Variable draw depths
- [ ] Add functions to load "Tiled" files into the TileMap
- [ ] Define or research a texture atlas format
	- [ ] Implement render-to-texture features
	- [ ] Implement screenshot saving
	- [ ] Create a texture atlas packaging tool
	- [ ] Implement loading this texture atlas
- [ ] Doxygen or equivalent documentation
- [ ] Test coverage of existing modules
- [ ] Simple HTML5 game build support

##Why C?
Adequate solutions to the problems solved by this library exist in other language or are trivial to implement, and I've found most other languages unsuitable for game development. Stack allocation is a must, which rules out many higher-level languages. Additionally, distribution without requiring the end user install a Virtual Machine or Runtime.
