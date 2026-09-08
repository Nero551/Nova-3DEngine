#pragma once
#include "DifferentiationMethod.hpp"
#include "IntegrationMethod.hpp"
#include "Math/Common/Comparison.hpp"
#include "Math/Common/Exponentials.hpp"
#include "Utilities/Logger.hpp"

namespace N::M
{
/** @brief Checks whether a type is a floating-point scalar (float). */
template <typename T>
concept IsScalar = std::same_as<std::remove_cvref_t<T>, float>;

template <typename Input, typename Output> struct Function;
/** @brief Checks whether a callable accepts Input, returns Output, and is not already a Function. */
template <typename F, typename Input, typename Output>
concept CompatibleCallable = std::same_as<std::invoke_result_t<F, Input>, Output> &&
    (!std::same_as<std::remove_cvref_t<F>, Function<Input, Output>>);

/** @brief Represents a mathematical function mapping an Input type to an Output type. */
template <typename Input, typename Output> struct Function
{
    /**
     * @brief Constructs a function from a callable returning Output when given Input.
     * @param f Callable used to evaluate the function.
     */
    template <typename F> requires CompatibleCallable<F, Input, Output>
    Function(F&& f) : Func(std::forward<F>(f)){};

    /**
     * @brief Evaluates the function at the given input.
     * @param input Input value at which to evaluate the function.
     * @return The function value at input.
     */
    Output Evaluate(const Input& input) const
    {
        return Func(input);
    }

    /**
     * @brief Composes this function with another function, producing f(g(x)).
     * @param g Function applied before this function.
     * @return The composed function.
     */
    template <typename Middle>
    Function<Middle, Output> Compose(const Function<Middle, Input>& g) const
    {
        return [f = *this, g](const Middle& input) { return f(g(input)); };
    }

    /**
     * @brief Calculates the numerical derivative of the function.
     * @param dx Step size used for numerical differentiation.
     * @param method Numerical differentiation method to use.
     * @param relativeStep Whether dx scales with the input magnitude.
     * @return A function representing the numerical derivative.
     * @note Only available for functions with a scalar Input type.
     */
    Function<float, Output> Differentiate(float dx = 0.001f,
        DifferentiationMethod method = DifferentiationMethod::Central,
        bool relativeStep = true) const requires IsScalar<Input>
    {
        return [f = *this, dx, method, relativeStep](const float x)
        {
            const float h = relativeStep ? dx * std::max(1.0f, std::abs(x)) : dx;
            switch (method)
            {
            case DifferentiationMethod::Central:
                return (f(x + h) - f(x - h)) / (2.0f * h);
            case DifferentiationMethod::Forward:
                return (f(x + h) - f(x)) / h;
            case DifferentiationMethod::Backward:
                return (f(x) - f(x - h)) / h;
            default:
                U::Logger::Fatal("Invalid Differentiation Method");
                return Output{};
            }
        };
    }

    /**
     * @brief Evaluates the numerical derivative at x.
     * @param x Input value at which to evaluate the derivative.
     * @param dx Step size used for numerical differentiation.
     * @param method Numerical differentiation method to use.
     * @param relativeStep Whether dx scales with the input magnitude.
     * @return The numerical derivative at x.
     * @note Only available for functions with a scalar Input type.
     */
    Output Derivative(float x, float dx = 0.001f,
        DifferentiationMethod method = DifferentiationMethod::Central,
        bool relativeStep = true) const requires IsScalar<Input>
    {
        return Differentiate(dx, method, relativeStep)(x);
    }

    /**
     * @brief Evaluates the definite integral from lowerBound to upperBound.
     * @param lowerBound Lower bound of the integration interval.
     * @param upperBound Upper bound of the integration interval.
     * @param dx Step size used for numerical integration.
     * @param method Numerical integration method to use.
     * @return The approximate value of the definite integral.
     * @note Only available for functions with a scalar Input type.
     */
    Output Integral(float lowerBound, float upperBound, float dx = 0.001f,
        IntegrationMethod method = IntegrationMethod::Midpoint) const requires IsScalar<Input>
    {
        return Integrate(lowerBound, dx, method)(upperBound);
    }

    /**
     * @brief Creates a numerical integral function with respect to its upper bound.
     * @param lowerBound Lower bound of the integration interval.
     * @param dx Step size used for numerical integration.
     * @param method Numerical integration method to use.
     * @return A function whose value is the integral from lowerBound to its input.
     * @note Only available for functions with a scalar Input type.
     */
    Function<float, Output> Integrate(float lowerBound, float dx = 0.001f,
        IntegrationMethod method = IntegrationMethod::Midpoint) const requires IsScalar<Input>
    {
        return [f = *this, lowerBound, dx, method](const float upperBound)
        {
            Output result{};
            for (float x = lowerBound; x < upperBound; x += dx)
            {
                const float width = std::min(dx, upperBound - x);
                switch (method)
                {
                case IntegrationMethod::Midpoint:
                    result += f(x + width / 2.0f) * width;
                    break;
                case IntegrationMethod::Right:
                    result += f(x + width) * width;
                    break;
                case IntegrationMethod::Left:
                    result += f(x) * width;
                    break;
                case IntegrationMethod::Trapezoid:
                    result += (f(x) + f(x + width)) / 2.0f * width;
                    break;
                default:
                    U::Logger::Fatal("Invalid Integration Method");
                }
            }
            return result;
        };
    }

    /**
     * @brief Creates a Taylor polynomial approximation about the point a.
     * @param terms Number of terms in the Taylor polynomial.
     * @param a Point about which the polynomial is expanded.
     * @return A function representing the Taylor polynomial approximation.
     * @note Only available for functions with a scalar Input type.
     */
    Function<float, Output> Taylor(unsigned int terms, float a) const requires IsScalar<Input>
    {
        return [terms, a, f = *this](const float x)
        {
            Output result{};
            Function currentFunc = f;
            for (int n = 0; n < terms; ++n)
            {
                result += currentFunc(a) * std::pow(x - a, n) / Factorial(n);
                currentFunc = currentFunc.Differentiate();
            }
            return result;
        };
    }

    /**
     * @brief Creates a Taylor polynomial approximation centered at zero.
     * @param terms Number of terms in the Taylor polynomial.
     * @return A function representing the Maclaurin polynomial approximation.
     * @note Only available for functions with a scalar Input type.
     */
    Function<float, Output> Maclaurin(unsigned int terms) const requires IsScalar<Input>
    {
        return Taylor(terms, 0.0f);
    }

    /**
     * @brief Numerically finds the input x such that f(x) approximately equals y using binary search.
     * @param y Function value whose corresponding input is sought.
     * @param domainMin Lower bound of the search domain.
     * @param domainMax Upper bound of the search domain.
     * @return An input value whose function value approximately equals y.
     * @note Requires a scalar-to-scalar function that is monotonic over the
     * given domain.
     */
    float InverseEvaluate(float y, float domainMin, float domainMax) const
        requires IsScalar<Input> && IsScalar<Output>
    {
        float x = 0.0f;
        //Binary search, i need a better way to calculate this. am too stupid though.
        while (!NearlyEquals(domainMax, domainMin))
        {
            x = (domainMin + domainMax) / 2.0f;
            const float value = Evaluate(x);
            if (value < y)
                domainMin = x;
            else
                domainMax = x;
        }
        return x;
    }

    /**
     * @brief Creates the numerical inverse of a scalar-to-scalar function over the given domain.
     * @param domainMin Lower bound of the function's domain.
     * @param domainMax Upper bound of the function's domain.
     * @return A function representing the numerical inverse.
     * @note The function must be monotonic over the given domain.
     */
    Function<float, float> Inverse(float domainMin, float domainMax) const
        requires IsScalar<Input> && IsScalar<Output>
    {
        return [f = *this, domainMin, domainMax](const float y) -> float
        { return f.InverseEvaluate(y, domainMin, domainMax); };
    }

    /** @brief Composes this function with another function, producing f(g(x)). */
    template <typename Middle>
    Function<Middle, Output> operator()(const Function<Middle, Input>& g) const
    {
        return Compose(g);
    }

    /** @brief Evaluates the function at the given input. */
    Output operator()(const Input& input) const
    {
        return Evaluate(input);
    }

    /** @brief Adds two functions pointwise, producing f(x) + g(x). */
    Function operator+(const Function& g) const
    {
        return [f = *this, g](const Input& x) { return f(x) + g(x); };
    }

    /** @brief Subtracts two functions pointwise, producing f(x) - g(x). */
    Function operator-(const Function& g) const
    {
        return [f = *this, g](const Input& x) { return f(x) - g(x); };
    }

    /** @brief Multiplies two functions pointwise, producing f(x) * g(x). */
    Function operator*(const Function& g) const
    {
        return [f = *this, g](const Input& x) { return f(x) * g(x); };
    }

    /** @brief Divides two functions pointwise, producing f(x) / g(x). */
    Function operator/(const Function& g) const
    {
        return [f = *this, g](const Input& x) { return f(x) / g(x); };
    }

    /** @brief Adds another function pointwise to this function. */
    Function& operator+=(const Function& g)
    {
        return *this = *this + g;
    }

    /** @brief Subtracts another function pointwise from this function. */
    Function& operator-=(const Function& g)
    {
        return *this = *this - g;
    }

    /** @brief Multiplies this function pointwise by another function. */
    Function& operator*=(const Function& g)
    {
        return *this = *this * g;
    }

    /** @brief Divides this function pointwise by another function. */
    Function& operator/=(const Function& g)
    {
        return *this = *this / g;
    }

    /** @brief Negates the function, producing -f(x). */
    Function operator-() const
    {
        return [f = *this](const Input& x) { return -f(x); };
    }

    /** @brief Adds a value of the Output type to the function result. */
    Function operator+(const Output& value) const
    {
        return [f = *this, value](const Input& x) { return f(x) + value; };
    }

    /** @brief Subtracts a value of the Output type from the function result. */
    Function operator-(const Output& value) const
    {
        return [f = *this, value](const Input& x) { return f(x) - value; };
    }

    /** @brief Multiplies the function result by a value of the Output type. */
    Function operator*(const Output& value) const
    {
        return [f = *this, value](const Input& x) { return f(x) * value; };
    }

    /** @brief Divides the function result by a value of the Output type. */
    Function operator/(const Output& value) const
    {
        return [f = *this, value](const Input& x) { return f(x) / value; };
    }

    /** @brief Adds a value of the Output type to the function result from the
     * left-hand side. */
    friend Function operator+(const Output& value, const Function& f)
    {
        return f + value;
    }

    /** @brief Subtracts the function result from a value of the Output type. */
    friend Function operator-(const Output& value, const Function& f)
    {
        return [f, value](const Input& x) { return value - f(x); };
    }

    /** @brief Multiplies a value of the Output type by the function result. */
    friend Function operator*(const Output& value, const Function& f)
    {
        return f * value;
    }

    /** @brief Divides a value of the Output type by the function result. */
    friend Function operator/(const Output& value, const Function& f)
    {
        return [f, value](const Input& x) { return value / f(x); };
    }

    /** @brief Adds a value of the Output type to this function. */
    Function& operator+=(const Output& value)
    {
        return *this = *this + value;
    }

    /** @brief Subtracts a value of the Output type from the function result. */
    Function& operator-=(const Output& value)
    {
        return *this = *this - value;
    }

    /** @brief Multiplies this function by a value of the Output type. */
    Function& operator*=(const Output& value)
    {
        return *this = *this * value;
    }

    /** @brief Divides this function by a value of the Output type. */
    Function& operator/=(const Output& value)
    {
        return *this = *this / value;
    }

    /** @brief Adds a scalar to the function result. */
    Function operator+(float scalar) const requires(!IsScalar<Output>)
    {
        return [f = *this, scalar](const Input& x) { return f(x) + scalar; };
    }

    /** @brief Subtracts a scalar from the function result. */
    Function operator-(float scalar) const requires(!IsScalar<Output>)
    {
        return [f = *this, scalar](const Input& x) { return f(x) - scalar; };
    }

    /** @brief Multiplies the function result by a scalar. */
    Function operator*(float scalar) const requires(!IsScalar<Output>)
    {
        return [f = *this, scalar](const Input& x) { return f(x) * scalar; };
    }

    /** @brief Divides the function result by a scalar. */
    Function operator/(float scalar) const requires(!IsScalar<Output>)
    {
        return [f = *this, scalar](const Input& x) { return f(x) / scalar; };
    }

    /** @brief Adds a scalar to the function result from the left-hand side. */
    friend Function operator+(float scalar, const Function& f) requires(!IsScalar<Output>)
    {
        return f + scalar;
    }

    /** @brief Subtracts the function result from a scalar. */
    friend Function operator-(float scalar, const Function& f) requires(!IsScalar<Output>)
    {
        return [scalar, f](const Input& x) { return scalar - f(x); };
    }

    /** @brief Multiplies a scalar by the function result. */
    friend Function operator*(float scalar, const Function& f) requires(!IsScalar<Output>)
    {
        return f * scalar;
    }

    /** @brief Divides a scalar by the function result. */
    friend Function operator/(float scalar, const Function& f) requires(!IsScalar<Output>)
    {
        return [scalar, f](const Input& x) { return scalar / f(x); };
    }

    /** @brief Adds a scalar to this function. */
    Function& operator+=(float scalar) requires(!IsScalar<Output>)
    {
        return *this = *this + scalar;
    }

    /** @brief Subtracts a scalar from the function result. */
    Function& operator-=(float scalar) requires(!IsScalar<Output>)
    {
        return *this = *this - scalar;
    }

    /** @brief Multiplies this function by a scalar. */
    Function& operator*=(float scalar) requires(!IsScalar<Output>)
    {
        return *this = *this * scalar;
    }

    /** @brief Divides this function by a scalar. */
    Function& operator/=(float scalar) requires(!IsScalar<Output>)
    {
        return *this = *this / scalar;
    }

  private:
    std::function<Output(Input)> Func;
};
} // namespace N::M