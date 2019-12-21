#pragma once

namespace ape
{

class ImGuiEventDispatcher;

class ImGuiFrame
{

public:

    explicit ImGuiFrame(ImGuiEventDispatcher & dispatcher);

    ImGuiFrame(ImGuiFrame const &) = delete;

    ImGuiFrame(ImGuiFrame && rhs) noexcept;

    auto operator = (ImGuiFrame const & rhs)
        -> ImGuiFrame & = delete;

    auto operator = (ImGuiFrame && rhs) noexcept
        -> ImGuiFrame &;

    ~ImGuiFrame();

private:

    ImGuiEventDispatcher * dispatcher;

};

} // namespace ape
