#pragma once
#include <tuple>
namespace variadic_templates {
    namespace fold_expression {
        namespace reverse {
            /**
            *write a struct ‘Reverse’ which can reverse the template arguments of any std::tuple so that
            *static_assert(std::is_same_v<typename Reverse<std::tuple<int, bool, double>>::Type, std::tuple<double, bool, int>>
             **/
            // Helper template
            template <typename T, typename S>
            struct Join;

            template <typename... Ts, typename... Ss>
            struct Join<std::tuple<Ts...>, std::tuple<Ss...>> {
                using Type = std::tuple<Ts..., Ss...>;
            };

            // Primary template: This is needed before any partial specializations
            template <typename T>
            struct Reverse;

            template <> // edge case: recursive must stop
            struct Reverse<std::tuple<>> {
                using Type = std::tuple<>;
            };

            template <typename FirstType, typename... Rest>
            struct Reverse<std::tuple<FirstType, Rest...>> {
                using Type = typename Join<typename Reverse<std::tuple<Rest...>>::Type, std::tuple<FirstType>>::Type;
            };

            // not ideal: constructing a real obj, it's run time
            // template <typename FirstType, typename ... Types>
            // struct Reverse<std::tuple<FirstType, Types...>> {
            //     using Type = decltype(std::tuple_cat(
            //         std::declval<typename Reverse<std::tuple<Types...>>::Type>(),
            //         std::declval<std::tuple<FirstType>>()));
            // };

        }
        namespace tree_iteration {
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
            auto right = &Node::right; // acts like an offset where spefic filed lives relative to the start of any instean of that class

            // traverse tree, using fold expression:
            template<typename T, typename... TP>
            Node* traverse (T np, TP... paths) {
                return (np ->* ... ->* paths); // np->*path1->*path2->...
            }

            // traverse
            Node* node = traverse(root, left, right);
        }

    }

    namespace tuple_contains {
        /***
         * Write a metafunction that returns true at compile-time if a specific type T exists within a std::tuple.
         */
        template <typename T, typename Tuple>
        struct TupleContains: std::false_type {};

        template <typename T, typename... TP>
        struct TupleContains<T, std::tuple<T,TP...>>: std::true_type{};

        template <typename T, typename S, typename... TP>
        struct TupleContains<T, std::tuple<S, TP...>>: TupleContains<T, TP...>{};

    }
    namespace tuple_size {
        template <typename T>
        struct TupleSize;

        template <typename... TP>
        struct TupleSize<std::tuple<TP...>> {
            static constexpr std::size_t value = sizeof...(TP); // key word to ensure compile time constant
            // if auto value = sizeof...(TP), you have to instantiate a TupleSize obj to get the value
        };
    }

    namespace first_type {
        template <typename T>
        struct FirstType;

        template <typename T, typename... Types>
        struct FirstType<std::tuple<T, Types...>> {
            using type = T; // using is compile time
        };
    }

    namespace pop_front {
        template <typename T>
        struct PopFront;
        template <typename T, typename... Types>
        struct PopFront<std::tuple<T, Types...>> {
            using Type = std::tuple<Types...>;
        };
    }

    namespace concat {
        template <typename T1, typename T2>
        struct Concat;

        template <typename... T1, typename... T2>
        struct Concat<std::tuple<T1...>, std::tuple<T2...>> {
          using Type = std::tuple<T1..., T2...>;
        };
    }

    namespace index {
        template <typename T, typename Types> // specialize the same number with how you use it
        struct Index;

        template <typename T, typename... Types>
        struct Index<T, std::tuple<T, Types...>> {
            static constexpr int value = 0;
        };

        template <typename T, typename S, typename... Types>
        struct Index<T, std::tuple<S, Types...>> {
            static constexpr int value = 1 + Index<T, std::tuple<Types...>>::value;
        };
    }

    namespace transform {
        /***
         * Apply meta function to all types
         */
        template <typename Tuple, template<typename> class F>
        struct Transform;

        template <typename T, typename... Types, template<typename> class F>
        struct Transform<std::tuple<T, Types...>, F> {
            using Type = std::tuple<typename F<Types>::Type...>;
        };
    }
}