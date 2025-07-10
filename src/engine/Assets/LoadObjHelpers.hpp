#ifndef ENGINE_LOADOBJHELPERS
#define ENGINE_LOADOBJHELPERS

#include <string>
#include <vector>
#include <exception>

#include "LoadedMesh.hpp"
#include "utils.hpp"

namespace Engine::Assets {

/// @brief stores the coordinates for a pair of texture coordinates
struct TexCoords {
  float u;
  float v;
};

/**
 * @brief Creates a vertex using a Vec3f and a set of UVs
 * @param vec The Vec3f that determines the vertexs position
 * @param uv The TexCoords that determines the vertexs UV values
 * @returns The new vertex
 */
Engine::Graphics::Vertex createVertex( const Engine::Vec3f & vec,
                                       const TexCoords & uv );

/**
 * @brief Takes a string and splits it on a particular character.
 * @param str The string to split.
 * @patam c The character to split it on.
 * @returns A vector of substrings of str split on c.
 *
 * reads through str character by character and adds each string to the vector as it splits them
 *
 * ex. str = "hello bob/jill", c = '/'
 * result: [ "hello bob", "jill" ]
 */
std::vector<std::string> splitString( const std::string & str, char c );

/**
 * @brief parses a vertex line from an obj file
 * @param line the line the vertex is parsed from
 * @returns the vertex that was read from the line
 * 
 * Used for reading .OBJ files. 
 * It will throw an error if the line isn't formatted correctly
 */
Vec3f parseVertex( std::string & line );

/**
 * @brief parses a UV line from an obj file
 * @param line the line the UV is parsed from
 * @returns the UV coords that were read from the line
 * 
 * Used for reading .OBJ files. 
 * It will throw an error if the line isn't formatted correctly
 */
TexCoords parseUV( std::string & line );

/**
 * @brief parses a normal vector line from an obj file
 * @param line the line the normal vector is parsed from
 * @returns the normal vector that was read from the line
 * 
 * Used for reading .OBJ files. 
 * It will throw an error if the line isn't formatted correctly
 */
Vec3f parseNormal( std::string & line );

/**
 * @brief parses a face line from an obj file
 * @param line the line the face is parsed from
 * @param vertexes the collection of vertexes that the face will reference
 * @param uVs the collection of UV coordinates that the face will reference
 * @returns the collection of triangles that make up the face
 * 
 * Used for reading .OBJ files. 
 * It will throw an error if the line isn't formatted correctly
 */
std::vector<Tri> parseFace( std::string & line, const std::vector<Vec3f> & vertexes, const std::vector<TexCoords> & uVs );

/**
 * @brief parses a color from a diffuse lighting line
 * @param line the diffuse lighting line to parse
 * @returns a color with r, g, and b specified by the diffuse color.
 * 
 * Used to parse the kd lines in a MTL file. 
 * The opacity ( `a` ) of the returned line is always `0xff`
 */
Engine::Color parseDiffuse( std::string line );

class InvalidLineException : public std::exception {
public:
  virtual const char* what() const throw();

}; //InvalidLineException 

} //PotionParts

/**
 * @brief addition operator for TexCoords
 * @param x a TexCoord to add
 * @param y a TexCoord to add
 * @returns the piecewise sum of x and y
 */
Engine::Assets::TexCoords operator+( const Engine::Assets::TexCoords& x, const Engine::Assets::TexCoords & y );

#endif //ENGINE_LOADOBJHELPERS