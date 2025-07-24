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
    renderer.UseTexture( meshes[ i ].texture, GL_TEXTURE0 );
    renderer.DrawMesh( & ( meshes[ i ].mesh ), position, scale, rotation );
  }
} //ModelBase::draw( render, position, rotation, scale )

Model Engine::Assets::loadStlModel( std::string filename ) {

  LoadedMesh mesh = loadStlMesh( filename );
  // uses a plain gray texture
  Engine::Graphics::ColorTexture text( { 0x77, 0x77, 0x77, 0xff } );

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
  std::unordered_map< std::string, std::vector< Tri > > Tris;
  std::vector< std::string > textureNames;

  Engine::Assets::AssetStream stream( filename );
  stream.open();
  std::string line = "";
  stream >> line;

  std::unordered_map<std::string, Engine::Graphics::Texture> currentTextMap;
  std::string currentText = "";

  while ( line != "" ) {

    /*if( line[0] == 'o' ) {
      if ( Tris.size() > 0 ) {
        std::cout << "new object has " << Tris.size() << " tris\n";
        if ( currentText == "" ) {
          base.addMesh( LoadedMesh( Tris ), 
            Engine::Graphics::Texture( "Assets/Placeholder.png" ) );
        } else {
          base.addMesh( LoadedMesh( Tris ), 
            currentTextMap.at( currentText ) );
        }
      }
      std::cout << "starting object: " << line << "\n";

      // no need to clear verts/uvs. face indicies are global
      Tris = std::vector< Tri >();
    }*/

    if ( line.length() > 6 ) {
      std::vector< std::string > lineParts = splitString( line, ' ' );

      if ( lineParts[ 0 ] == "mtllib" ) {

        std::vector< std::string > fileNameParts = splitString( filename, '/' );
        fileNameParts.pop_back();
        std::string mtlFileName = "";
        for ( int i = 0; i < fileNameParts.size(); i++ ) {
          if ( i > 0 ) {
            mtlFileName += "/";
          }
          mtlFileName += fileNameParts[ i ];
        }
        mtlFileName += "/" + lineParts[ 1 ];

        currentTextMap = parseMtlFile( mtlFileName );
      }

      if ( lineParts[ 0 ] == "usemtl" ) {
        std::cout << "using mtl " << lineParts[ 1 ] << "\n";
        Tris.insert( { lineParts[ 1 ], std::vector< Tri >() } );
        textureNames.push_back( lineParts[ 1 ] );
        currentText = lineParts[ 1 ];
      }
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

      if ( !Tris.contains( currentText ) ) {
        Tris.insert( { currentText, std::vector< Tri >() } );
      }

      while ( newTris.size() > 0 ) {
        Tris.at( currentText ).push_back( newTris.back() );
        newTris.pop_back();
      }
    }

    //all others (comment on unknown) are skipped
    stream >> line;
  }

  if ( Tris.size() > 0 ) {
    std::cout << "adding last object\n";

    for ( int i = 0; i < textureNames.size(); i++ ) {
      if ( Tris.contains( textureNames[ i ] ) ) {
        std::cout << "adding mesh for mlt " << textureNames[ i ] << "\n";
        if ( currentTextMap.contains( textureNames[ i ] ) ) {
          base.addMesh( LoadedMesh( Tris.at( textureNames[ i ] ) ), 
            currentTextMap.at( textureNames[ i ] ) );
        } else {
          std::cout << "doesn't have texture for mtl\n";
          base.addMesh( LoadedMesh( Tris.at( textureNames[ i ] ) ), 
            Engine::Graphics::Texture( "Assets/Placeholder.png" ) );
        }
      }
    }
  }

  return base;
}