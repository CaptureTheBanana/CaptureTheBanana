// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_SCENE_EXCEPTIONS_HPP
#define ENGINE_SCENE_EXCEPTIONS_HPP

#include <string>

#include <common/Exceptions.hpp>

namespace ctb {
namespace engine {

class SdlException : public UserExceptionBaseT {
   public:
    explicit SdlException(std::string message, std::string sdlError) noexcept
        : m_message(std::move(message)),
          m_sdlError(std::move(sdlError)) {}

    ~SdlException() noexcept override {}

    std::string exceptionMessage() const override {
        return m_message + "\nSDL2 message: " + m_sdlError;
    }

   private:
    std::string m_message;
    std::string m_sdlError;
};

}  // namespace engine
}  // namespace ctb

#endif  // ENGINE_SCENE_EXCEPTIONS_HPP
