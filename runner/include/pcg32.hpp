#pragma once
#include <bit>
#include <cassert>
#include <chrono>
#include <compare>
#include <cstdint>
#include <limits>
#include <random>
#include <string_view>
#include <thread>
#include <utility>
// PCG32 - Permuted Congruential Generator
// Based on the implementation by Melissa O'Neill (https://www.pcg-random.org)
// C++ implementation by Ulf Benjaminsson (ulfbenjaminsson.com) 2024
//  https://github.com/ulfben/cpp_prngs
// This implementation is placed in the public domain. Use freely.
// Satisfies 'UniformRandomBitGenerator' requirements - compatible with std::shuffle, 
// std::sample, and most std::*_distribution classes.

 #pragma warning(push)
 #pragma warning(disable: 26493 26472) // "Don't use C-style casts". The C-style casts here make the code more readable and match the reference implementation more closely.

struct PCG32{
    using u64 = std::uint64_t;
    using u32 = std::uint32_t;
    using result_type = u32;
    using state_type = u64;
    static constexpr u64 PCG32_DEFAULT_SEED = 0x853c49e6748fea9bULL;
    static constexpr u64 PCG32_DEFAULT_STREAM = 0xda3e39cb94b95bdbULL;
    static constexpr u64 PCG32_MULT = 6364136223846793005ULL;

    constexpr PCG32() noexcept : state(PCG32_DEFAULT_SEED), inc(PCG32_DEFAULT_STREAM){}

    //seed and a sequence selection constant (a.k.a. stream id).
    constexpr PCG32(u64 seed_, u64 seq_ = 1) noexcept{
        seed(seed_, seq_);
    }

    constexpr void seed(u64 seed_, u64 seq_ = 1) noexcept{
        state = 0U;
        inc = (seq_ << 1u) | 1u; //ensure inc is odd
        next();
        state += seed_;
        next();
    }

    constexpr result_type next() noexcept{
        const auto oldstate = state;
        state = oldstate * PCG32_MULT + (inc | 1);
        const auto xorshifted = ((oldstate >> 18u) ^ oldstate) >> 27u;
        const auto rot = oldstate >> 59u;
        return static_cast<u32>((xorshifted >> rot) | (xorshifted << ((~rot + 1) & 31)));
    }
   
    constexpr result_type next(u32 bound) noexcept{
        //highly performant Lemire's algorithm (Debiased Integer Multiplication) after research by Melissa O'Neill
        // https://www.pcg-random.org/posts/bounded-rands.html        
        u64 result = u64{next()} * u64{ bound };
        if(u32 lowbits = static_cast<u32>(result); lowbits < bound){
            u32 threshold = u32{0} - bound;   // Calculate 2^32 - bound for rejection sampling
            if(threshold >= bound){
                threshold -= bound;
                if(threshold >= bound)
                    threshold %= bound;
            }
            while(lowbits < threshold){
                result = u64{next()} * u64{ bound };
                lowbits = static_cast<u32>(result);
            }
        }
        return result >> 32;
    }    

    constexpr bool coinToss() noexcept{
        return next() & 1; //checks the least significant bit
    }

    //generate float in [0, 1)
    constexpr float normalized() noexcept{
        return 0x1.0p-32f * next(); //again, courtesy of Melissa O'Neill. See: https://www.pcg-random.org/posts/bounded-rands.html
        //0x1.0p-32 is a binary floating point constant for 2^-32 that we use to convert a random 
        // integer value in the range [0..2^32) into a float in the unit interval 0-1        
        //A less terrifying, but also less efficient, way to achieve the same goal  is:         
        //    return static_cast<float>(std::ldexp(next(), -32));
        //see https://mumble.net/~campbell/tmp/random_real.c for more info.        
    }

    //generate float in [-1, 1)    
    constexpr float unit_range() noexcept{
        return 2.0f * normalized() - 1.0f;
    }

    template<std::floating_point F>
    constexpr F between(F min, F max) noexcept{
        assert(min < max && "PCG32::between(min, max) called with inverted range.");
        return min + (max - min) * normalized();
    }

    template<std::integral I>
    constexpr I between(I min, I max) noexcept{
        using UI = std::make_unsigned_t<I>;
        static_assert(std::numeric_limits<UI>::max() <= std::numeric_limits<result_type>::max(),
            "PCG32::between() only supports types up to PCG32::result_type in size");
        assert(min < max && "pcg32::between(min, max) called with inverted range.");        
        UI range = static_cast<UI>(max - min);
        return min + static_cast<I>(next(static_cast<result_type>(range)));
    }

        //Based on Brown, "Random Number Generation with Arbitrary Stride,"
        // Transactions of the American Nuclear Society (Nov. 1994)    
    constexpr void advance(u64 delta) noexcept{
        u64 cur_mult = PCG32_MULT;
        u64 cur_plus = inc;
        u64 acc_mult = 1u;
        u64 acc_plus = 0u;
        while(delta > 0){
            if(delta & 1){
                acc_mult *= cur_mult;
                acc_plus = acc_plus * cur_mult + cur_plus;
            }
            cur_plus = (cur_mult + 1) * cur_plus;
            cur_mult *= cur_mult;
            delta /= 2;
        }
        state = acc_mult * state + acc_plus;
    }

