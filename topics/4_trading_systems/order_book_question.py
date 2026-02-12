from sortedcontainers import SortedDict
from collections import deque
from enum import Enum

class Side(Enum):
    BUY = 1
    SELL = 2

class PriceLevel:
    def __init__(self):
        self.orders = deque()  # deque of (order_id, quantity)
        self.total_quantity = 0  # total quantity at this price level

class OrderBook:
    def __init__(self):
        # SortedDict keeps keys sorted automatically
        self.buy_book = SortedDict(lambda x: -x)  # descending order for buy
        self.sell_book = SortedDict()             # ascending order for sell
        self.id_to_order = dict()  # order_id -> (side, price, deque iterator index)

    def add_order(self, order_id: int, side: Side, price: int, quantity: int):
        if side == Side.BUY:
            self._buy_order(order_id, price, quantity)
        else:
            self._sell_order(order_id, price, quantity)

    def get_best_bid(self):
        return next(iter(self.buy_book)) if self.buy_book else -1

    def get_best_ask(self):
        return next(iter(self.sell_book)) if self.sell_book else -1

    def get_order_book_depth(self):
        buy_depth = [(p, lvl.total_quantity) for p, lvl in self.buy_book.items()]
        sell_depth = [(p, lvl.total_quantity) for p, lvl in self.sell_book.items()]
        return {"bids": buy_depth, "asks": sell_depth}

    def cancel_order(self, order_id: int):
        ref = self.id_to_order.pop(order_id, None)
        if not ref:
            return False
        side, price, index = ref
        book = self.buy_book if side == Side.BUY else self.sell_book
        level = book[price]

        qty = level.orders[index][1]
        level.total_quantity -= qty
        # Remove order
        level.orders.remove(level.orders[index])
        if not level.orders:
            del book[price]
        return True

    def _buy_order(self, order_id, price, quantity):
        # Match against lowest sell orders
        while self.sell_book and quantity > 0:
            best_sell = next(iter(self.sell_book))
            if best_sell > price:
                break
            level = self.sell_book[best_sell]
            while level.orders and quantity > 0:
                oid, q = level.orders[0]
                matched = min(q, quantity)
                quantity -= matched
                q -= matched
                level.total_quantity -= matched
                if q == 0:
                    level.orders.popleft()
                    self.id_to_order.pop(oid, None)
                else:
                    level.orders[0] = (oid, q)
            if not level.orders:
                del self.sell_book[best_sell]

        # Add remaining to buy book
        if quantity > 0:
            level = self.buy_book.setdefault(price, PriceLevel())
            level.orders.append((order_id, quantity))
            level.total_quantity += quantity
            self.id_to_order[order_id] = (Side.BUY, price, len(level.orders)-1)

    def _sell_order(self, order_id, price, quantity):
        # Match against highest buy orders
        while self.buy_book and quantity > 0:
            best_buy = next(iter(self.buy_book))
            if best_buy < price:
                break
            level = self.buy_book[best_buy]
            while level.orders and quantity > 0:
                oid, q = level.orders[0]
                matched = min(q, quantity)
                quantity -= matched
                q -= matched
                level.total_quantity -= matched
                if q == 0:
                    level.orders.popleft()
                    self.id_to_order.pop(oid, None)
                else:
                    level.orders[0] = (oid, q)
            if not level.orders:
                del self.buy_book[best_buy]

        # Add remaining to sell book
        if quantity > 0:
            level = self.sell_book.setdefault(price, PriceLevel())
            level.orders.append((order_id, quantity))
            level.total_quantity += quantity
            self.id_to_order[order_id] = (Side.SELL, price, len(level.orders)-1)
