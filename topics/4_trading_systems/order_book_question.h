/***
 * Coding Question: Order Book Matching Engine
 * Order: (order_id: int, side: str, price: int, quantity: int)
 * side="BUY" or "SELL"
 * Rules:
 *  Higher BUY price has priority
 *  Lower SELL price has priority
 *  If prices are equal, earlier order gets priority (FIFO)
 * Matching Rules
 *  When a new order arrives:
 *  If BUY: Match against lowest SELL orders. Only match if sell_price <= buy_price
 *  If SELL: Match against highest BUY orders. Only match if buy_price >= sell_price
 * Matching Behavior
 *  Partial fills allowed
 *  If quantity not fully filled → remaining goes into book
 *  Remove orders when quantity becomes 0
 * Constrains
 *  Up to 200k orders
 *  Must be efficient
 *  Can't sort entire book everytime -> Sorted structure
 *  Must support partial fills efficiently
 * Structure Selection
 *  In C++: map<price, queue_of_order>, map is sorted by key. for low latency, use customized skip list and flatten tree
 *  In Python: no balanced tree. heapq for price, dict<price, dequeue<order>> for orders
***/
#pragma once
#include <string>
#include <map>
#include <vector>
#include <deque>
#include <algorithm>

enum class Side {
    Buy,
    Sell
};


class OrderBook {
public:
    typedef std::pair<int, int> Order; // id, quantity
    typedef std::vector<Order> Orders;
    typedef std::deque<Order> OrdersQueue;
    struct OrderRef {
        Side side;
        int price;
        std::deque<Order>::iterator iter;
    };
    struct PriceLevel {
        std::deque<Order> orders;
        int total_quantity; // avoid recomputing every time for get_order_book_depth
    };
    OrderBook() = default;
    void add_order(int order_id, Side side, int price, int quantity) {
        // Try to match the order, update the order book if matched, append remaining to the book
        if (side == Side::Buy) {
            return buy_order(order_id, price, quantity);
        }
        if (side == Side::Sell) {
            return sell_order(order_id, price, quantity);
        }
        // TODO: handle exception
        return;
    }

    int get_best_bid() const {
        // Return the highest buy order
        if (buy_book.empty()) {
            return -1;
        }
        return buy_book.begin()->first;
    }
    int get_best_ask() const {
        // return the lowest sell order
        if (sell_book.empty()) {
            return -1;
        }
        return sell_book.begin()->first;
    }

    std::map<std::string, Orders> get_order_book_depth() const {
        // Return map of {"bids": Orders, "sell": Orders}, Orders sorted.
        std::map<std::string, Orders> order_book;
        Orders buy_orders, sell_order;
        buy_orders.reserve(buy_book.size());
        for (const auto& data: buy_book) {

            buy_orders.emplace_back(std::make_pair<int, int>(data.first, data.second.total_quantity));
        }
        sell_order.reserve(sell_book.size());
        for (const auto& data: sell_book) {
            sell_order.emplace_back(std::make_pair<int, int>(data.first, data.second.total_quantity));
        }
        order_book["bids"] = std::move(buy_orders);
        order_book["asks"] = std::move(sell_order);
        return order_book; // no need std::move, RVO(return value optimization) will handle it automatically
    }

    bool cancel_order(int order_id) {
        auto iter = id_to_order.find(order_id);
        if (iter == id_to_order.end()) {
            return false;
        }
        auto& ref = iter->second;
        auto& level = (ref.side == Side::Buy ? buy_book[ref.price] : sell_book[ref.price]);
        level.total_quantity -= ref.iter->second;
        level.orders.erase(ref.iter);

        if (level.orders.empty()) {
            if (ref.side == Side::Buy) buy_book.erase(ref.price);
            else sell_book.erase(ref.price);
        }

        id_to_order.erase(iter);
        return true;
    }
private:
    void buy_order(int order_id, int price, int quantity) {
        // Try matching with existing sell orders
        while (!sell_book.empty() && quantity) {
            // loop by price
            if (sell_book.begin()->first > price) {
                // break if min sell price is too high
                break;
            }
            auto first_ele_iter = sell_book.begin();
            while (!first_ele_iter->second.orders.empty() && quantity) {
                // loop by order
                int matched = std::min(first_ele_iter->second.orders.front().second, quantity);
                first_ele_iter->second.orders.front().second -= matched;
                first_ele_iter->second.total_quantity -= matched;
                quantity -= matched;
                if (first_ele_iter->second.orders.front().second == 0) {
                    // Sell order fully filled
                    first_ele_iter->second.orders.pop_front();
                }
            }
            // No more orders at this price
            if (sell_book.begin()->second.orders.empty()) {
                sell_book.erase(sell_book.begin());
            }
        }
        // Append to buy book if remaining
        if (quantity != 0) {
            auto& level = buy_book[price]; // direct creating if not found, avoid double lookup
            level.orders.emplace_back(order_id, quantity);
            level.total_quantity += quantity;

            // track iterator
            OrderRef ref {
                .side = Side::Buy,
                .price = price,
                .iter = std::prev(level.orders.end()),
            };
            id_to_order[order_id] = ref;
        }
    }

    void sell_order(int order_id, int price, int quantity) {
        // Try matching with existing sell orders
        while (!buy_book.empty() && quantity) {
            // loop by price
            if (buy_book.begin()->first < price) {
                // break if max buy price is too low
                break;
            }
            auto first_ele_iter = buy_book.begin();
            while (!first_ele_iter->second.orders.empty() && quantity) {
                // loop by order
                int matched = std::min(first_ele_iter->second.orders.front().second, quantity);
                first_ele_iter->second.orders.front().second -= matched;
                first_ele_iter->second.total_quantity -= matched;
                quantity -= matched;
                if (first_ele_iter->second.orders.front().second == 0) {
                    // Buy order fully filled
                    first_ele_iter->second.orders.pop_front();
                }
            }
            // No more orders at this price
            if (buy_book.begin()->second.orders.empty()) {
                buy_book.erase(buy_book.begin());
            }
        }
        // Append to sell book if remaining
        if (quantity != 0) {
            auto& level = sell_book[price]; // direct creating if not found, avoid double lookup
            level.orders.emplace_back(order_id, quantity);
            level.total_quantity += quantity;
            // track iterator
            OrderRef ref {
                .side = Side::Sell,
                .price = price,
                .iter = std::prev(level.orders.end()),
            };
            id_to_order[order_id] = ref;
        }

    }
    std::map<int, PriceLevel, std::greater<int>> buy_book; // price -> buy_queue
    std::map<int, PriceLevel> sell_book; // price -> sell_queue

    // Support order cancellation
    std::map<int, OrderRef>  id_to_order; // order_id, order_iterator
};
