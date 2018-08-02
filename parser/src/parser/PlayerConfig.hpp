// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef PARSER_PLAYERCONFIG_HPP
#define PARSER_PLAYERCONFIG_HPP

#include <string>
#include <utility>

namespace ctb {
namespace parser {

/// Class containing all infos about the players
class PlayerConfig {
   public:
    inline PlayerConfig(std::string filename, int frameWidth, int frameHeight, int numFrames)
        : m_filename(std::move(filename)),
          m_frameWidth(frameWidth),
          m_frameHeight(frameHeight),
          m_numFrames(numFrames) {}

    inline std::string getName() { return m_filename; }
    inline int getFrameWidth() { return m_frameWidth; }
    inline int getFrameHeight() { return m_frameHeight; }
    inline int getNumFrames() { return m_numFrames; }

   private:
    std::string m_filename;
    int m_frameWidth;
    int m_frameHeight;
    int m_numFrames;
};

}  // namespace parser
}  // namespace ctb

#endif
