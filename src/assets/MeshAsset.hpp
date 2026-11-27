#pragma once

#include <unordered_map>
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
    Mat3 unitMoment, unitMomentInv;

    BVH bvh;

    MeshAsset(std::string filename) : model(RES_DIR + filename) {
        loadModel();
        setTraits();
        buildBVH();
    }

    Triangle triangle(int t) const {
        const TriangleIndex &tri = triangles[t];
        return Triangle{
            positions[tri.i],
            positions[tri.j],
            positions[tri.k],
        };
    }

    void walk(
        const State &state,
        const State &other,
        const MeshAsset &assetOther,
        std::unordered_map<int, std::vector<int>> &out
    ) const;
};
