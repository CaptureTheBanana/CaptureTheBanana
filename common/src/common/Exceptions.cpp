// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include "common/Exceptions.hpp"

namespace ctb {
namespace common {

const char* UserExceptionBase::what() const noexcept {
    if (m_msg_.empty()) {
        try {
            exceptionMessage().swap(m_msg_);
        } catch (...) {
            return "Error: Cannot swap message.";
        }
    }
    return m_msg_.c_str();
}

}  // namespace common
}  // namespace ctb
