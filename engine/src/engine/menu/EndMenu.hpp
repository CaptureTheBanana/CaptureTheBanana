// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_MENU_ENDMENU_HPP
#define ENGINE_MENU_ENDMENU_HPP

#include "engine/core/Game.hpp"
#include "engine/graphics/Rect.hpp"
#include "engine/menu/Menu.hpp"

namespace ctb {
namespace engine {

class Label;
class Player;

/// The final menu showing the winning team and list of all players with scores
class EndMenu : public Menu {
   public:
    explicit EndMenu(Team teamWon);

    /// Renders the menu
    void render() override;

    ~EndMenu() override;

   private:
    /// Input handler
    void handleInput(InputType type, const Input* input, bool state, float angle) override;

    /// Create all labels needed
    void setupUI();

    /// Check, if one player has a new highscore.
    void checkForHighscores();

    /// The team that has won
    Team m_team;

    /// All labels used in this menu
    std::vector<Label*> m_labels;

    /// List of all players in the R2L team
    std::vector<Player*> m_r2l;

    /// List of all players in the L2R team
    std::vector<Player*> m_l2r;

    /// divider rect between the two teams
    Rect* m_divider;

    /// Whether already checked for new highscores.
    bool m_checkedForHighscores;

    /// Sorts players by their score
    struct PlayerScoreSorter {
        inline bool operator()(Player*& a1, Player*& a2) {
            return (a1->getScore() > a2->getScore());
        }
    };
};

}  // namespace engine
}  // namespace ctb

#endif
