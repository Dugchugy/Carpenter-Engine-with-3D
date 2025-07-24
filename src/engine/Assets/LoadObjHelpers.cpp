#include "LoadObjHelpers.hpp"

#include <iostream>

#include "AssetStream.hpp"

using namespace Engine::Assets;

Engine::Graphics::Vertex Engine::Assets::createVertex( 
    const Engine::Vec3f & vec,
    const TexCoords & uv ) {
  return { vec.x, vec.y, vec.z, uv.u, uv.v };
}

std::vector<std::string> Engine::Assets::splitString( const std::string & str, char c ) {
  std::vector<std::string> subStrs;

  std::string currStr = "";

  for ( int i = 0; i < str.size(); i++ ){
    if ( str[i] == c ) {
      subStrs.push_back( currStr );
      currStr = "";
    } else {
      currStr = currStr + str[i];
    }
  }

  subStrs.push_back( currStr );

  return subStrs;
}

Engine::Vec3f Engine::Assets::parseVertex( std::string & line ) {
  std::vector<std::string> split = splitString( line, ' ' );

  if ( split.size() != 4 ) {
    std::cout << "thowing InvalidLineException from line 30\n";
    throw InvalidLineException();
  }

  if ( split[0] != "v" ) {
    std::cout << "thowing InvalidLineException from line 35\n";
    throw InvalidLineException();
  }

  float x = stof( split[1], nullptr );
  float y = stof( split[2], nullptr );
  float z = stof( split[3], nullptr );

  return { x, y, z };
}

TexCoords Engine::Assets::parseUV( std::string & line ){
  std::vector<std::string> split = splitString( line, ' ' );

  if ( split.size() != 3 ) {
    std::cout << "thowing InvalidLineException from line 50\n";
    throw InvalidLineException();
  }

  if ( split[0] != "vt" ) {
    std::cout << "thowing InvalidLineException from line 55\n";
    throw InvalidLineException();
  }

  float u = stof( split[1], nullptr );
  float v = stof( split[2], nullptr );

  if( u > 1 || u < 0 ) {
    std::cout << "u value of " << u << " is out of bounds\n";
  }

  if( v > 1 || v < 0 ) {
    std::cout << "v value of " << v << " is out of bounds\n";
  }

  return { u, v };
}

Engine::Vec3f Engine::Assets::parseNormal( std::string & line ) {
  std::vector<std::string> split = splitString( line, ' ' );

  if ( split.size() != 4 ) {
    std::cout << "thowing InvalidLineException from line 69\n";
    throw InvalidLineException();
  }

  if ( split[0] != "vn" ) {
    std::cout << "thowing InvalidLineException from line 74\n";
    throw InvalidLineException();
  }

  float x = stof( split[1], nullptr );
  float y = stof( split[2], nullptr );
  float z = stof( split[3], nullptr );

  return { x, y, z };
}

