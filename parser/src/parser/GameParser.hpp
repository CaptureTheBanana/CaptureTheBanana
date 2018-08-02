// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef PARSER_GAMEPARSER_HPP
#define PARSER_GAMEPARSER_HPP

#include "parser/GameConfig.hpp"

#include <string>

#include <gsl/gsl>

namespace ctb {
namespace parser {

/// Game validator status code
enum class GameValidatorStatus {
    /// Success
    kOk = 0,

    /// Game has no levels
    kEmptyGame = 1,
    /// Level has no layers/tilesets
    kEmptyLevel = 2,
    /// No left door available
    kNoDoorL = 3,
    /// No right door available
    kNoDoorR = 4,
    /// No flag spawn available
    kNoFlagSpawn = 5,
    /// No player spawns available
    kNoPlayerSpawn = 6,
    /// Game has no center level
    kNoCenterLevel = 7,
    /// Game has no default level
    kNoDefaultLevel = 8,
    /// Game has no end level
    kNoEndLevel = 9,

    kError = 255
};

/// Game validator information
struct GameValidatorResult {
    /// Status code
    GameValidatorStatus status{GameValidatorStatus::kError};
    /// Name of the incorrect level or empty
    std::string name{};
};

/// \brief Game parser
///
/// \param file The path to game.xml file.
/// \return GameConfig Returns a pointer to the game configuration.
GameConfig* parseGame(const std::string& file);

/// \brief Update game levels in the given xml file
///
/// \param gamexml Path to the game.xml
/// \param game Game configuration
void saveGameLevels(const std::string& gamexml, gsl::not_null<GameConfig*> game);

/// \brief Converts a GameValidatorStatus to a string message.
///
/// \param status Status code
/// \return std::string Message
std::string to_string(GameValidatorStatus status);

/// \brief Converts a GameValidatorResult to a string message.
///
/// \param status Status code
/// \return std::string Message
std::string to_string(const GameValidatorResult& info);

/// \brief Validates the game configuration.
///
/// \param gameConfig Game configuration
/// \return GameValidatorResult Validator information
GameValidatorResult validateGameConfig(gsl::not_null<GameConfig*> gameConfig);

}  // namespace parser
}  // namespace ctb
#endif  // PARSER_GAMEPARSER_HPP
