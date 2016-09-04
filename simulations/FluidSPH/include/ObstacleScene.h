#ifndef OBSTACLESCENE
#define OBSTACLESCENE

#include "Primitives.h"
#include <vector>

class ObstacleScene
{
public:
  ObstacleScene();
  ~ObstacleScene();

  void addCuboidsToList(std::vector<Primitives::Cuboid> &_list);
  void rotateCuboid(Primitives::Cuboid &_c, ngl::Vec3);

};

#endif // OBSTACLESCENE

