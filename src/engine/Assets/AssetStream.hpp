#ifndef ENGINE_ASSETSTREAM
#define ENGINE_ASSETSTREAM

#include <string>
#include <emscripten.h>
#include <cstdint>
#include <exception>

#include "Utils.hpp"

namespace Engine::Assets {

class AssetStream {
public:

  /**
   * @brief Creates an AssetStream to access the asset at the passed path
   * @param path The tath to the Asset this stream will access
   * Creates a new AssetStream pointing to the specified asset path. 
   * Once the asset is opened, bytes can be streamed out of it to be processed by another system.
   */
  AssetStream( std::string path );

  /**
   * @brief the copy constructor for the Asset stream
   * @param a the asset to copy
   * creates a copy of the passed AssetStream. 
   * all data will be copied including any current asset data, current read position, and whether or not the asset is opened.
   */
  AssetStream( const AssetStream& a );

  /**
   * @brief AssetStream destructor
   */
  ~AssetStream();

  /**
   * @brief the equals operator
   * @param a The AssetStream that this AssetStream will be set to.
   * @returns a const reference to AssetStream a so the operator may be chained.
   * sets this AssetStream equal to the passed AssetStream
   * this returns the previous AssetStream allowing the equals operators to be chained
   */
  const AssetStream& operator=( const AssetStream& a );

  /**
   * @brief takes the asset data from the stream
   * @return the void* that points to the underlying data
   * Takes the asset data from the AssetStream.
   * after this, the asset stream returns to an unopened state and operations that require
   * the asset data cannot be performed until the asset is opened again.
   *
   * after this, the user is responsible for freeing the returned asset data using free( x ) to prevent memory leaks.
   */
  void* takeAsset();

  /**
   * @breif opens the asset stream
   * When called, the AssetStream will attempt to open the asset at its path and load it into memory. if unsuccessful it may throw an error
   * This operation must be called before any data can be read from the asset stream
   */
  void open();

  /**
   * @brief skips the specified number of bytes
   * @param bytes the number of bytes to skip
   * Moves the index in the asset forward by bytes.
   */
  void skipBytes( const int& bytes );

  /** 
   * @brief reads bytes until the specified character is read or the file ends.
   * @param end the character to end the read on
   * Reads bytes as chars adding each one to a string. 
   * Stops and returns the string if the specified character is found or the file ends.
   * The resulting string will not include the specified character
   */
  std::string readUntil( char end );

  /**
   * @brief Extraction operator for floats
   * @param x The float to write the data into.
   * @returns A reference to the AssetStream so extractions can be chained
   * Reads 4 bytes out of the Asset stream and uses their value to set the passed float, x.
   *
   * can be chained.
   */
  AssetStream& operator>>( float& x );

  /**
   * @brief Extraction operator for ints
   * @param x The int to write the data into.
   * @returns A reference to the AssetStream so extractions can be chained
   * Reads 4 bytes out of the Asset stream and uses their value to set the passed int, x.
   *
   * can be chained.
   */
  AssetStream& operator>>( int& x );

  /**
   * @brief Extraction operator for bytes
   * @param x The byte to write the data into.
   * @returns A reference to the AssetStream so extractions can be chained
   * Reads a byte out of the Asset stream and sets x to its value.
   *
   * can be chained.
   */
  AssetStream& operator>>( uint8_t& x );

  /**
   * @brief Extraction operator for chars
   * @param x The char to write the data into.
   * @returns A reference to the AssetStream so extractions can be chained
   * Reads a byte out of the Asset stream and uses its value to set the passed char, x.
   *
   * can be chained.
   */
  AssetStream& operator>>( char& x );

  /**
   * @brief Extraction operator for Vec3fs
   * @param x The Vec3f to write the data into.
   * @returns A reference to the AssetStream so extractions can be chained
   * Reads 12 bytes out of the Asset stream and uses their value to set the passed Vec3f, x.
   * The twelve bytes are ready as 3 floats in the order x, y, z.
   *
   * can be chained.
   */
  AssetStream& operator>>( Vec3f& x );

  /// @brief extraction operator for strings
  /// extracts one line from the stream. 
  /// extracts bytes one at a time until either the current line ends or the file ends
  AssetStream& operator>>( std::string& x );

private:

  uint8_t* _asset;
  int _position;
  std::string _path;
  int _size;

}; //AssetStream


class AccessException : public std::exception {
public:
  
  AccessException( int err ) : ErrorNum( err ) {}

  virtual const char* what() const throw();

  int ErrorNum;

}; //AccessException 

class NotOpenedException : public std::exception {

  virtual const char* what() const throw();

}; // NotOpenedException 

class EndOfFileException : public std::exception {

  virtual const char* what() const throw();

}; // EndOfFileException

} //Engine::Assets

#endif //ENGINE_ASSETSTREAM