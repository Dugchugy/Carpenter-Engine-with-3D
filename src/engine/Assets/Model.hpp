#ifndef ENGINE_MODEL
#define ENGINE_MODEL

#include <Graphics/Mesh.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/Renderer.hpp>
#include <vector>

namespace Engine::Assets {

/**
 * A model with a collection of meshes and textures.
 * Used to simplify rendering of 3D models
 */
class Model {

private:

  struct TexturedMesh {
    Engine::Graphics::Mesh mesh;
    Engine::Graphics::Texture texture;
  }; 

  std::vector<TexturedMesh> meshes;
  
public:

  /**
   * @brief Constructs an empty model.
   * 
   * Creates a model with no meshes and no textures. 
   */
  Model();

  /**
   * @breif Constructs a model from a mesh and a texture
   * @param m The mesh to use for this model
   * @param t The texture to use for this model
   * 
   * creates a new model that contains a single mesh and a single texture.
   */
  Model( Engine::Graphics::Mesh m, Engine::Graphics::Texture t );

  /**
   * @brief adds a mesh and texture pair to the model
   * @param m the mesh to add to the model
   * @param t the texture used for this model
   */
  void addMesh( Engine::Graphics::Mesh m, Engine::Graphics::Texture t );

  /**
   * @brief draws this mesh using the given renderer and transform
   * @param renderer the renderer to render this model with
   * @param position the position to render this model
   * @param rotation the rotation to render this model with 
   * @param scale the scale to render this model with
   */
  void draw( Engine::Graphics::Renderer& renderer,
             const Engine::Vec3f & position,
             const Engine::Vec3f & rotation,
             const Engine::Vec3f & scale );

}; //Model

/**
 * @brief loads an STL model from the passed filename
 * @param filename the path to the STL file. must end in '.stl'
 * @return a model containing the loaded STL model with a plain grey texture
 */
Model loadStlModel( std::string filename );

/**
 * @brief loads a cube mesh with the appropriate texture
 * @param textFilename the path to the PNG texture. must be a PNG file
 * @return a model containing a cube mesh with the loaded texture
 */
Model loadCube( char* textFilename );

/**
 * @brief loads an OBJ model from the passed filename
 * @param filename the the path to the .obj file
 * @return the obj model with any textures specified in the MTL file
 * 
 * also loads any .mtl files required by the .obj file as well as any textures they reference
 */
Model loadObjModel( std::string filename );

} //Engine::Assets

#endif //ENGINE_MODEL