#ifndef PRIMITIVES
#define PRIMITIVES

#include <ngl/Vec3.h>
#include <vector>

namespace Primitives {

  struct Sphere
  {
    ngl::Vec3 m_position;
    float m_radius;
  };

  struct Plane
  {
    ngl::Vec3 normal;
    ngl::Vec3 centerOfPlane;
    float m_length;
  };

  struct Rectangle
  {
    Rectangle(ngl::Vec3 _p1, ngl::Vec3 _p2, ngl::Vec3 _p3, ngl::Vec3 _p4)
    {
      m_p1 = _p1;
      m_p2 = _p2;
      m_p3 = _p3;
      m_p4 = _p4;

      m_normal = -(m_p2 - m_p1).cross(m_p3 - m_p1);
      m_normal.normalize();
      m_width = (m_p1 - m_p2).length();
      m_height = (m_p1 - m_p3).length();
    }

    void flipNormal()
    {
      m_normal *= -1;
    }

    ngl::Vec3 m_p1; //A corner of the plane
     //p2 and p3 are both corners connected to p1. Should be defined that (p2-p1).dot(p3-p1) = 1
    ngl::Vec3 m_p2;
    ngl::Vec3 m_p3;
    ngl::Vec3 m_p4;
    ngl::Vec3 m_normal;
    float m_width;
    float m_height;
  };

  struct Cuboid
  {
    ngl::Vec3 m_centre;
    float m_width;
    float m_height;
    float m_breadth;
    std::vector<Rectangle> m_sides;

    Cuboid(ngl::Vec3 _centre, float _width, float _height, float _breadth)
    {
      m_centre = _centre;
      m_width = _width;
      m_height = _height;
      m_breadth = _breadth;

      //Top face
      ngl::Vec3 corner1 = ngl::Vec3(m_centre.m_x - m_width/2.0f, m_centre.m_y + m_height/2.0f, m_centre.m_z - m_breadth/2.0f);
      ngl::Vec3 corner2 = ngl::Vec3(m_centre.m_x + m_width/2.0f, m_centre.m_y + m_height/2.0f, m_centre.m_z - m_breadth/2.0f);
      ngl::Vec3 corner3 = ngl::Vec3(m_centre.m_x - m_width/2.0f, m_centre.m_y + m_height/2.0f, m_centre.m_z + m_breadth/2.0f);
      ngl::Vec3 corner4 = ngl::Vec3(m_centre.m_x + m_width/2.0f, m_centre.m_y + m_height/2.0f, m_centre.m_z + m_breadth/2.0f);
      Rectangle r1(corner1, corner2, corner3, corner4);
      r1.m_normal = ngl::Vec3(0,1,0);
      m_sides.push_back(r1);

      //Bottom face
      corner1 = ngl::Vec3(m_centre.m_x - m_width/2.0f, m_centre.m_y - m_height/2.0f, m_centre.m_z - m_breadth/2.0f);
      corner2 = ngl::Vec3(m_centre.m_x + m_width/2.0f, m_centre.m_y - m_height/2.0f, m_centre.m_z - m_breadth/2.0f);
      corner3 = ngl::Vec3(m_centre.m_x - m_width/2.0f, m_centre.m_y - m_height/2.0f, m_centre.m_z + m_breadth/2.0f);
      corner4 = ngl::Vec3(m_centre.m_x + m_width/2.0f, m_centre.m_y - m_height/2.0f, m_centre.m_z + m_breadth/2.0f);
      Rectangle r2(corner1, corner2, corner3, corner4);
      r2.m_normal = ngl::Vec3(0,-1,0);
      m_sides.push_back(r2);

      //Left face
      corner1 = ngl::Vec3(m_centre.m_x - m_width/2.0f, m_centre.m_y - m_height/2.0f, m_centre.m_z - m_breadth/2.0f);
      corner2 = ngl::Vec3(m_centre.m_x - m_width/2.0f, m_centre.m_y + m_height/2.0f, m_centre.m_z - m_breadth/2.0f);
      corner3 = ngl::Vec3(m_centre.m_x - m_width/2.0f, m_centre.m_y - m_height/2.0f, m_centre.m_z + m_breadth/2.0f);
      corner4 = ngl::Vec3(m_centre.m_x - m_width/2.0f, m_centre.m_y + m_height/2.0f, m_centre.m_z + m_breadth/2.0f);
      Rectangle r3(corner1, corner2, corner3, corner4);
      r3.m_normal = ngl::Vec3(-1,0,0);
      m_sides.push_back(r3);

      //Right face
      corner1 = ngl::Vec3(m_centre.m_x + m_width/2.0f, m_centre.m_y - m_height/2.0f, m_centre.m_z - m_breadth/2.0f);
      corner2 = ngl::Vec3(m_centre.m_x + m_width/2.0f, m_centre.m_y + m_height/2.0f, m_centre.m_z - m_breadth/2.0f);
      corner3 = ngl::Vec3(m_centre.m_x + m_width/2.0f, m_centre.m_y - m_height/2.0f, m_centre.m_z + m_breadth/2.0f);
      corner4 = ngl::Vec3(m_centre.m_x + m_width/2.0f, m_centre.m_y + m_height/2.0f, m_centre.m_z + m_breadth/2.0f);
      Rectangle r4(corner1, corner2, corner3, corner4);
      r4.m_normal = ngl::Vec3(1,0,0);
      m_sides.push_back(r4);

      //Front face
      corner1 = ngl::Vec3(m_centre.m_x - m_width/2.0f, m_centre.m_y - m_height/2.0f, m_centre.m_z + m_breadth/2.0f);
      corner2 = ngl::Vec3(m_centre.m_x - m_width/2.0f, m_centre.m_y + m_height/2.0f, m_centre.m_z + m_breadth/2.0f);
      corner3 = ngl::Vec3(m_centre.m_x + m_width/2.0f, m_centre.m_y - m_height/2.0f, m_centre.m_z + m_breadth/2.0f);
      corner4 = ngl::Vec3(m_centre.m_x + m_width/2.0f, m_centre.m_y + m_height/2.0f, m_centre.m_z + m_breadth/2.0f);
      Rectangle r5(corner1, corner2, corner3, corner4);
      r5.m_normal = ngl::Vec3(0,0,1);
      m_sides.push_back(r5);

      //Back face
      corner1 = ngl::Vec3(m_centre.m_x - m_width/2.0f, m_centre.m_y - m_height/2.0f, m_centre.m_z - m_breadth/2.0f);
      corner2 = ngl::Vec3(m_centre.m_x - m_width/2.0f, m_centre.m_y + m_height/2.0f, m_centre.m_z - m_breadth/2.0f);
      corner3 = ngl::Vec3(m_centre.m_x + m_width/2.0f, m_centre.m_y - m_height/2.0f, m_centre.m_z - m_breadth/2.0f);
      corner4 = ngl::Vec3(m_centre.m_x + m_width/2.0f, m_centre.m_y + m_height/2.0f, m_centre.m_z - m_breadth/2.0f);
      Rectangle r6(corner1, corner2, corner3, corner4);
      r6.m_normal = ngl::Vec3(0,0,-1);
      m_sides.push_back(r6);
    }

  };

}

#endif // PRIMITIVES

