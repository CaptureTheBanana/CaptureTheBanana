// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef PARSER_WEAPONCONFIG_HPP
#define PARSER_WEAPONCONFIG_HPP

#include <string>
#include <utility>

namespace ctb {
namespace parser {

/// Class containing all infos needed for constructing a weapon
class WeaponConfig {
   public:
    WeaponConfig(std::string filename,
                 std::string name,
                 float range,
                 float as,
                 float damage,
                 std::string special,
                 std::string projectile)
        : m_filename(std::move(filename)),
          m_name(std::move(name)),
          m_range(range),
          m_attackspeed(as),
          m_damage(damage),
          m_special(std::move(special)),
          m_projectile(std::move(projectile)) {}

    inline std::string getFilename() { return m_filename; }
    inline std::string getName() { return m_name; }
    inline float getRange() { return m_range; }
    inline float getAttackspeed() { return m_attackspeed; }
    inline float getDamage() { return m_damage; }
    inline std::string getSpecial() { return m_special; }
    inline std::string getProjectile() { return m_projectile; }

   private:
    std::string m_filename;
    std::string m_name;
    float m_range;
    float m_attackspeed;
    float m_damage;
    std::string m_special;
    std::string m_projectile;
};

}  // namespace parser
}  // namespace ctb

#endif
