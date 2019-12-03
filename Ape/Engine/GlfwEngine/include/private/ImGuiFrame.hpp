#pragma once

namespace ape
{

class ImGuiFrame
{

public:

    ImGuiFrame();

    ImGuiFrame(ImGuiFrame const &) = delete;

    ImGuiFrame(ImGuiFrame && rhs) noexcept;

    auto operator = (ImGuiFrame const & rhs)
        -> ImGuiFrame & = delete;

    auto operator = (ImGuiFrame && rhs) noexcept
        -> ImGuiFrame &;

    ~ImGuiFrame();

private:

    bool isValid;

};

} // namespace ape
