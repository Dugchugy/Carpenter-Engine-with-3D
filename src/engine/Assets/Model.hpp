#ifndef ENGINE_MODEL
#define ENGINE_MODEL

#include "Transform.hpp"
#include <Graphics/Mesh.hpp>
#include <Graphics/Texture.hpp>
#include <Graphics/Renderer.hpp>
#include <vector>

namespace PotionParts {

/// @brief stores both a mesh and a texture to be used for rendering a model. model itself references the model base to determine its rendering process
class ModelBase {

private:

  struct TexturedMesh {
    Engine::Graphics::Mesh mesh;
    Engine::Graphics::Texture texture;
  }; 

  std::vector<TexturedMesh> meshes;
  
public:

  ModelBase();

  ModelBase( Engine::Graphics::Mesh m, Engine::Graphics::Texture t );

  void addMesh( Engine::Graphics::Mesh m, Engine::Graphics::Texture t );

  /// @brief draws this mesh using the given renderer and transform
  void draw( Engine::Graphics::Renderer& renderer, Transform transform );

  std::vector<TexturedMesh> getMeshes();

}; //ModelBase

class Model {
public:

  Model( ModelBase* base );

  /// @brief draws this Model using the given renderer and transform
  void draw( Engine::Graphics::Renderer& renderer, const Transform transform ) const;

private:

  ModelBase* _base;

}; //Model

/// @brief Singleton class that is used to load models from asset files. keeps memory usage down by reducing number of duplicate models loaded
class ModelManager {
public:
  /// @brief gets the model manager Singleton. if it does not exist, creates it
  static ModelManager & getManager();

  /// @brief loads an STL model from the passed filename
  /// @param filename the path to the STL file. must end in '.stl'
  /// checks the internal cache before loading the file and only loads it if it hasn't been loaded before
  Model loadStlModel( std::string filename );

  /// @brief loads a cube mesh with the appropriate texture
  /// @param textFilename the path to the PNG texture. must be a PNG file
  /// checks the internal cache before loading the file and only loads it if it hasn't been loaded before
  Model loadCube( char* textFilename );

  /// @brief loads an OBJ model from the passed filename
  /// @param filename the the path to the .obj file
  /// @return the model read from the file
  /// checks the internal cache before loading the file and only loads it if it hasn't been loaded before
  /// also loads any .mtl files required by the .obj file as well as any textures
  Model loadObjModel( std::string filename );

private:
  ModelManager();

  /// @brief checks if a model has been loaded before and returns a pointer to it if it has
  ModelBase* checkLoaded( std::string filename );

  static ModelManager* manager;

  std::unordered_map<std::string, ModelBase*> modelMap;

}; //ModelManager

} //PotionParts

#endif //ENGINE_MODEL