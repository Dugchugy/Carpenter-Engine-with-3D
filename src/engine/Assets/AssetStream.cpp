#include "AssetStream.hpp"
#include <cstring>
#include <stdlib.h>
#include <iostream>

using namespace Engine::Assets;

AssetStream::AssetStream( std::string path ) {
  _asset = nullptr;
  _path = path;
  _size = 0;
  _position = 0;
}

AssetStream::~AssetStream() {
  if ( _asset != nullptr ) {
    free( _asset );
    _asset = nullptr;
  }
}

AssetStream::AssetStream( const AssetStream& a ) {
  if ( _asset != nullptr ) {
    free( _asset );
    _asset = nullptr;
  }

  _asset = (uint8_t *) malloc( a._size );

  memcpy( &_asset, &a._asset, a._size );

  _path = a._path;
  _size = a._size;
  _position = a._position;
}

/*const &AssetStream AssetStream::operator= ( const &AssetStream a ) {
  cpy = AssetStream( a );

  unit8_t* tmp = cpy._asset;
  cpy._asset = _asset;
  _asset = tmp;

  _path = a._path;
  _size = a._size;
  _position = a._position;

  return a;
}*/

void* AssetStream::takeAsset() {
  if ( _asset == nullptr ) {
    // throw error
    std::cout << "thowing NotOpenedException from line 54\n";
    throw NotOpenedException();
  }

  void* tmp = _asset;
  _asset = nullptr;
  _size = 0;
  return tmp;
}

void AssetStream::open() {
  int fileError= 0;

  if ( _asset != nullptr ) {
    free( _asset );
    _asset = nullptr;
  }

  _position = 0;

  emscripten_wget_data( _path.c_str(), ( (void**) &_asset) , &_size, &fileError );

  if ( fileError ) {
    // throw error
    throw AccessException( fileError );
  }
}

void AssetStream::skipBytes( const int& bytes ) {
  if ( _asset == nullptr ) {
    std::cout << "thowing NotOpenedException from line 84\n";
    throw NotOpenedException();
  }

  if ( _size - _position < bytes ) {
    std::cout << "thowing EndOfFileException from line 89\n";
    throw EndOfFileException();
  }

  _position += bytes;
}

std::string AssetStream::readUntil( char end ) {
  
  std::string result = "";

  char c;
  *this >> c;

  while ( ( c != end ) && ( _position < ( _size - 1 ) ) ) {
    result = result + c;
    *this >> c;
  }

  return result;
}

AssetStream& AssetStream::operator>>( float& x ) {
  if ( _asset == nullptr ) {
    std::cout << "thowing NotOpenedException from line 113\n";
    throw NotOpenedException();
  }

  if ( _size - _position < 4 ) {
    std::cout << "thowing EndOfFileException from line 118\n";
    throw EndOfFileException();
  }

  void* current = _asset + _position;
  x = *( (float*) current);

  _position += 4;

  return *this;
}

AssetStream& AssetStream::operator>>( int& x ) {
  if ( _asset == nullptr ) {
    std::cout << "thowing NotOpenedException from line 132\n";
    throw NotOpenedException();
  }

  if ( _size - _position < 4 ) {
    std::cout << "thowing EndOfFileException from line 137\n";
    throw EndOfFileException();
  }

  void* current = _asset + _position;
  x = *( (int*) current);

  _position += 4;

  return *this;
}

AssetStream& AssetStream::operator>>( char& x ) {
  if ( _asset == nullptr ) {
    std::cout << "thowing NotOpenedException from line 151\n";
    throw NotOpenedException();
  }

  if ( _size - _position < 1 ) {
    std::cout << "thowing EndOfFileException from line 156\n";
    throw EndOfFileException();
  }

  void* current = _asset + _position;
  x = *( (char*) current);

  _position += 1;

  return *this;
}

AssetStream& AssetStream::operator>>( uint8_t& x ) {
  if ( _asset == nullptr ) {
    std::cout << "thowing NotOpenedException from line 170\n";
    throw NotOpenedException();
  }

  if ( _size - _position < 1 ) {
    std::cout << "thowing EndOfFileException from line 175\n";
    throw EndOfFileException();
  }

  void* current = _asset + _position;
  x = *( (uint8_t*) current);

  _position += 1;

  return *this;
}

AssetStream& AssetStream::operator>>( Vec3f& x ) {
  float f1, f2, f3;
  *this >> f1 >> f2 >> f3;

  x.xIs( f1 );
  x.yIs( f2 );
  x.zIs( f3 );

  return *this;
}

AssetStream& AssetStream::operator>>( std:: string& x ) {
  x = readUntil( '\n' );

  return *this;
}

const char* AccessException::what() const throw() {
  return "Failed to access asset file";
}

const char* NotOpenedException::what() const throw() {
  return "Failed to stream asset, file has not been opened";
}

const char* EndOfFileException::what() const throw() {
  return "Failed to stream asset, end of file reached";
}