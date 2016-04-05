#pragma once

#include <iostream>

#include <exception>
#include <boost/optional.hpp>

#include <memory>

template <typename T>
class fallible
{
public:
    /**
     *  fallible objects are copy constructible.
     */
    fallible(fallible const& other) = default;

    /**
     *  fallible objects are move constructible.
     */
    fallible(fallible&& other) = default;

    /**
     *  fallible are copy assignable.
     */
    fallible& operator=(fallible const& other) = default;

    /**
     *  fallible objects are move assignable
     */
    fallible& operator=(fallible&& other) = default;

    /**
     *  This class does not support sub-classing / derivation.
     */
    ~fallible() = default;

    /**
     *  Constructor creates a fallible object with a result value.
     *  The fallible object does not contain an exception.
     *
     *  @param value A valid value to store.
     */
    constexpr fallible(T const& value)
        : exc{}
        , value{value}
    {
    }

    /**
     *  Constructor creates a fallible object with a result value.
     *  The fallible object does not contain an exception.
     *
     *  @param value A valid value to store.
     */
    constexpr fallible(T&& value)
        : exc{}
        , value{std::move(value)}
    {
    }

    /**
     *  A fallible object created from an exception does not contain a value
     *  and obviously represents a delayed exception now.
     */
    template <typename ExceptionT>
    constexpr fallible(ExceptionT const& exception)
        : exc{new ExceptionT {exception}}
        , value{}
    {
    }

    /**
     *  A fallible object created from an exception does not contain a value
     *  and obviously represents a delayed exception now.
     */
    template <typename ExceptionT>
    constexpr fallible(ExceptionT&& exception)
        : exc(new ExceptionT {std::move(exception)})
        , value{}
    {
    }

    /**
     *  @return Returns the capsulated value.
     */
    T get() const
    {
        return value.get();
    }

    /**
     *  @return Returns the exception message.
     */
    const char* what() const noexcept
    {
        if (!operator bool())
            return exc->what();
        else
            return nullptr;
    }

    /**
     *  @return Is this class containing a valid value?
     */
    explicit operator bool() const
    {
        return static_cast <bool> (value);
    }

private:
    std::unique_ptr <std::exception> exc;
    boost::optional <T> value;
};
