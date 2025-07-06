#ifndef ENGINE_ASSETSTREAM
#define ENGINE_ASSETSTREAM

#include <string>
#include <emscripten.h>
#include <cstdint>
#include <exception>

namespace Engine::Assets {

class AssetStream {
public:

  /// @brief creates an AssetStream to access the asset at the passed path
  /// @param path the path to the Asset this stream will access
  AssetStream( std::string path );

  /// @brief the copy constructor for the Asset stream, copies it's filename and opens a copy of the asset if it is open
  /// @param a the asset to copy
  AssetStream( const AssetStream& a );

  /// @brief AssetStream destructor
  ~AssetStream();

  /// @brief the equals operator
  /// sets this AssetStream equal to the passed AssetStream
  const AssetStream& operator=( const AssetStream& a );

  /// @brief takes the underlying void* from the asset
  /// @return the void* used by this asset stream to store the assets data
  /// this also sets the pointer in the AssetStream to nullptr ensuring the AssetStream doesn't unexpected free the memory. this moves full responsibility for freeing the memory to the caller of takeAsset, returning the AssetStream to an unopened state
  void* takeAsset();

  /// @breif opens the asset stream
  /// when called, the AssetStream will attempt to open the asset at its path and load it into memory. if unsuccessful it may throw an error
  /// must be called before reading data from the AssetStream.
  void open();

  /// @brief skips some bytes
  /// moves the index in the asset forward by bytes.
  void skipBytes( const int& bytes );

  /// @brief reads bytes until the specified character is read or the file ends.
  /// @param end the character to end the read on
  /// the resulting string will not include the specified character
  std::string readUntil( char end );

  /// @brief extraction operator for floats
  /// pulls 4 bytes out of the AssetStream and returns them as a float
  AssetStream& operator>>( float& x );

  /// @brief extraction operator for ints
  /// pulls 4 bytes out of the AssetStream and returns them as an int
  AssetStream& operator>>( int& x );

  /// @brief extraction operator for bytes
  /// pull a byte out of the AssetStream and return it
  AssetStream& operator>>( uint8_t& x );

  /// @brief extraction operator for chars
  /// pull a byte out of the AssetStream and return it as a char
  AssetStream& operator>>( char& x );

  /// @brief extraction operator for vec3s
  /// pull 12 bytes out of the AssetStream and return them as a Vector3
  AssetStream& operator>>( Vector3& x );

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