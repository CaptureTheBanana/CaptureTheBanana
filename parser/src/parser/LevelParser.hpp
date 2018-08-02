// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef PARSER_LEVELPARSER_HPP
#define PARSER_LEVELPARSER_HPP

#include "parser/LevelConfig.hpp"

#include <string>

#include <gsl/gsl>

namespace ctb {
namespace parser {

/// \brief Level parser
///
/// \param absoluteFilename the absolute filename to the level xml file
/// \param relativeFilename the relative filename from the game.xml to the level.xml
/// \return LevelConfig* Returns the level configuration.
LevelConfig* parseLevel(const std::string& absoluteFilename, const std::string& relativeFilename);

/// \brief stores a level configuration to an xml file
///
/// \param levelConfig pointer to the level config to be saved
/// \param gameXmlFile path to game.xml
void saveLevel(gsl::not_null<LevelConfig*> levelConfig, const std::string& gameXmlFile);

}  // namespace parser
}  // namespace ctb

#endif
