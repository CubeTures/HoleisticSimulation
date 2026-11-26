#pragma once

#include <vector>

#include "Model.hpp"

#include "assets/BVH.hpp"
#include "structures/Mat3.hpp"
#include "structures/Triangle.hpp"
#include "structures/TriangleIndex.hpp"

class MeshAsset {
  private:
    void loadModel();
    void setTraits();
    void buildBVH();

  public:
    raylib::Model model;

    std::vector<raylib::Vector3> positions;
    std::vector<TriangleIndex> triangles;
    std::vector<Triangle> normals;

    raylib::Vector3 centerOfMass;
    Mat3 unitInertia;

    BVH bvh;

    MeshAsset(std::string filename) : model(RES_DIR + filename) {
        loadModel();
        setTraits();
        buildBVH();
    }
};
