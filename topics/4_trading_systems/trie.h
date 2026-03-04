#pragma once
namespace trie {
    class Trie {
        struct Node {
            std::array<std::shared_ptr<Node>, 26> children;
            int is_leaf;
            Node(): is_leaf(false){};
        };

        std::shared_ptr<Node> root;

        int index(char c) {
            return c -  'a';
        }
    public:
        Trie() {
            root = make_shared<Node>();
        }

        void insert(string word) {
            auto p = root;
            int idx = 0;
            int N = word.size();
            while (idx < N) {
                char c = word[idx];
                if (p->children[index(c)] == nullptr) {
                    p->children[index(c)] = make_shared<Node>();
                }
                p = p->children[index(c)];
                ++idx;
            }
            p->is_leaf = true;
        }

        bool search(string word) {
            auto p = root;
            int idx = 0;
            int N = word.size();
            while (idx < N) {
                char c = word[idx];
                if (p->children[index(c)] == nullptr) {
                    return false;
                }
                p = p->children[index(c)];
                ++idx;
            }
            return p->is_leaf;
        }

        bool startsWith(string prefix) {
            auto p = root;
            int idx = 0;
            int N = prefix.size();
            while (idx < N) {
                char c = prefix[idx];
                if (p->children[index(c)] == nullptr) {
                    return false;
                }
                p = p->children[index(c)];
                ++idx;
            }
            return true;
        }
    };

    /**
     * Your Trie object will be instantiated and called as such:
     * Trie* obj = new Trie();
     * obj->insert(word);
     * bool param_2 = obj->search(word);
     * bool param_3 = obj->startsWith(prefix);
     */
}