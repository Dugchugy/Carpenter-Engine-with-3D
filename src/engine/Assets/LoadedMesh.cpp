#include "LoadedMesh.hpp"

#include "AssetStream.hpp"
#include "LoadObjHelpers.hpp"

#include <iostream>

using namespace Engine::Assets;

LoadedMesh::LoadedMesh( std::vector<Tri> triangles ) {
  for ( int i = 0; i < triangles.size(); i++ ) {
    AddTriangle( triangles[i].v1, triangles[i].v2, triangles[i].v3 );
    AddTriangle( triangles[i].v3, triangles[i].v2, triangles[i].v1 );
  }
}

LoadedMesh Engine::Assets::loadStlMesh( std::string filename ) {

  AssetStream stlAsset( filename );
  stlAsset.open();

  // skips the 80 byte header
  stlAsset.skipBytes( 80 );

  int triCount = 0;
  stlAsset >> triCount;

  std::vector<Tri> triangles;

  for ( int i = 0; i < triCount; i++ ) {
    Vec3f normal, v1, v2, v3;

    stlAsset >> normal >> v1 >> v2 >> v3;

    Vec3f calcNorm = cross( v2-v1, v3-v1 );

    normal.normalize();
    calcNorm.normalize();

    if( ( normal - calcNorm ).lengthSquared() < 0.001f ) {
      triangles.push_back( { { v1.x, v1.y, v1.x, 0, 0 },
                             { v2.x, v2.y, v2.z, 1, 0 },
                             { v3.x, v3.y, v3.x, 1, 1 } } );
    } else {
      triangles.push_back( { { v3.x, v3.y, v3.x, 1, 1 },
                             { v2.x, v2.y, v2.z, 1, 0 },
                             { v1.x, v1.y, v1.x, 0, 0 } } );
    }

    // skip two byte mesh number
    stlAsset.skipBytes( 2 );
  }

  std::cout << "loaded: " << triangles.size() << " triangles\n";

  return LoadedMesh( triangles );


}