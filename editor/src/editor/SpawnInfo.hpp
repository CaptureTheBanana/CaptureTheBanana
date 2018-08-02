// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef SPAWNINFO_HPP
#define SPAWNINFO_HPP

#include <parser/DynamicTilestore.hpp>

namespace ctb {
namespace editor {

using SpawnInfoT = ctb::DynamicTilestore;

/// Spawn types
enum class SpawnType {
    kNone = 1 << 0,
    kFlag = 1 << 1,
    kDoor = 1 << 2,
    kPlayer = 1 << 3,
    kBot = 1 << 4,
    kWeapon = 1 << 5,
    kMark = 1 << 6
};

/// \brief Converts SpawnType to int.
///
/// \param type Spawn type
/// \return int Returns the converted integer value.
inline int fromSpawnType(SpawnType type) {
    return static_cast<int>(type);
}

/// \brief Converts int to SpawnType.
///
/// \param index Integer value
/// \return SpawnType Returns the converted spawn type.
inline SpawnType toSpawnType(int index) {
    return static_cast<SpawnType>(index);
}

/// \brief Determines whether the given type is set.
///
/// \param flags Flags to check
/// \param type Value
/// \return bool Returns true if type is set.
inline bool hasSpawnType(const SpawnType& flags, SpawnType type) {
    return (static_cast<uint32_t>(flags) & static_cast<uint32_t>(type)) ==
           static_cast<uint32_t>(type);
}

/// \brief Removes the given spawn from flags.
///
/// \param flags Flags
/// \param type Spawn type to remove
inline void removeSpawnType(SpawnType& flags, SpawnType type) {
    flags = static_cast<SpawnType>(static_cast<uint32_t>(flags) & ~static_cast<uint32_t>(type));
}

/// \brief Sets the given spawn type.
///
/// \param flags Flags
/// \param type Spawn type
inline void setSpawnType(SpawnType& flags, SpawnType type) {
    flags = static_cast<SpawnType>(static_cast<uint32_t>(flags) | static_cast<uint32_t>(type));
}

}  // namespace editor
}  // namespace ctb

#endif  // SPAWNINFO_HPP
