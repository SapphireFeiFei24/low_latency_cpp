#pragma once
#include <map>
#include <math.h>
#include <queue>
#include <stdexcept>
#include <string>
#include <unordered_map>
/**
 * HFT Online Assessment
 * Requirements:
 * Input is a stream orders.
 * Each order format is "StockId,action,price,quantity StockId,action,price,quantity"
 *  - StockId: stock to trade, i.e. "QQQ", "VOO"
 *  - action: BUY,SELL,ASK,BID
 *    - BUY and SELL: action taken by you
 *    - ASK: action taken by others that is willing to sell at no less than price for quantity amount
 *    - BID: action taken by others that is willing to buy at no more than price for quantity amount
 *  - When some order comes in, try to match with the best existing order by price, any leftover quantity will be stored in to the order book
 *  Output: 4 types of p&L(achieved, and leftover)
 *   - buy_pnl — when your BUY matches an ASK: sum(qty × (your_buy_price - ask_price)) for each match
 *   - sell_pnl — when your SELL matches a BID: sum(qty × (bid_price - your_sell_price)) for each match
 *   - open_buy_notional — leftover: sum(remaining_qty × price) for all unmatched BUY orders
 *   - open_sell_notional — leftover: sum(remaining_qty × price) for all unmatched SELL orders
 */
enum class Action {
    BUY,
    SELL,
    BID,
    ASK
};

struct OrderInfo {
    std::string id;
    Action action;
    int quantity;
    double price; // nenver use float for financial calculations - precision loss compounds badly

    bool operator<(const OrderInfo& o) const {
        return price < o.price;
    }
};

class MatchingEngine {
    // Using integer cent to avoid mismatching due to precision
    using SellBook = std::map<int, std::queue<OrderInfo>>; // ascending order
    using BuyBook = std::map<int, std::queue<OrderInfo>, std::greater<double>>; // descending order

    std::unordered_map<std::string, BuyBook> buy_book; // One order book per instrument
    std::unordered_map<std::string, SellBook> sell_book; // One order book per instrument

    static Action get_action(const std::string_view str) {
        if (str == "BUY") {
            return Action::BUY;
        }
        if (str == "SELL") {
            return Action::SELL;
        }
        if (str == "BID") {
            return Action::BID;
        }
        if (str == "ASK") {
            return Action::ASK;
        }
        throw std::invalid_argument("invalid order book action");
    }

    static bool is_self_action(Action action) {
        switch (action) {
            case Action::BUY:
                return true;
            case Action::SELL:
                return true;
            case Action::BID:
                return false;
            case Action::ASK:
                return false;
            default:
                throw std::invalid_argument("Invalid action");
        }
    }

    OrderInfo process_order_info(const std::string_view& data_stream) {
        OrderInfo order;
        size_t idx = 0;

        auto order_id = data_stream.substr(idx, data_stream.find(',', idx) - idx);
        order.id = std::string(order_id);
        idx += order_id.size() + 1;

        auto action = data_stream.substr(idx, data_stream.find(',', idx) - idx);
        order.action = get_action(action);
        idx += action.size() + 1;

        auto price = data_stream.substr(idx, data_stream.find(',', idx) - idx);
        order.price = std::stod(std::string(price));
        idx += price.size() + 1;

        auto quantity = data_stream.substr(idx, data_stream.find(',', idx) - idx - idx);
        order.quantity = std::stoi(std::string(quantity));
        return order;
    }

    std::vector<OrderInfo> process_str_stream(const std::string_view& data_stream) {
        size_t idx = 0;
        std::vector<OrderInfo> orders;
        orders.reserve(std::count(data_stream.begin(), data_stream.end(), ' ') + 1);
        while (idx < data_stream.size()) {
            auto sub_data = data_stream.substr(idx, data_stream.find(' ', idx) - idx);
            orders.emplace_back(process_order_info(sub_data));
            idx += sub_data.size() + 1;
        }
        return orders;
    }

    void submit_order(const OrderInfo& order, MatchingResult& result);

public:
    struct MatchingResult {
        double buy_pnl = 0.0;
        double sell_pnl = 0.0;
        double open_buy_notional = 0.0;
        double open_sell_notional = 0.0;
    };

    /**
     * @brief Entry point of OrderBook
     * @param data_stream "StockId,action,price,quantity StockId,action,price,quantity"
     * @return
     */
    MatchingResult ProcessOrder(const std::string& data_stream) {
        std::vector<OrderInfo> orders = process_str_stream(data_stream);

        MatchingResult result;
        // process every order, updating result
        for (const auto& order : orders) {
            submit_order(order, result);
        }

        return result;
    }
};

void MatchingEngine::submit_order(const OrderInfo& order, MatchingResult& result) {

}
