#include "test_runner.h"
#include <cassert>
#include <deque>
#include <iostream>

using namespace std;


struct Node {
    Node(int v, Node* p)
        : value(v)
        , parent(p)
    {}

    int value;
    Node* left = nullptr;
    Node* right = nullptr;
    Node* parent;
};


// class NodeBuilder {
// public:
//     Node* CreateRoot(int value) {
        // nodes.emplace_back(value, nullptr);
//         return &nodes.back();
//     }

//     Node* CreateLeftSon(Node* me, int value) {
//         assert( me->left == nullptr );
//         nodes.emplace_back(value, me);
//         me->left = &nodes.back();
//         return me->left;
//     }

//     Node* CreateRightSon(Node* me, int value) {
//         assert( me->right == nullptr );
//         nodes.emplace_back(value, me);
//         me->right = &nodes.back();
//         return me->right;
//     }

// private:
//     deque<Node> nodes;
// };


Node* Next(Node* me) {
    Node* res;
    
    if (me->right) {
        res = me->right;
        while (res->left) {
            res = res->left;
        }
        return res;
    }
    res = me;
    while (res->parent) {
        if (res->parent->value > me->value) {
            return res->parent;
        }
        res = res->parent;
    }
    return nullptr;
}
