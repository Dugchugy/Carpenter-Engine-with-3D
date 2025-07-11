#ifndef ENGINE_LOADEDMESH
#define ENGINE_LOADEDMESH

#include"Graphics/Mesh.hpp"
#include<vector>

namespace Engine::Assets {

struct Tri {
  Engine::Graphics::Vertex v1;
  Engine::Graphics::Vertex v2;
  Engine::Graphics::Vertex v3;
}; //Tri

class LoadedMesh : public Engine::Graphics::Mesh {
public:

  /**
   * @brief Creates a mesh from an array of triangles.
   * @param triangles The array of triangles that specifies all the faces of the mesh
   */
  LoadedMesh( std::vector<Tri> triangles );

}; //LoadedMesh

LoadedMesh loadStlMesh( std::string filename );

}

#endif //ENGINE_LOADEDMESH