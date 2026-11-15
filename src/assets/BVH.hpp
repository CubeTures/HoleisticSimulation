#pragma once

#include <algorithm>
#include <memory>
#include <numeric>

#include "assets/Bounds.hpp"
#include "structures/MeshView.hpp"
#include "structures/State.hpp"

constexpr int MAX_LEAF_SIZE = 4;

class BVH {
  public:
    struct Node {
        std::unique_ptr<Node> left;
        std::unique_ptr<Node> right;
        Bounds bounds;
        std::vector<int> tris;

        bool isLeaf() const {
            return tris.size() != 0;
        }
    };

    using Pair = std::pair<const std::vector<int> &, const std::vector<int> &>;

  protected:
    std::unique_ptr<Node> tree;

  private:
    std::unique_ptr<Node> buildTree(
        const MeshView &mesh,
        std::vector<int> &tris,
        int start,
        int end
    ) {
        Bounds bounds;
        AABB centroids;

        for (int i = start; i < end; i++) {
            const TriangleIndex &tri = mesh.triangles[tris[i]];
            bounds.grow(mesh, tri);
            centroids.grow(centroid(mesh, tri));
        }

        int axis = centroids.longestAxis();
        if (end - start <= MAX_LEAF_SIZE || centroids.flatAlong(axis)) {
            return std::make_unique<Node>(
                nullptr,
                nullptr,
                bounds,
                std::vector<int>(tris.begin() + start, tris.begin() + end)
            );
        }

        int mid = partition(mesh, tris, start, end, axis);
        std::unique_ptr<Node> left = buildTree(mesh, tris, start, mid);
        std::unique_ptr<Node> right = buildTree(mesh, tris, mid, end);
        return std::make_unique<Node>(
            std::move(left), std::move(right), bounds
        );
    }

    int partition(
        const MeshView &mesh,
        std::vector<int> &tris,
        int start,
        int end,
        int axis
    ) {
        int mid = (start + end) / 2;
        std::nth_element(
            tris.begin() + start,
            tris.begin() + mid,
            tris.begin() + end,
            [&mesh, axis](int a, int b) {
                return centroid(mesh, mesh.triangles[a], axis) <
                       centroid(mesh, mesh.triangles[b], axis);
            }
        );
        return mid;
    }

    static raylib::Vector3 centroid(const MeshView &mesh, TriangleIndex tri) {
        return (mesh.positions[tri.i] + mesh.positions[tri.j] +
                mesh.positions[tri.k]) /
               3.0f;
    };

    static float centroid(const MeshView &mesh, TriangleIndex tri, int axis) {
        raylib::Vector3 center = centroid(mesh, tri);
        switch (axis) {
        case 0:
            return center.x;
        case 1:
            return center.y;
        default:
            return center.z;
        }
    };

    void narrow(
        const State &state,
        const Node &node,
        const State &other,
        const Node &nodeOther,
        std::vector<Pair> &results
    ) const {
        if (!node.bounds.overlap(state, other, nodeOther.bounds)) {
            return;
        }

        if (node.isLeaf() && nodeOther.isLeaf()) {
            results.push_back(Pair{node.tris, nodeOther.tris});
            return;
        }

        if (descendThis(node, nodeOther)) {
            narrow(state, *node.left, other, nodeOther, results);
            narrow(state, *node.right, other, nodeOther, results);
        } else {
            narrow(state, node, other, *nodeOther.left, results);
            narrow(state, node, other, *nodeOther.right, results);
        }
    }

    bool descendThis(const Node &node, const Node &nodeOther) const {
        if (node.isLeaf()) {
            return false;
        } else if (nodeOther.isLeaf()) {
            return true;
        } else {
            return node.bounds.volume() >= nodeOther.bounds.volume();
        }
    }

  public:
    BVH() : tree(nullptr) {
    }

    BVH(const MeshView &mesh) {
        std::vector<int> tris(mesh.triangles.size());
        std::iota(tris.begin(), tris.end(), 0);
        tree = buildTree(mesh, tris, 0, static_cast<int>(tris.size()));
    }

    bool broad(const State &state, const State &other, const BVH &bvhOther)
        const {
        return tree->bounds.overlap(state, other, bvhOther.tree->bounds);
    }

    void narrow(
        const State &state,
        const State &other,
        const BVH &bvhOther,
        std::vector<Pair> &results
    ) const {
        narrow(state, *tree, other, *bvhOther.tree, results);
    }
};
