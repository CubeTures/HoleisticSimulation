#include "assets/MeshAsset.hpp"

#include <algorithm>
#include <array>
#include <map>
#include <set>
#include <utility>

#include "assets/BVH.hpp"
#include "structures/TriangleIndex.hpp"

void MeshAsset::loadModel() {
    std::map<std::tuple<float, float, float>, int> seenVertices;
    std::set<std::array<int, 3>> seenTriangles;

    auto addVertex = [this, &seenVertices](raylib::Vector3 v) {
        auto [it, inserted] = seenVertices.emplace(
            std::make_tuple(v.x, v.y, v.z), static_cast<int>(positions.size())
        );
        if (inserted) {
            positions.push_back(v);
        }
        return it->second;
    };

    bool hasNormals = true;
    for (size_t i = 0; i < model.GetMeshCount(); i++) {
        hasNormals = hasNormals && model.GetMeshes()[i].normals;
    }

    for (size_t i = 0; i < model.GetMeshCount(); i++) {
        auto m = model.GetMeshes()[i];

        std::vector<int> vIndices;
        std::vector<raylib::Vector3> vNormals;
        for (int j = 0; j < m.triangleCount * 3; j++) {
            int v = m.indices ? m.indices[j] : j;
            vIndices.push_back(addVertex(raylib::Vector3(
                m.vertices[3 * v + 0],
                m.vertices[3 * v + 1],
                m.vertices[3 * v + 2]
            )));
            if (hasNormals) {
                vNormals.push_back(raylib::Vector3(
                    m.normals[3 * v + 0],
                    m.normals[3 * v + 1],
                    m.normals[3 * v + 2]
                ));
            }

            if (vIndices.size() == 3) {
                std::array<int, 3> key = {
                    vIndices[0], vIndices[1], vIndices[2]
                };
                std::sort(key.begin(), key.end());

                bool degenerate = key[0] == key[1] || key[1] == key[2];
                if (!degenerate && seenTriangles.insert(key).second) {
                    triangles.push_back(
                        TriangleIndex{vIndices[0], vIndices[1], vIndices[2]}
                    );
                    if (hasNormals) {
                        normals.push_back(
                            Triangle{vNormals[0], vNormals[1], vNormals[2]}
                        );
                    }
                }
                vIndices.clear();
                vNormals.clear();
            }
        }
    }

    if (!hasNormals) {
        for (const TriangleIndex &tri : triangles) {
            raylib::Vector3 n =
                (positions[tri.j] - positions[tri.i])
                    .CrossProduct(positions[tri.k] - positions[tri.i])
                    .Normalize();
            normals.push_back(Triangle{n, n, n});
        }
    }
}

void MeshAsset::setTraits() {
    int size = positions.size();

    centerOfMass = raylib::Vector3::Zero();
    for (const raylib::Vector3 &p : positions) {
        centerOfMass += p;
    }
    centerOfMass /= static_cast<float>(size);

    unitMoment = Mat3::zero();
    for (int i = 0; i < size; i++) {
        raylib::Vector3 r = positions[i] - centerOfMass;
        unitMoment += Mat3::identity() * r.DotProduct(r) - Mat3::outer(r, r);
    }
    unitMomentInv = unitMoment.inverse();
}
void MeshAsset::buildBVH() {
    bvh = BVH(MeshView{positions, triangles});
}

void MeshAsset::walk(
    const State &state,
    const State &other,
    const MeshAsset &assetOther,
    std::unordered_map<int, std::vector<int>> &out
) const {
    std::vector<BVH::Pair> pairs;
    bvh.narrow(state, other, assetOther.bvh, pairs);

    for (const BVH::Pair &pair : pairs) {
        for (int t : pair.first) {
            const TriangleIndex &tri = triangles[t];
            for (int v : {tri.i, tri.j, tri.k}) {
                std::vector<int> &candidates = out[v];
                candidates.insert(
                    candidates.end(), pair.second.begin(), pair.second.end()
                );
            }
        }
    }
}