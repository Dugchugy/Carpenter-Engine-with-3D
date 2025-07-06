#include "LoadObjHelpers.hpp"

#include <iostream>

using namespace Engine::Assets;

std::vector<std::string> Engine::Assets::splitString( const std::string & str, char c ){
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

Vec3f Engine::Assets::parseVertex( std::string & line ) {
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

  return Vec3f( x, y, z );
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

Vec3f Engine::Assets::parseNormal( std::string & line ) {
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

  return Vec3f( x, y, z );
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

  Vec3f avgVert = Vec3f( 0, 0, 0 );
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
      faceVertexes[ 0 ].toVertex( faceUvs[ 0 ].u, faceUvs[ 0 ].v ),
      faceVertexes[ 1 ].toVertex( faceUvs[ 1 ].u, faceUvs[ 1 ].v ),
      faceVertexes[ 2 ].toVertex( faceUvs[ 2 ].u, faceUvs[ 2 ].v )
    } );
  } else if ( split.size() == 5 ) {
    // adds a quad (4 vertices)
    result.push_back( {
      faceVertexes[ 0 ].toVertex( faceUvs[ 0 ].u, faceUvs[ 0 ].v ),
      faceVertexes[ 1 ].toVertex( faceUvs[ 1 ].u, faceUvs[ 1 ].v ),
      faceVertexes[ 2 ].toVertex( faceUvs[ 2 ].u, faceUvs[ 2 ].v )
    } );
    result.push_back( {
      faceVertexes[ 2 ].toVertex( faceUvs[ 2 ].u, faceUvs[ 2 ].v ),
      faceVertexes[ 3 ].toVertex( faceUvs[ 3 ].u, faceUvs[ 3 ].v ),
      faceVertexes[ 0 ].toVertex( faceUvs[ 0 ].u, faceUvs[ 0 ].v )
    } );
  } else {
    //uses the calculated average vertex as a middle point for a face
    for ( int i = 1; i < split.size(); i++ ) {
      if ( i == ( split.size() - 1 ) ) {
        result.push_back( {
          faceVertexes[ i - 1 ].toVertex( faceUvs[ i - 1 ].u, faceUvs[ i - 1 ].v ),
          faceVertexes[ 0 ].toVertex( faceUvs[ 0 ].u, faceUvs[ 0 ].v ),
          avgVert.toVertex( avgTex.u, avgTex.v )
        } );
      } else {
        result.push_back( {
          faceVertexes[ i - 1 ].toVertex( faceUvs[ i - 1 ].u, faceUvs[ i - 1 ].v ),
          faceVertexes[ i ].toVertex( faceUvs[ i ].u, faceUvs[ i ].v ),
          avgVert.toVertex( avgTex.u, avgTex.v )
        } );
      }
    }
  }

  return result;
}

TexCoords operator+( const TexCoords & x, const TexCoords & y ) {
  return { x.u + y.u, x.v + y.v };
}

const char* InvalidLineException::what() const throw() {

}