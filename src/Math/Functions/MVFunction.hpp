#pragma once

namespace N::M {
/** @brief Represents a mathematical multi-variable function mapping an Input type to an Output type. */
template <typename Input, typename Output> struct MVFunction {
    /** @brief Constructs a function from a callable returning Output when given Input. */
    template <typename F>
        requires std::same_as<std::invoke_result_t<F, Input>, Output>
    MVFunction(F&& f) : Func(std::forward<F>(f)) {
    }

    /** @brief Evaluates the function at the given input. */
    Output Evaluate(const Input& input) const {
        return Func(input);
    }

    /** @brief Composes this function with another function, producing f(g(x)). */
    MVFunction Compose(const MVFunction& g) const {
        return [f = *this, g](const Input input) -> Output { return f(g(input)); };
    }

    /** @brief Composes this function with another function, producing f(g(x)). */
    MVFunction operator()(const MVFunction& g) const {
        return Compose(g);
    }

    /** @brief Evaluates the function at the given input. */
    Output operator()(const Input& input) const {
        return Evaluate(input);
    }

    /** @brief Adds two functions pointwise, producing f(x) + g(x). */
    MVFunction operator+(const MVFunction& g) const {
        return [f = *this, g](const Input input) -> Output { return f(input) + g(input); };
    }

    /** @brief Subtracts two functions pointwise, producing f(x) - g(x). */
    MVFunction operator-(const MVFunction& g) const {
        return [f = *this, g](const Input input) -> Output { return f(input) - g(input); };
    }

    /** @brief Multiplies two functions pointwise, producing f(x) * g(x). */
    MVFunction operator*(const MVFunction& g) const {
        return [f = *this, g](const Input input) -> Output { return f(input) * g(input); };
    }

    /** @brief Divides two functions pointwise, producing f(x) / g(x). */
    MVFunction operator/(const MVFunction& g) const {
        return [f = *this, g](const Input input) -> Output { return f(input) / g(input); };
    }

    /** @brief Adds another function pointwise to this function. */
    MVFunction& operator+=(const MVFunction& g) {
        Func = [f = *this, g](const Input input) -> Output { return f(input) + g(input); };
        return *this;
    }

    /** @brief Subtracts another function pointwise from this function. */
    MVFunction& operator-=(const MVFunction& g) {
        Func = [f = *this, g](const Input input) -> Output { return f(input) - g(input); };
        return *this;
    }

    /** @brief Multiplies this function pointwise by another function. */
    MVFunction& operator*=(const MVFunction& g) {
        Func = [f = *this, g](const Input input) -> Output { return f(input) * g(input); };
        return *this;
    }

    /** @brief Divides this function pointwise by another function. */
    MVFunction& operator/=(const MVFunction& g) {
        Func = [f = *this, g](const Input input) -> Output { return f(input) / g(input); };
        return *this;
    }

    /** @brief Negates the function, producing -f(x). */
    MVFunction operator-() const {
        return [f = *this](const Input input) -> Output { return -f(input); };
    }

    /** @brief Adds a value of the Output type to the function result. */
    MVFunction operator+(const Output& value) const {
        return [f = *this, value](const Input input) -> Output { return f(input) + value; };
    }

    /** @brief Subtracts a value of the Output type from the function result. */
    MVFunction operator-(const Output& value) const {
        return [f = *this, value](const Input input) -> Output { return f(input) - value; };
    }

    /** @brief Multiplies the function result by a value of the Output type. */
    MVFunction operator*(const Output& value) const {
        return [f = *this, value](const Input input) -> Output { return f(input) * value; };
    }

    /** @brief Divides the function result by a value of the Output type. */
    MVFunction operator/(const Output& value) const {
        return [f = *this, value](const Input input) -> Output { return f(input) / value; };
    }

    /** @brief Adds a value of the Output type to the function result from the left-hand side. */
    friend MVFunction operator+(const Output& value, const MVFunction& f) {
        return [f, value](const Input input) -> Output { return value + f(input); };
    }

    /** @brief Subtracts the function result from a value of the Output type. */
    friend MVFunction operator-(const Output& value, const MVFunction& f) {
        return [f, value](const Input input) -> Output { return value - f(input); };
    }

    /** @brief Multiplies a value of the Output type by the function result. */
    friend MVFunction operator*(const Output& value, const MVFunction& f) {
        return [f, value](const Input input) -> Output { return value * f(input); };
    }

    /** @brief Divides a value of the Output type by the function result. */
    friend MVFunction operator/(const Output& value, const MVFunction& f) {
        return [f, value](const Input input) -> Output { return value / f(input); };
    }

    /** @brief Adds a value of the Output type to this function. */
    MVFunction& operator+=(const Output& value) {
        Func = [f = *this, value](const Input input) -> Output { return f(input) + value; };
        return *this;
    }

    /** @brief Subtracts a value of the Output type from this function. */
    MVFunction& operator-=(const Output& value) {
        Func = [f = *this, value](const Input input) -> Output { return f(input) - value; };
        return *this;
    }

    /** @brief Multiplies this function by a value of the Output type. */
    MVFunction& operator*=(const Output& value) {
        Func = [f = *this, value](const Input input) -> Output { return f(input) * value; };
        return *this;
    }

    /** @brief Divides this function by a value of the Output type. */
    MVFunction& operator/=(const Output& value) {
        Func = [f = *this, value](const Input input) -> Output { return f(input) / value; };
        return *this;
    }

    /** @brief Adds a scalar to the function result. */
    MVFunction operator+(float scalar) const {
        return [f = *this, scalar](const Input input) -> Output { return f(input) + scalar; };
    }

    /** @brief Subtracts a scalar from the function result. */
    MVFunction operator-(float scalar) const {
        return [f = *this, scalar](const Input input) -> Output { return f(input) - scalar; };
    }

    /** @brief Multiplies the function result by a scalar. */
    MVFunction operator*(float scalar) const {
        return [f = *this, scalar](const Input input) -> Output { return f(input) * scalar; };
    }

    /** @brief Divides the function result by a scalar. */
    MVFunction operator/(float scalar) const {
        return [f = *this, scalar](const Input input) -> Output { return f(input) / scalar; };
    }

    /** @brief Adds a scalar to the function result from the left-hand side. */
    friend MVFunction operator+(float scalar, const MVFunction& f) {
        return [f, scalar](const Input input) -> Output { return scalar + f(input); };
    }

    /** @brief Subtracts the function result from a scalar. */
    friend MVFunction operator-(float scalar, const MVFunction& f) {
        return [f, scalar](const Input input) -> Output { return scalar - f(input); };
    }

    /** @brief Multiplies a scalar by the function result. */
    friend MVFunction operator*(float scalar, const MVFunction& f) {
        return [f, scalar](const Input input) -> Output { return scalar * f(input); };
    }

    /** @brief Divides a scalar by the function result. */
    friend MVFunction operator/(float scalar, const MVFunction& f) {
        return [f, scalar](const Input input) -> Output { return scalar / f(input); };
    }

    /** @brief Adds a scalar to this function. */
    MVFunction& operator+=(float scalar) {
        Func = [f = *this, scalar](const Input input) -> Output { return f(input) + scalar; };
        return *this;
    }

    /** @brief Subtracts a scalar from this function. */
    MVFunction& operator-=(float scalar) {
        Func = [f = *this, scalar](const Input input) -> Output { return f(input) - scalar; };
        return *this;
    }

    /** @brief Multiplies this function by a scalar. */
    MVFunction& operator*=(float scalar) {
        *this = *this * scalar;
        return *this;
    }

    /** @brief Divides this function by a scalar. */
    MVFunction& operator/=(float scalar) {
        Func = [f = *this, scalar](const Input input) -> Output { return f(input) / scalar; };
        return *this;
    }

private:
    std::function<Output(Input)> Func;
};
} // namespace N::M
