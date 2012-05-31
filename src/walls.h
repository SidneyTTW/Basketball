#ifndef WALLS_H
#define WALLS_H

class Flat;
class MyWorld;

/**
 * Class of walls.
 */
class Walls
{
public:
  Walls(MyWorld *world);

  void render();

  ~Walls();

  /**
   * The walls.
   */
  Flat *walls[4];

private:

  /**
   * Ids of the textures.
   */
  static int *textures;

  static float materialAmbient[4];
  static float materialDiffuse[4];
  static float materialSpecular[4];
};

#endif // WALLS_H
