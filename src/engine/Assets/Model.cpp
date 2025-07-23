#include "Model.hpp"

#include <Graphics/Shapes.hpp>
#include <iostream>
#include <vector>

#include "LoadObjHelpers.hpp"
#include "AssetStream.hpp"

using namespace Engine::Assets;

Model::Model(): 
  meshes( std::vector<TexturedMesh>() ) {}

Model::Model( Engine::Graphics::Mesh m, Engine::Graphics::Texture t ) : 
    Model() {
  addMesh( m, t );
}

void Model::addMesh( Engine::Graphics::Mesh m, Engine::Graphics::Texture t ) {
  meshes.push_back( { m, t } );
}

void Model::draw( Engine::Graphics::Renderer& renderer,
                  const Engine::Vec3f & position,
                  const Engine::Vec3f & rotation,
                  const Engine::Vec3f & scale ) {
  for ( int i = 0; i < meshes.size(); i++ ) {
    std::cout << "drawing mesh " << i << " of model\n";
    renderer.UseTexture( meshes[ i ].texture, GL_TEXTURE0 );
    renderer.DrawMesh( & ( meshes[ i ].mesh ), position, scale, rotation );
  }
} //ModelBase::draw( render, position, rotation, scale )

Model Engine::Assets::loadStlModel( std::string filename ) {

  LoadedMesh mesh = loadStlMesh( filename );
  // uses a plain gray texture
  //Engine::Graphics::ColorTexture text( { 0x77, 0x77, 0x77, 0xff } );
  Engine::Graphics::Texture( "daGreyPixel.png" );

  return Model( mesh, text );
}

Model Engine::Assets::loadCube( char* textFilename ) {
    
  Engine::Graphics::Cube cube;
  Engine::Graphics::Texture text( textFilename );

  return Model( cube, text );
}

Model Engine::Assets::loadObjModel( std::string filename ) {

  Model base = Model();

  std::vector< Vec3f > verts;
  std::vector< TexCoords > uvs;
  std::vector< Tri > Tris;

  Engine::Assets::AssetStream stream( filename );
  stream.open();
  std::string line = "";
  stream >> line;

  while ( line != "" ) {

    if( line[0] == 'o' ) {
      if ( Tris.size() > 0 ) {
        std::cout << "new object has " << Tris.size() << " tris\n";
        base.addMesh( LoadedMesh( Tris ), 
          Engine::Graphics::Texture( "Assets/Placeholder.png" ) );
      }
      std::cout << "starting object: " << line << "\n";

      // no need to clear verts/uvs. face indicies are global
      Tris = std::vector< Tri >();
    }

    if( line[0] == 'v' && line[1] == ' ' ) {
      verts.push_back( parseVertex( line ) );
    }

    if( line[0] == 'v' && line[1] == 't' ) {
      uvs.push_back( parseUV( line ) );
    }

    if( line[0] == 'f' ) {
      //std::cout << "parsing face: " << line << "\n";
      std::vector< Tri > newTris = parseFace( line, verts, uvs );
      while ( newTris.size() > 0 ) {
        Tris.push_back( newTris.back() );
        newTris.pop_back();
      }
    }

    //all others (comment on unknown) are skipped
    stream >> line;
  }

  if ( Tris.size() > 0 ) {
    std::cout << "adding last object\n";
    std::cout << "new object has " << Tris.size() << " tris\n";
    base.addMesh( LoadedMesh( Tris ), 
      Engine::Graphics::Texture( "Assets/Placeholder.png" ) );
  }

  return base;
}