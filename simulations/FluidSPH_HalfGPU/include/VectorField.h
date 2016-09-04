#ifndef VECTORFIELD_H__
#define VECTORFIELD_H__

#include <ngl/Vec3.h>
#include <VectorNode.h>
#include <vector>
#include <ngl/VertexArrayObject.h>

//----------------------------------------------------------------------------------------------------------------------
/// @file VectorField.h
/// @brief Creates a cubic field of VectorNode objects. Acts as an interface for changing the vectors of those nodes and changing their magnitude.
/// @author Ewan Rice
/// @version 1.0
/// @date 28/01/16
/// @class VectorField
/// @brief Creates a cubic field of VectorNode objects. Acts as an interface for changing the vectors of those nodes and changing their magnitude.
//----------------------------------------------------------------------------------------------------------------------

class VectorField
{

/// @brief Struct for containing the data to be sent to shaders.
struct drawData
{
  ngl::Vec3 pos;
  ngl::Vec3 vector;
  ngl::Vec3 color;
};

public:
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Enum data structure. Stores the types of wind functions available.
  enum VectorFunction {turbulentWind, cyclone, sinwave, swirly, sideways};
  //----------------------------------------------------------------------------------------------------------------------


  /// @brief Constructor. Initializes VectorField and creates the grid of Vector Nodes.
  VectorField(ngl::Vec3 _centerPosition, int _numCellsWidth, int _numCellsHeight, int _numCellsDepth, ngl::Vec3 _cellSize);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Destructor
  ~VectorField();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Set the magnitude of the each Vector Node vector. May be thought of as wind strength.
  void setVectorMagnitude(float _magnitude);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Set the same vector for all cells.
  void setGlobalVector(ngl::Vec3 _v);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Set the function used to calculate the Vector Node vectors. An int is passed in which corresponds to a function. There are currently only 5 functions so the int should be in the range of 1 to 5, otherwise it will default to the first function.
  void setVectorFunction(unsigned int _i);
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Call once per frame. This applies the Vector Node vectors to the particles that are inside those nodes.
  void applyVector();
  //----------------------------------------------------------------------------------------------------------------------
  /// @brief Call every frame that the scene is drawn. This applies the Vector Node vectors to the particles that are inside those nodes.
  void draw(ngl::Mat4 _VP);
  //----------------------------------------------------------------------------------------------------------------------

  ngl::Vec3 m_position;
  std::vector<VectorNode> m_nodeList;
  float m_vectorMagnitude;

private:

  void setupVAO();
  std::vector<drawData> getDrawData();

  ngl::Vec3 getVectorValueFromFunction(VectorFunction _functionType, ngl::Vec3 _position);
  ngl::Vec3 cycloneFunction(ngl::Vec3 position);
  ngl::Vec3 turbulentWindFunction(ngl::Vec3 position);
  ngl::Vec3 sinwaveFunction(ngl::Vec3 position);
  ngl::Vec3 swirlyFunction(ngl::Vec3 position);
  ngl::Vec3 sidewaysFunction();

  ngl::VertexArrayObject *m_vao;

  int m_numCellsWidth;
  int m_numCellsHeight;
  int m_numCellsDepth;
};

#endif // VECTORFIELD_H__

