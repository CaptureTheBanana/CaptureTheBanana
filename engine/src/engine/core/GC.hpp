// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_CORE_GC_HPP
#define ENGINE_CORE_GC_HPP

#include <stack>

namespace ctb {
namespace engine {

class Object;

class GC {
   public:
    /// "Internal garbage collector"
    /// Add any pointer and the object will be deleted in the next
    /// run of the main loop
    static void add(Object* item);

    static void execute();

   private:
    /// The stack for the GC
    static std::stack<Object*> GC_stack;
};

}  // namespace engine
}  // namespace ctb
#endif