std::vector<Tri> Engine::Assets::parseFace( std::string & line, const std::vector<Vec3f> & vertexes,
                   const std::vector<TexCoords> & uVs ) {
  std::vector<std::string> split = splitString( line, ' ' );

  if ( split[0] != "f" ) {
    std::cout << "thowing InvalidLineException from line 90\n";
    throw InvalidLineException();
  }

  if ( split.size() < 4 ) {
    // must have at least 3 verticies for a face (triangle)
    std::cout << "thowing InvalidLineException from line 96\n";
    throw InvalidLineException();
  }

  std::vector<Tri> result;

  std::vector<Vec3f> faceVertexes;
  std::vector<TexCoords> faceUvs;

  Engine::Vec3f avgVert = { 0, 0, 0 };
  TexCoords avgTex = { 0, 0 };

  for ( int i = 1; i < split.size(); i++ ) {
    std::vector<std::string> indicies = splitString( split[i], '/' );

    // indicies all start at 1
    int vertIndex = stoi( indicies[0], nullptr ) - 1;
    int uvIndex = stoi( indicies[1], nullptr ) - 1; 

    faceVertexes.push_back( vertexes[ vertIndex ] );
    faceUvs.push_back( uVs[ uvIndex ] );

    avgVert = avgVert + vertexes[ vertIndex ];
    avgTex = avgTex + uVs[ uvIndex ];
  }
  float frac = 1.0f / ( split.size() - 1 );

  avgVert = avgVert * frac;
  avgTex = { avgTex.u * frac, avgTex.v * frac };

  if ( split.size() == 4 ) {
    result.push_back( {
      createVertex( faceVertexes[ 0 ], faceUvs[ 0 ] ),
      createVertex( faceVertexes[ 1 ], faceUvs[ 1 ] ),
      createVertex( faceVertexes[ 2 ], faceUvs[ 2 ] )
    } );
  } else if ( split.size() == 5 ) {
    // adds a quad (4 vertices)
    result.push_back( {
      createVertex( faceVertexes[ 0 ], faceUvs[ 0 ] ),
      createVertex( faceVertexes[ 1 ], faceUvs[ 1 ] ),
      createVertex( faceVertexes[ 2 ], faceUvs[ 2 ] )
    } );
    result.push_back( {
      createVertex( faceVertexes[ 2 ], faceUvs[ 2 ] ),
      createVertex( faceVertexes[ 3 ], faceUvs[ 3 ] ),
      createVertex( faceVertexes[ 0 ], faceUvs[ 0 ] )
    } );
  } else {
    //uses the calculated average vertex as a middle point for a face
    for ( int i = 1; i < split.size(); i++ ) {
      if ( i == ( split.size() - 1 ) ) {
        result.push_back( {
          createVertex( faceVertexes[ i - 1 ], faceUvs[ i - 1 ] ),
          createVertex( faceVertexes[ 0 ], faceUvs[ 0 ] ),
          createVertex( avgVert, avgTex )
        } );
      } else {
        result.push_back( {
          createVertex( faceVertexes[ i - 1 ], faceUvs[ i - 1 ] ),
          createVertex( faceVertexes[ i ], faceUvs[ i ] ),
          createVertex( avgVert, avgTex )
        } );
      }
    }
  }

  return result;
}

Engine::Color Engine::Assets::parseDiffuse( std::string line ) {
  std::vector<std::string> split = splitString( line, ' ' );

  if ( split.size() != 4 || split[ 0 ] != "kd" ) {
    std::cout << "invalid line " << line << "\n";
    throw InvalidLineException();
  }

  Engine::Color c = { 0xff, 0xff, 0xff, 0xff };
  c.r = c.r * stof( split[ 1 ], nullptr );
  c.g = c.g * stof( split[ 2 ], nullptr );
  c.b = c.b * stof( split[ 3 ], nullptr );
  return c;
}

std::unordered_map<std::string, Engine::Graphics::Texture> 
  Engine::Assets::parseMtlFile( std::string filename ) {
  
  Engine::Assets::AssetStream stream( filename );
  stream.open();
  std::string line = "";

  std::string currentName = "";
  Engine::Color currentDiffuse;
  float currentOpacity = 0.0f;
  stream >> line;

  std::unordered_map< std::string, Engine::Graphics::Texture> map;

  while( !stream.isEmpty() ) {
    std::cout << "parsing line " << line << "\n";

    if ( line.length() > 6 ) {
      if ( line[ 0 ] == 'n' && line[ 1 ] == 'e' &&
           line[ 2 ] == 'w' && line[ 3 ] == 'm' &&
           line[ 4 ] == 't' && line[ 5 ] == 'l' ) {

        if ( currentName != "" ) {
          std::cout << "adding map entry for mtl " << currentName << "\n";
          currentDiffuse.a = currentDiffuse.a * currentOpacity;
          map.insert( { 
            currentName, 
            Engine::Graphics::ColorTexture( currentDiffuse )
          } );
        }

        currentOpacity = 0.0f;
        currentDiffuse = Engine::Color();
        currentName = splitString( line, ' ' )[ 1 ];
      }

      if ( line[ 0 ] == 'K' && line[ 1 ] == 'd') {
        currentDiffuse = parseDiffuse( line );
      }

      if ( line[ 0 ] == 'd' ) {
        currentOpacity = stof( splitString( line, ' ' )[ 1 ] );
      }
    }

    stream >> line;
  }

  // adds the last entry to the map if needed
  if ( currentName != "" ) {
    currentDiffuse.a = currentDiffuse.a * currentOpacity;
    std::cout << "adding map entry for mtl " << currentName << "\n";
    map.insert( { 
      currentName, 
      Engine::Graphics::ColorTexture( currentDiffuse )
    } );
  }

  return map;
}

TexCoords operator+( const TexCoords & x, const TexCoords & y ) {
  return { x.u + y.u, x.v + y.v };
}

const char* InvalidLineException::what() const throw() {

}