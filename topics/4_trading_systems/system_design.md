# Trading System Design
## General Pipeline
> Market Data → Signal Generation → Order Decision → Risk Check → Order Routing → Exchange → Execution → Position Update
## Market Data Feed Handlers
> * Receives list quotes/trades from exchanges (TCP/UDP)
> * Parses packets efficiently
> * Publishes internally for trading logic

## Trading Strategy Engine
> * Implements algorithms: arbitrage, market making
> * Processes market data to generate orders

## Order Manager / Execution System
> * Sends orders to exchange
> * Tracks order state
> * Handles acknowledgements/cancellations

## Risk/Compliance
> * Checks trade limits
> * Prevents catastrophic positions

## Callbacks in C++
> * `std::function` for flexibility
> * Template-based callbacks to eliminate runtime overhead


### 1. Market Data Processing
> Use Case: A "Feed Handler" receives raw UDP packets from an exchange, decodes them into a structured format, and triggers a callback to all subscribed trading strategies.
* Use **CTPR(Curiously Recurring Template Pattern)** or **Compile-time Policy** callbacks to avoid the overhead of virtual function calls or `std::function`.


### Order Lifecycle Management
> Use Case: An Order Management System (OMS) uses callbacks to update risk limits or notify a UI whenever a "Fill" event is received from the FIX engine.
* **Disruptor Pattern** with lock-free callbacks to pass order events between threads with minimal cache misses.