/*
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at https://mozilla.org/MPL/2.0/.
 */

#include "Texture.hpp"
#include <GLES3/gl3.h>
#include <emscripten.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

Engine::Graphics::Texture::Texture(const char* path) {
  m_filename = path;
}

void Engine::Graphics::Texture::LoadTexture() {
  if ( textureData == nullptr ) {
    LoadTextureArray();
  }
  
  std::cout << (short) textureData[0] << " " << (short) textureData[1] << " " <<
               (short) textureData[2] << " " << (short) textureData[3] << "\n";
  setTextureFromSource( textureData, m_dimensions[ 0 ], m_dimensions[ 1 ] );

  if ( isSTBI ) {
    STBI_FREE( textureData );
  } else {
    free( textureData );
  }
}

void Engine::Graphics::Texture::setTextureFromSource( unsigned char* data, int width, int height ) {
  glGenTextures(1, &m_texture);
  glBindTexture(GL_TEXTURE_2D, m_texture);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
    GL_RGBA, GL_UNSIGNED_BYTE, data);
  glGenerateMipmap(GL_TEXTURE_2D);
}

void Engine::Graphics::Texture::LoadTextureArray() {
  unsigned char* data;
  int size;

  emscripten_wget_data(m_filename, (void**)&data, &size, nullptr);

  while (data == nullptr);

  // Load Image
  textureData = stbi_load_from_memory(data, size, &m_dimensions[0],
    &m_dimensions[1], nullptr, 4);

  free(data);

  if (textureData == nullptr) {
    std::cerr << "ERROR: Failed to load image name: " << m_filename << std::endl;
    return;
  }
}

unsigned Engine::Graphics::Texture::GetTexture() {
  if (m_texture == -1)
    LoadTexture();

  return m_texture;
}

Engine::Graphics::ColorTexture::ColorTexture( Color c ) :
    Texture( "Color" ), color( c ) {
  unsigned int imageSize = 1024;

  textureData = (unsigned char*) malloc( ( imageSize * imageSize ) * 4 );

  for ( int i = 0; i < ( imageSize * imageSize ); i++ ) {
    textureData[ 4*i ] = color.r;
    textureData[ 4*i + 1 ] = color.g;
    textureData[ 4*i + 2 ] = color.b;
    textureData[ 4*i + 3 ] = color.a;
  }

  isSTBI = false;

  m_dimensions[ 0 ] = imageSize;
  m_dimensions[ 1 ] = imageSize;
}

void Engine::Graphics::ColorTexture::LoadTextureArray() {}