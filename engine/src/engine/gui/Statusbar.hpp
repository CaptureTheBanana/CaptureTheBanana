// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_GUI_STATUSBAR_HPP
#define ENGINE_GUI_STATUSBAR_HPP

#include <map>
#include <vector>

#include "engine/graphics/Rect.hpp"
#include "engine/graphics/SDLRenderable.hpp"

namespace ctb {
namespace engine {

class Label;

class Player;

/// The in-game GUI for scores, health and more information
class Statusbar : public SDLRenderable {
   public:
    Statusbar();

    /// Renders the status bar
    void render() override;

    void setupUI();

    ~Statusbar() override;

   private:
    /// The width of the combo of one player and the score and health label
    static int entryWidth;

    /// Set the frame rate for the blinking
    void setFPS(int fps);

    /// Sets the black rectangle over the current level and make it blink
    void updateCurrentLevelRect();

    /// All labels
    std::vector<Label*> m_labels;

    /// Maps the player to his/her score label for easy access
    std::map<Player*, Label*> m_scoreLabels;

    /// Maps the player to his/her health label for easy access
    std::map<Player*, Label*> m_healthLabels;

    /// Maps the heart label to the player
    std::map<Player*, Label*> m_heartLabels;

    /// All rects used for the level indicator
    std::vector<Rect*> m_rects;

    /// The one rect that causes the blink effekt for the current level
    Rect* m_currentLevelRect;

    /// Whether to show the rect or not. Used for blinking
    bool m_currentLevelRectShow;

    /// Ticks count when the last frame was rendered
    Uint32 m_lastRenderTicks;

    /// Timeout between frames
    Uint32 m_frameTimeout;
};

}  // namespace engine
}  // namespace ctb

#endif
