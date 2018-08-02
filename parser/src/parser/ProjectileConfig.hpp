// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef PARSER_PROJECTILECONFIG_HPP
#define PARSER_PROJECTILECONFIG_HPP

#include <string>

namespace ctb {
namespace parser {

/// class containing all infos about a projectile
class ProjectileConfig {
   public:
    ProjectileConfig(std::string filename, std::string name)
        : m_filename(std::move(filename)), m_name(std::move(name)) {}

    inline std::string getFilename() { return m_filename; }
    inline std::string getName() { return m_name; }

   private:
    std::string m_filename;
    std::string m_name;
};

}  // namespace parser
}  // namespace ctb

#endif
