#pragma once

#define DECL(name) struct name; typedef struct name name

DECL(Animation);
DECL(ArcadeObject);
DECL(AssetManager);
DECL(Camera);
DECL(Circle);
DECL(Group);
DECL(Level);
DECL(Line);
DECL(Matrix3);
DECL(Polygon);
DECL(QuadTree);
DECL(Rect);
DECL(Shape);
DECL(SpatialMap);
DECL(Sprite);
DECL(Texture);
DECL(TextureRegion);
DECL(Transform);
DECL(Window);
DECL(World);
DECL(Vector2);

typedef void (*WorldUpdate)(World, ArcadeObject*, void*);
