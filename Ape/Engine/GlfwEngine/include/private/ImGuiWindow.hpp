#pragma once

#include <Basix/Mathematics/Position.hpp>
#include <Basix/Mathematics/Size.hpp>

#include <string_view>

namespace ape
{

class ImGuiWindow
{

public:

    explicit ImGuiWindow(std::string_view title);

    ImGuiWindow(
        std::string_view title,
        basix::Position<int> const & initialPosition,
        basix::Size<int> const & initialSize);

    ImGuiWindow(ImGuiWindow const & rhs) = delete;

    ImGuiWindow(ImGuiWindow && rhs) noexcept;

    auto operator = (ImGuiWindow const & rhs)
        -> ImGuiWindow & = delete;

    auto operator = (ImGuiWindow && rhs) noexcept
        -> ImGuiWindow &;

    ~ImGuiWindow();

private:

    bool isValid;

};

} // namespace ape
