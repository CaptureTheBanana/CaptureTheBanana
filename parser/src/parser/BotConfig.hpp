// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef PARSER_BOTCONFIG_HPP
#define PARSER_BOTCONFIG_HPP

#include <string>

namespace ctb {
namespace parser {

/// Class containing all needed infos for constructing a bot
class BotConfig {
   public:
    BotConfig() = default;

    /// setter for m_filename
    inline void setFilename(std::string filename) { m_filename = filename; }

    /// setter for m_name
    inline void setName(std::string name) { m_name = name; }

    /// setter for m_numFrames
    inline void setNumFrames(int numFrames) { m_numFrames = numFrames; }

    /// setter for m_frameWidth
    inline void setFrameWidth(int frameWidth) { m_frameWidth = frameWidth; }

    /// setter for m_frameHeight
    inline void setFrameHeight(int frameHeight) { m_frameHeight = frameHeight; }

    /// getter for m_filename
    inline std::string getFilename() { return m_filename; }

    /// getter for m_name
    inline std::string getName() { return m_name; }

    /// getter for m_numFrames
    inline int getNumFrames() { return m_numFrames; }

    /// getter for m_frameWidth
    inline int getFrameWidth() { return m_frameWidth; }

    /// getter for m_frameHeight
    inline int getFrameHeight() { return m_frameHeight; }

   private:
    /// the path to the texture of the bot
    std::string m_filename;

    /// the name of the bot
    std::string m_name;

    /// number of frames
    int m_numFrames{0};

    /// frame width
    int m_frameWidth{0};

    /// frame height
    int m_frameHeight{0};
};

}  // namespace parser
}  // namespace ctb

#endif  // PARSER_BOTCONFIG_HPP
