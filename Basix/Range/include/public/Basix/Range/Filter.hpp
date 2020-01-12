#pragma once

#include <algorithm>

namespace basix
{

template<typename Cont, typename Pred>
auto filterIn(Cont cont, Pred const pred)
{
    auto result = std::vector<typename Cont::value_type>{};

    std::copy_if(std::cbegin(cont), std::cend(cont), std::back_inserter(result), pred);

    return result;
}

template<typename Cont, typename Pred>
auto filterOut(Cont cont, Pred const pred)
{
    auto result = std::vector<typename Cont::value_type>{};

    std::copy_if_not(std::cbegin(cont), std::cend(cont), std::back_inserter(result), pred);

    return result;
}

} // namespace basix
