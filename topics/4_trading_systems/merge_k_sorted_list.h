/**
* Definition for singly-linked list.
 * struct ListNode {
 *     int val;
 *     ListNode *next;
 *     ListNode() : val(0), next(nullptr) {}
 *     ListNode(int x) : val(x), next(nullptr) {}
 *     ListNode(int x, ListNode *next) : val(x), next(next) {}
 * };
 */
class Solution {

    struct Comparator {
        bool operator()(const ListNode* a, const ListNode* b) const {
            return a->val > b->val;
        }
    };

    bool comparator(const ListNode* a, const ListNode* b) const {
        return a->val > b->val;
    }
public:
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        std::priority_queue<ListNode*, std::vector<ListNode*>, Comparator> queue;
        // OR, std::priority_queue<ListNode*, std::vector<ListNode*>, decltype(comparator)> queue;
        for (const auto& list: lists) {
            if (list) {
                queue.push(list);
            }

        }

        ListNode* head = new ListNode(0); // wrong
        ListNode* p = head;

        ListNode head(0);
        ListNode* p = &head;
        while (!queue.empty()) {
            auto node = queue.top();
            // printf("curr node:%d p is nullptr:%s\n", node->val, p?"false":"true");
            p->next = node;
            p = p->next;
            queue.pop();
            if (node->next != nullptr) {
                queue.push(node->next);
            }

        }
        return head->next;
    }
};