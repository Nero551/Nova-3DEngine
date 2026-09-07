#pragma once

#include "DifferentiationMethod.hpp"
#include "IntegrationMethod.hpp"

namespace N::M {
/**
 * @brief Concept for callable objects that accept a float and return a float.
 */
template <typename T>
concept Func = std::invocable<T, float> && std::same_as<std::invoke_result_t<T, float>, float>;

/**
 * @brief Represents a real-valued mathematical function of one variable.
 *
 * Function provides evaluation, composition, numerical differentiation,
 * numerical integration, inverse evaluation, and arithmetic operations.
 *
 * Functions can be constructed from any callable satisfying the Func concept.
 */
struct Function {
    /**
     * @brief Constructs a function from a callable object.
     * @param f Callable that accepts a float and returns a float.
     */
    template <Func T>
        requires Func<T> && (!std::same_as<std::remove_cvref_t<T>, Function>)
    Function(T&& f) : Func(std::forward<T>(f)) {
    }

    /**
     * @brief Evaluates the function at a given point.
     * @param x Input value.
     * @return The value of the function at x.
     */
    [[nodiscard]] float Evaluate(float x) const;

    /**
     * @brief Creates a function representing the composition f(g(x)).
     * The supplied function g is evaluated first, and its result is passed
     * to this function f.
     * @param g Inner function
     * @return A new function representing f(g(x)).
     */
    [[nodiscard]] Function Compose(const Function& g) const;

    /**
     * @brief Numerically finds the input corresponding to a given output.
     * Uses binary search over the specified interval.
     * The function must be monotonic over the interval.
     * @param y Target output value.
     * @param domainMin Lower bound of the search interval.
     * @param domainMax Upper bound of the search interval.
     * @return An approximation of x such that f(x) = y.
     */
    [[nodiscard]] float InverseEvaluate(float y, float domainMin = 0, float domainMax = 10) const;

    /**
     * @brief Creates the inverse of the function restricted to a specified domain.
     * The function must be one-to-one (monotonic for continuous functions)
     * over the specified domain.
     * @param domainMin Lower bound of the domain restriction.
     * @param domainMax Upper bound of the domain restriction.
     * @return A function representing the inverse of the restricted function.
     */
    [[nodiscard]] Function Inverse(float domainMin, float domainMax) const;

    /**
     * @brief Numerically evaluates the derivative at a point.
     * @param x Point at which to evaluate the derivative.
     * @param dx Step size used for the numerical approximation.
     * @param method Finite-difference method used for differentiation.
     * @param relativeStep where to use relative step or the specified dx only.
     * @return An approximation of f'(x).
     */
    [[nodiscard]] float Derivative(float x, float dx = 0.001f, DifferentiationMethod method = DifferentiationMethod::Central,
        bool relativeStep = true) const;

    /**
     * @brief Creates a function representing the numerical derivative.
     * @param dx Step size used for the numerical approximation.
     * @param method Finite-difference method used for differentiation.
     * @param relativeStep whether to use relative step or the specified dx only.
     * @return A function approximating f'(x).
     */
    [[nodiscard]] Function Differentiate(
        float dx = 0.001f, DifferentiationMethod method = DifferentiationMethod::Central, bool relativeStep = true) const;

    /**
     * @brief Creates a function representing the numerical integral from a lower bound.
     * The returned function evaluates:
     * @code
     * F(x) = integral(lowerBound, x, f(t) dt)
     * @endcode
     * using a the specified integration method.
     * @param lowerBound Lower bound of integration.
     * @param dx Width of each integration interval.
     * @param method sum method used for integration.
     * @return A function representing the accumulated integral.
     */
    [[nodiscard]] Function Integrate(
        float lowerBound, float dx = 0.001f, IntegrationMethod method = IntegrationMethod::Midpoint) const;

    /**
     * @brief Numerically evaluates a definite integral.
     * Computes the integral of the function over [lowerBound, upperBound] using a the specified integration method.
     * @param lowerBound Lower bound of integration.
     * @param upperBound Upper bound of integration.
     * @param dx Width of each rectangle.
     * @param method sum method used for integration.
     * @return An approximation of the definite integral (signed area).
     */
    [[nodiscard]] float Integral(
        float lowerBound, float upperBound, float dx = 0.001f, IntegrationMethod method = IntegrationMethod::Midpoint) const;

    /**
     * @brief Creates a Taylor polynomial approximation of the function.
     *
     * The returned function approximates this function around the specified
     * center point `a` using the first `terms` terms of its Taylor series:
     *
     *     f(a) + f'(a)(x - a) + f''(a)(x - a)^2 / 2! + ...
     *
     * Each derivative is evaluated at the center point `a`.
     *
     * Note that `terms` specifies the number of terms, not the highest
     * polynomial degree. For example, `Taylor(4, a)` produces terms of
     * degrees 0 through 3.
     *
     * @remark Since the derivatives are computed numerically, higher-order Taylor
     * approximations will become increasingly affected by numerical error.
     *
     * @param terms Number of Taylor series terms to include.
     * @param a Point around which the function is approximated.
     * @return A function representing the Taylor polynomial approximation.
     */
    [[nodiscard]] Function Taylor(int terms, float a);

    /**
     * @brief Creates a Maclaurin polynomial approximation of the function.
     *
     * A Maclaurin polynomial is a Taylor polynomial centered at zero.
     * Equivalent to:
     * @code
     * Taylor(terms, 0)
     * @endcode
     *
     * @param terms Number of terms to include in the Maclaurin polynomial.
     * @return A function representing the Maclaurin polynomial approximation.
     */
    [[nodiscard]] Function Maclaurin(int terms);

    /**
     * @brief Evaluates the function at x.
     * @param x Input value.
     * @return The value of the function at x.
     */
    float operator()(float x) const;

    /**
     * @brief Composes this function with another function.
     * Equivalent to Compose(g).
     */
    Function operator()(const Function& g) const;

    /** @brief Adds another function pointwise. */
    Function operator+(const Function& g) const;

    /** @brief Subtracts another function pointwise. */
    Function operator-(const Function& g) const;

    /** @brief Multiplies another function pointwise. */
    Function operator*(const Function& g) const;

    /** @brief Divides this function by another function pointwise. */
    Function operator/(const Function& g) const;

    /** @brief Negates the function. */
    Function operator-() const;

    /** @brief Adds a scalar to the function. */
    Function operator+(float scalar) const;

    /** @brief Subtracts a scalar from the function. */
    Function operator-(float scalar) const;

    /** @brief Multiplies the function by a scalar. */
    Function operator*(float scalar) const;

    /** @brief Divides the function by a scalar. */
    Function operator/(float scalar) const;

    /** @brief Adds a scalar to a function. */
    friend Function operator+(float scalar, const Function& f);

    /** @brief Subtracts a function from a scalar. */
    friend Function operator-(float scalar, const Function& f);

    /** @brief Multiplies a function by a scalar. */
    friend Function operator*(float scalar, const Function& f);

    /** @brief Divides a scalar by a function. */
    friend Function operator/(float scalar, const Function& f);

private:
    std::function<float(float)> Func;
};
} // namespace N::M
