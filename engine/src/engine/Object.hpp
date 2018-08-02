// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_OBJECT_HPP
#define ENGINE_OBJECT_HPP

namespace ctb {
namespace engine {

/// Base object. Every class should derive from this. Just a marker interface, but usefull for the
/// GC.
class Object {
   public:
    // Funfact: If you remove this, you have a garbage creator rather then collector :D
    virtual ~Object() = default;
};
}  // namespace engine
}  // namespace ctb
#endif
