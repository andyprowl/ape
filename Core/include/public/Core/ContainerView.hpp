//--------------------------------------------------------------------------------------------------
// Honeywell Proprietary
// This document and all information and expression contained herein are the property of
// Honeywell International Inc., are loaned in confidence, and may not, in whole or in
// part, be used, duplicated or disclosed for any purpose without prior written permission
// of Honeywell International Inc. This document is an unpublished work.
// Copyright 2011 Honeywell International Inc. All rights reserved.
//--------------------------------------------------------------------------------------------------

#pragma once

#include <iterator>
#include <type_traits>
#include <vector>

namespace ape
{

template<typename Container, typename = void>
class ContainerView
{

public:

    explicit ContainerView(Container & container)
        : container{&container}
    {
    }

    auto size() const
    {
        return container->size();
    }

    auto empty() const
    {
        return container->empty();
    }
        
    auto begin() const
    {
        return container->begin();
    }

    auto cbegin() const
    {
        return container->cbegin();
    }

    auto end() const
    {
        return container->end();
    }

    auto cend() const
    {
        return container->cend();
    }

    auto front() const
        -> auto const &
    {
        return container->front();
    }

    auto back() const
        -> decltype(std::declval<Container>().back())
    {
        return container->back();
    }

    auto operator [] (const std::size_t index) const
        -> auto const &
    {
        return (*container)[index];
    }

protected:

    Container * container;

};

template<typename Container>
class ContainerView<
    Container,
    std::enable_if_t<!std::is_const_v<Container>>> 
    : public ContainerView<Container const>
{

public:
    
    using ConstView = ContainerView<std::add_const_t<Container>>;

public:

    using ConstView::begin;

    using ConstView::end;

    using ConstView::front;

    using ConstView::back;

    using ConstView::operator [];

    explicit ContainerView(Container & container)
        : ConstView{container}
    {
    }

    auto begin()
    {
        return const_cast<Container *>(this->container)->begin();
    }
    
    auto end()
    {
        return const_cast<Container *>(this->container)->end();
    }

    auto front()
        -> auto &
    {
        return const_cast<Container *>(this->container)->front();
    }

    auto back()
        -> decltype(std::declval<Container>().back())
    {
        return const_cast<Container *>(this->container)->back();
    }

    auto operator [] (const std::size_t index)
        -> auto &
    {
        auto & mutableContainer = const_cast<Container &>(*this->container);

        return mutableContainer[index];
    }

};

template<typename Container>
auto makeView(Container & c)
    -> ContainerView<Container>
{
    return ContainerView<Container>{c};
}

} // namespace ape