    constexpr void backstep(u64 delta) noexcept{
        advance(u32{0} - delta);  // going backwards works due to modular arithmetic
    }

    constexpr void set_state(u64 new_state, u64 new_inc) noexcept{
        state = new_state;
        inc = new_inc | 1u;  // ensure inc is odd
    }
    constexpr std::pair<u64, u64> get_state() const noexcept{
        return {state, inc};
    }
    static constexpr PCG32 from_state(u64 saved_state, u64 saved_inc) noexcept{
        PCG32 rng;
        rng.set_state(saved_state, saved_inc);
        return rng;
    }

    // operators and standard interface
    constexpr result_type operator()() noexcept{
        return next();
    }
    constexpr result_type operator()(u32 bound) noexcept{
        return next(bound);
    }
    static result_type constexpr min() noexcept{
        return 0;
    }
    static result_type constexpr max() noexcept{
        return std::numeric_limits<result_type>::max();
    }
    constexpr void discard(u64 count) noexcept{
        advance(count);
    }
    constexpr auto operator<=>(const PCG32& other) const noexcept = default;
private:
    u64 state{0}; // RNG state.  All values are possible.
    u64 inc{1}; // controls which RNG sequence (stream) is selected. Must *always* be odd.
};

// Utility functions for seeding PRNGs (Pseudo Random Number Generators).
// 
// This namespace provides several approaches to generating high-quality seed values,
// both at compile-time and runtime. Each entropy source has different properties
// that make it suitable for different use cases.
//
// Features:
// - Compile-time seeding via string hashing (FNV1a) and source information
// - SplitMix64 for high-quality mixing of numeric values
// - Runtime entropy from various sources (time, thread ID, hardware, etc.)
// - Utilities for converting from 64-bit to 32-bit seeds
//
// The entropy sources are designed to be composable - you can use them individually
// or combine them for maximum entropy when needed.
// For details, see https://github.com/ulfben/cpp_prngs
namespace seed {
    using u64 = std::uint64_t;
    using u32 = std::uint32_t;

    // SplitMix64 mixing function - fast and high-quality mixing of 64-bit values
    constexpr u64 splitmix64(u64 x) noexcept{
        x += 0x9e3779b97f4a7c15;
        x = (x ^ (x >> 30)) * 0xbf58476d1ce4e5b9;
        x = (x ^ (x >> 27)) * 0x94d049bb133111eb;
        return x ^ (x >> 31);
    }

    // Wall clock time
    // Properties:
    // - High resolution (typically nanoseconds)
    // - Monotonic (always increases)
    // - Reflects real-world time progression
    // - May be synchronized across machines
    // - Useful default! seeds will vary with time
    inline u64 from_time() noexcept{
        const auto now = std::chrono::high_resolution_clock::now().time_since_epoch().count();
        return splitmix64(now);
    }

    // CPU time consumed by the program
    // Properties:
    // - Varies with CPU frequency scaling
    // - Affected by system load and power states
    // - Can differ significantly from wall time in multi-threaded programs
    inline u64 from_cpu_time() noexcept{
        return splitmix64(static_cast<u64>(std::clock()));
    }

    // Stack address
    // Properties:
    // - Varies between runs due to ASLR (Address Space Layout Randomization)
    // - May be predictable if ASLR is disabled
    // - Usually aligned to specific boundaries
    // - Cheap to obtain
    // - Potentially useful as supplementary entropy source
    inline u64 from_stack() noexcept{
        constexpr auto dummy = 0;  // local variable just for its address
        #pragma warning(push)
#pragma warning(disable: 26490) // Don't use reinterpret_cast
        return splitmix64(reinterpret_cast<std::uintptr_t>(&dummy));
#pragma warning(pop)
    }

    // Hardware entropy
    // Properties:
    // - High-quality entropy from system source (when available)
    // - May be slow or limited on some platforms
    // - May fall back to pseudo-random implementation
    // - Best option when available but worth having alternatives
    inline u64 from_entropy(){
        std::random_device rd;
        const auto entropy = (static_cast<u64>(rd()) << 32) | rd();
        return splitmix64(entropy);
    }

    // Combines all available entropy sources
    // Properties:
    // - Maximum entropy mixing
    // - Higher overhead
    // - Useful when seed quality is critical
    // - Good for initializing large-state PRNGs
    inline u64 from_all(){
        const auto time = from_time();
        const auto cpu = from_cpu_time();
        const auto stack = from_stack();
        const auto entropy = from_entropy();
        return splitmix64(time ^ cpu ^ stack ^ entropy);
    }

    constexpr inline u32 to_32(u64 seed) noexcept{
        return static_cast<u32>(seed ^ (seed >> 32)); //XOR-fold to preserve entropy
    }
}

#pragma warning(pop)