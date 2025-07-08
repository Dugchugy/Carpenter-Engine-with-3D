#ifndef LOADOBJHELPERS_HPP
#define LOADOBJHELPERS_HPP

#include <string>
#include <vector>
#include <exception>

#include "LoadedMesh.hpp"

namespace Engine::Assets {

/// @brief stores the coordinates for a pair of texture coordinates
struct TexCoords {
  float u;
  float v;
};

std::vector<std::string> splitString( const std::string & str, char c );

Vec3f parseVertex( std::string & line );

TexCoords parseUV( std::string & line );

Vec3f parseNormal( std::string & line );

std::vector<Tri> parseFace( std::string & line, const std::vector<Vec3f> & vertexes, const std::vector<TexCoords> & uVs );

class InvalidLineException : public std::exception {
public:
  virtual const char* what() const throw();

}; //InvalidLineException 

} //PotionParts

Engine::Assets::TexCoords operator+( const Engine::Assets::TexCoords& x, const Engine::Assets::TexCoords & y );

#endif //LOADOBJHELPERS_HPP