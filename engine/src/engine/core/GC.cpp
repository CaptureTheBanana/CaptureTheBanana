// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "engine/core/GC.hpp"
#include "engine/Object.hpp"

namespace ctb {
namespace engine {

std::stack<Object*> GC::GC_stack;

void GC::execute() {
    // Cleanup: delete all objects in the GC:
    while (!GC_stack.empty()) {
        Object* item = GC_stack.top();
        GC_stack.pop();
        delete item;
    }
}

void GC::add(Object* item) {
    GC_stack.push(item);
}

}  // namespace engine
}  // namespace ctb
