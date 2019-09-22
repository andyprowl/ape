#pragma once

#include <functional>
#include <vector>

template<typename T>
class Receiver
{

public:

    virtual auto receive(T const & value)
        -> void = 0;

};

template<typename T>
class Sender
{

public:

    auto send(T const & value) const
        -> void
    {
        for (auto const receiver : receivers)
        {
            receiver->receive(value);
        }
    }

    auto connect(Receiver<T> & receiver)
        -> void
    {
        receivers.push_back(&receiver);
    }

public:

    std::vector<Receiver<T> *> receivers; 

};

template<typename T, typename U>
class Transformer : public Receiver<T>, public Sender<U>
{

public:

    using TransformationFunction = std::function<auto (T const & value) -> U>;

public:

    explicit Transformer(TransformationFunction function)
        : function{std::move(function)}
    {
    }

    // virtual (from Receiver<T>
    auto receive(T const & value)
        -> void
    {
        auto const transformedValue = function(value);

        this->send(transformedValue);
    }

private:

    TransformationFunction function;

};
