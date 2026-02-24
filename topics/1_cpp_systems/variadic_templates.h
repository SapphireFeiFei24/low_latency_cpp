#pragma once
#include <tuple>
namespace variadic_templates {
    namespace fold_expression {
        namespace reverse {
            /**
            *write a struct ‘Reverse’ which can reverse the template arguments of any std::tuple so that
            *static_assert(std::is_same_v<typename Reverse<std::tuple<int, bool, double>>::Type, std::tuple<double, bool, int>>
             **/
            // Primary template: This is needed before any partial specializations
            template <typename T>
            struct Reverse;

            template <> // edge case: recursive must stop
            struct Reverse<std::tuple<>> {
                using Type = std::tuple<>;
            };
            template <typename FirstType, typename ... Types>
            struct Reverse<std::tuple<FirstType, Types...>> {
                using Type = decltype(std::tuple_cat(
                    std::declval<typename Reverse<std::tuple<Types...>>::Type>(),
                    std::declval<std::tuple<FirstType>>()));
            };

        }
        // define binary tree structure and traverse helpers:
        struct Node {
            int value;
            Node* left;
            Node* right;
            Node(int i=0) : value(i), left(nullptr), right(nullptr) {
            }
        };
        // decltype(left) // Node* Node::*
        auto left = &Node::left; // pointer to a class member where the type of the member is Node*
        auto right = &Node::right;

        // traverse tree, using fold expression:
        template<typename T, typename... TP>
        Node* traverse (T np, TP... paths) {
            return (np ->* ... ->* paths); // np->*path1->*path2->...
        }

        // traverse
        Node* node = traverse(root, left, right);
    }

}