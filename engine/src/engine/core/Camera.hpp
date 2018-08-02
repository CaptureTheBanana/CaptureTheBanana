// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_CORE_CAMERA_HPP
#define ENGINE_CORE_CAMERA_HPP

#include "engine/util/Vector2d.hpp"

namespace ctb {
namespace engine {

class Player;
class Door;
class TextureBasedRenderable;

/**
 * @brief A camera object to scroll within a level
 */
class Camera {
   public:
    /**
     * Constructs a camera with the given pixel offsets
     */
    explicit Camera(int x = 0,
                    int y = 0,
                    int w = 0,
                    int h = 0,
                    int levelWidth = 0,
                    int levelHeight = 0);

    /**
     * Moves the camera by the given amount of pixels in x-direction. Positive
     * values indicate movement to the right, negative values
     * move the camera to the left
     */
    void moveX(int pixels) { m_position -= Vector2dT(pixels, 0); }

    /**
     * Moves the camera by the given amount of pixels in y-direction. Positive
     * values indicate downward movement, negative value indicate upward movement
     */
    void moveY(int pixels) { m_position -= Vector2dT(0, pixels); }

    /// return the current camera position
    Vector2dT getPosition() const { return {x(), y()}; }

    /// set the current camera position
    void setPosition(int x, int y) { m_position = Vector2dT(x, y); }

    /// Returns the current x-position
    int x() const;

    /// Returns the current y-position
    int y() const;

    /// get width
    int width() const { return m_width; }

    /// get height
    int height() const { return m_height; }

    /// Sets an actor to follow
    void setFocus(TextureBasedRenderable* r) { m_focus = r; }

    /**
     * @brief   Focus the door d, taking in account the team of the door via an offset in the right
     * direction
     *
     * @param d The door to focus
     */
    void focusDoor(Door* d);

    /// returns leftmost x-coord visible in window
    int minX() {
        int halfWidth = static_cast<int>((m_width + 0.5) / 2);
        return this->x() - halfWidth;
    }

    /// returns topmost x-coord visible in window
    int minY() {
        int halfHeight = static_cast<int>((m_height + 0.5) / 2);
        return this->y() - halfHeight;
    }

    /// returns rightmost x-coord visible in window
    int maxX() {
        int halfWidth = static_cast<int>((m_width + 0.5) / 2);
        return this->x() + halfWidth;
    }

    /// returns bottommost x-coord visible in window
    int maxY() {
        int halfHeight = static_cast<int>((m_height + 0.5) / 2);
        return this->y() + halfHeight;
    }

    /**
     * @brief checks if the given renderable is out of the bounds of this camera
     *
     * @param r     the renderable to check
     *
     * @return      if r is out of this bounds or not
     */
    bool checkBounds(TextureBasedRenderable* r);

    /// Destructor
    virtual ~Camera() = default;

   private:
    /**
     * @brief limit val between min and max
     *
     * @param val   the value to limit
     * @param min   the minimum the value should be
     * @param max   the maximum the value should be
     *
     * @return      val if min < val < max,
     *              min if val < min,
     *              max if val > max
     */
    int limitValue(int val, int min, int max) const;

    /// Position of the camera
    Vector2dT m_position;

    /// An player to follow
    TextureBasedRenderable* m_focus;

    /// Height of the associated window
    int m_width;

    /// Width of the associated window
    int m_height;

    /// the width of the level this camera belongs to; needed for keeping the camera in level bounds
    int m_levelWidth;

    /// the height of the underlying level
    int m_levelHeight;
};

}  // namespace engine
}  // namespace ctb

#endif  // ENGINE_CORE_CAMERA_HPP
