#include "ObstacleScene.h"
#include "ngl/Transformation.h"

ObstacleScene::ObstacleScene()
{

}

ObstacleScene::~ObstacleScene()
{

}

void ObstacleScene::addCuboidsToList(std::vector<Primitives::Cuboid> &_list)
{
  ngl::Vec3 leftCorner = ngl::Vec3(-9,-10,-3.5);
  float size1 = 6;
  /// Tall tower base
  Primitives::Cuboid c1(ngl::Vec3(leftCorner.m_x + size1/2.0f,0.0f,leftCorner.m_z + size1/2.0f),size1,5,size1);
  /// Battlements
  //Front
  float bSize1 = size1 *0.1f;
  float yPos1 = 3.0f;
  Primitives::Cuboid c2(ngl::Vec3(leftCorner.m_x + size1/2.0f,yPos1,leftCorner.m_z + size1 - bSize1/2.0f),size1,1.3f,bSize1);
  //Back
  Primitives::Cuboid c3(ngl::Vec3(leftCorner.m_x + size1/2.0f,yPos1,leftCorner.m_z + bSize1/2.0f),size1,1.3f,bSize1);
  //Left
  Primitives::Cuboid c4(ngl::Vec3(leftCorner.m_x + bSize1/2.0f,yPos1,leftCorner.m_z + size1/2.0f),bSize1,1.3f,size1);
  //Right Front
  float bsize2 = size1 * 0.20f;
  Primitives::Cuboid c5(ngl::Vec3(leftCorner.m_x + size1 - bSize1/2.0f,yPos1,leftCorner.m_z + size1 - bsize2/2.0f),bSize1,1.3f,bsize2);
  //Right Back
  Primitives::Cuboid c6(ngl::Vec3(leftCorner.m_x + size1 - bSize1/2.0f,yPos1,leftCorner.m_z + bsize2/2.0f),bSize1,1.3f,bsize2);

  /// Second layer base
  Primitives::Cuboid c7(ngl::Vec3(leftCorner.m_x + size1,-3.0f,leftCorner.m_z + size1/2.0f),size1*2,2,size1);
  /// Battlements
  float yPos2 = -1.5f;
  float bsize3 = size1 * 0.35f;
  float bsize4 = size1 * 0.2f;
  ngl::Vec3 leftCorner2 = ngl::Vec3(leftCorner.m_x + size1, -10, -10);
  //Front Left
  Primitives::Cuboid c8(ngl::Vec3(leftCorner2.m_x + bsize4/2.0f, yPos2, leftCorner.m_z + size1 - bSize1/2.0f),bsize4,1.0f,bSize1);
  //Front Mid
  Primitives::Cuboid c9(ngl::Vec3(leftCorner2.m_x + size1/2.0f, yPos2, leftCorner.m_z + size1 - bSize1/2.0f),bsize4*0.8f,1.0f,bSize1);
  //Front Right
  Primitives::Cuboid c10(ngl::Vec3(leftCorner2.m_x + size1 - bsize4/2.0f, yPos2, leftCorner.m_z + size1 - bSize1/2.0f),bsize4,1.0f,bSize1);
  //Back
  Primitives::Cuboid c11(ngl::Vec3(leftCorner2.m_x + size1/2.0f,yPos2,leftCorner.m_z + bSize1/2.0f),size1,1.0f,bSize1);
  //Right
  Primitives::Cuboid c12(ngl::Vec3(leftCorner2.m_x + size1 - bSize1/2.0f,yPos2,leftCorner.m_z + size1/2.0f),bSize1,1.0f,bsize3);

  _list.push_back(c1);
  _list.push_back(c2);
  _list.push_back(c3);
  _list.push_back(c4);
  _list.push_back(c5);
  _list.push_back(c6);

  _list.push_back(c7);
  _list.push_back(c8);
  _list.push_back(c9);
  _list.push_back(c10);
  _list.push_back(c11);
  _list.push_back(c12);

  /// Legs
  float legSize = 1.3f;
  Primitives::Cuboid c13(ngl::Vec3(leftCorner.m_x + legSize/2.0f,-8.0f,leftCorner.m_z + legSize/2.0f),legSize,9,legSize);
  Primitives::Cuboid c14(ngl::Vec3(leftCorner.m_x + legSize/2.0f,-8.0f,leftCorner.m_z + size1 - legSize/2.0f),legSize,9,legSize);
  Primitives::Cuboid c15(ngl::Vec3(leftCorner.m_x + size1,-8.0f,leftCorner.m_z + size1 - legSize/2.0f),legSize,9,legSize);
  Primitives::Cuboid c16(ngl::Vec3(leftCorner.m_x + size1,-8.0f,leftCorner.m_z + legSize/2.0f),legSize,9,legSize);
  Primitives::Cuboid c17(ngl::Vec3(leftCorner.m_x + size1*2.0f - legSize/2.0f,-8.0f,leftCorner.m_z + size1 - legSize/2.0f),legSize,9,legSize);
  Primitives::Cuboid c18(ngl::Vec3(leftCorner.m_x + size1*2.0f - legSize/2.0f,-8.0f,leftCorner.m_z + legSize/2.0f),legSize,9,legSize);

  _list.push_back(c13);
  _list.push_back(c14);
  _list.push_back(c15);
  _list.push_back(c16);
  _list.push_back(c17);
  _list.push_back(c18);

  /// Floor
  float floorSize = 13;
  float bSize5 = bSize1 * 0.8f;
  float zOffset = -2.0f;
  Primitives::Cuboid c19(ngl::Vec3(leftCorner.m_x + size1*2 ,-7.0f,leftCorner.m_z + size1 +0.5f+ zOffset),floorSize,2,floorSize);
  //Front
  Primitives::Cuboid c20(ngl::Vec3(leftCorner.m_x + size1*2, -6.0f, leftCorner.m_z + size1*2.0f + 0.75f + zOffset),floorSize,1.5f,bSize5);
  //Back
  Primitives::Cuboid c21(ngl::Vec3(leftCorner.m_x + size1*2, -6.0f, leftCorner.m_z + 0.75f - bSize5 + zOffset),floorSize,1.5f,bSize5);
  //Left
  Primitives::Cuboid c22(ngl::Vec3(leftCorner.m_x + size1-0.25f, -6.0f, leftCorner.m_z + size1 + bSize5 + zOffset),bSize5,1.5f,floorSize);
  //Right
  Primitives::Cuboid c23(ngl::Vec3(leftCorner.m_x + size1+floorSize-0.75f, -6.0f, leftCorner.m_z + size1 + bSize5 + zOffset),bSize5,1.5f,floorSize);

  _list.push_back(c19);
  _list.push_back(c20);
  _list.push_back(c21);
  _list.push_back(c22);
  _list.push_back(c23);
}

void ObstacleScene::rotateCuboid(Primitives::Cuboid &_c, ngl::Vec3 _rot)
{
  /*ngl::Transformation transform;
  transform.reset();
  transform.setRotation(_rot.m_x, _rot.m_y, _rot.m_z);
  for (int i=0; i<_c.m_sides.size(); i++)
  {
    for (int j=0; j<4; j++)
    {
      _c.m_sides[i].m_normal = _c.m_sides[i].m_normal * transform.getMatrix();
      _c.m_sides[i].m_p1 = _c.m_sides[i].m_p1 * transform.getMatrix();
      _c.m_sides[i].m_p2 = _c.m_sides[i].m_p2 * transform.getMatrix();
      _c.m_sides[i].m_p3 = _c.m_sides[i].m_p3 * transform.getMatrix();
      _c.m_sides[i].m_p4 = _c.m_sides[i].m_p4 * transform.getMatrix();
    }
  }*/
}
