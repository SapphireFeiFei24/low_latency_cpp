#pragma once

#include <functional>
namespace callback {
/***
 * std::function (runtime flexibility)
 * Define: function wrapper class which can store and call any function or a callable object
 * Type Erasure: The specific type of the callable is hidden from the public interface. only exposes a generic operator()
 * Performance Cost: It typically involves a virtual function call (indirection) and
 *  may require dynamic memory allocation (heap)
 *  if the captured state of a lambda exceeds a small buffer (usually 16-32 bytes).
 * Use Cases: "slow path", need to change acallback at runtime
 */
    // Taking lambda
    std::function<double(int, int)> calc = [](int a, int b) {return double(a) / b;};

    // Taking class function
    class C {
    public:
        int f(int a, int b) {
            return a + b;
        }

    };
   std::function<int(C&, int, int)> member_func = &C::f;

    // function composition
    std::function<int(int)> compose(std::function<int(int)> f1, std::function<int(int)> f2) {
        // return a lambda function
        return [f1, f2](int x) {
            return f1(f2(x));
        };
    }

    // TODO: use with std::bind

    // combine with void* and use static cast

/***
 * Static Polymorphism via templates or the Curiously Recurring Template Pattern(CRTP)
 * Template Callbacks
 *  Make function a template that accepts any callable type
 *  Pros: know code at compile-time
 *  Limitation: no flexibility
 * CRTP: "Polymorphism at Compile-Time"
 *  The Base class uses static_cast<Derived*>(this) to call methods in the Derived class.
*/
    template <typename Derived>
    class TradingBase {
    public:
        void processTick() {
            // "this" is actually a pointer to the derived class
            static_cast<Derived*>(this)->onTick();
        }
    };

    class NasdaqHandler: public TradingBase<NasdaqHandler> {
    public:
        void onTick() {
            // acutal implementation
        }
    };

    // use derived class
    NasdaqHandler nasdaq_handler;

    // achieve compile time polymorphism
    template <typename Strategy>
    void run_engine();

    if (config.exchange == "NASDAQ") {
        run_engine<NasdaqHandler>();
    }
}