// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef PARSER_BACKGROUNDCONFIG_HPP
#define PARSER_BACKGROUNDCONFIG_HPP

#include <string>
#include <utility>

namespace ctb {
namespace parser {

/// \brief a background layer of the level
class BackgroundConfig {
   public:
    /// \param layer determines when the Background layer gets rendered
    /// \param scrollSpeed speed of scrolling relative to camera Movement (in %)
    /// \param imageFilename filename of background image
    /// \param absolute path to parent directory of level.xml
    BackgroundConfig(int layer,
                     int scrollSpeed,
                     std::string imageFilename,
                     std::string absolutePath)
        : m_layer(layer),
          m_scrollSpeed(scrollSpeed),
          m_imageFilename(std::move(imageFilename)),
          m_absolutePath(std::move(absolutePath)) {}

    /// \brief sets layer
    ///
    /// \param id new layer
    inline void setLayer(int layer) { m_layer = layer; }

    /// \brief delivers layer that determines when this background gets rendered
    ///
    /// \return int layer
    inline int getLayer() const { return m_layer; }

    /// \brief sets scroll Speed relative to camera Position in %
    ///
    /// \param scrollSpeed new scrollSpeed in %
    inline void setScrollSpeed(int scrollSpeed) { m_scrollSpeed = scrollSpeed; }

    /// \brief delivers the scroll Speed in %
    ///
    /// \return int scrollSpeed
    inline int getScrollSpeed() const { return m_scrollSpeed; }

    /// \brief sets background image
    ///
    /// \param imageFilename filename of the new backgrounmd image
    inline void setImage(std::string imageFilename) { m_imageFilename = imageFilename; }

    /// \brief delivers the filename of the backgtound image
    ///
    /// \return std::string image Filename
    inline std::string getImageFilename() const { return m_absolutePath + m_imageFilename; }

    /// \brief delivers the filename of the backgtound image
    ///
    /// \return std::string image Filename
    inline std::string getRelativeImageFilename() const { return m_imageFilename; }

    virtual ~BackgroundConfig() = default;

   private:
    /// defines time when layer gets rendered
    int m_layer;
    /// speed of scrolling relative to camera speed in %
    int m_scrollSpeed;
    /// background image
    std::string m_imageFilename;
    /// absolute path to parent directory of level.xml
    std::string m_absolutePath{""};
};

}  // namespace parser
}  // namespace ctb

#endif  // PARSER_BACKGROUNDCONFIG_HPP
