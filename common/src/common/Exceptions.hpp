// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef COMMON_EXCEPTIONS_HPP
#define COMMON_EXCEPTIONS_HPP

#include <exception>
#include <stdexcept>
#include <string>
#include <utility>

namespace ctb {
namespace common {
/// Base user exception class.
class UserExceptionBase : public std::exception {
   public:
    UserExceptionBase() noexcept {}
    ~UserExceptionBase() noexcept override {}

    virtual std::string exceptionMessage() const = 0;
    const char* what() const noexcept override;

   private:
    mutable std::string m_msg_;
};

/// User runtime exception
class RuntimeException : public UserExceptionBase {
   public:
    explicit RuntimeException(std::string message) noexcept : m_message(std::move(message)) {}
    ~RuntimeException() noexcept override {}

    std::string exceptionMessage() const override { return m_message; }

   private:
    std::string m_message;
};

/// File not found exception
class FileNotFoundException : public UserExceptionBase {
   public:
    explicit FileNotFoundException(std::string path) noexcept : m_path(std::move(path)) {}
    ~FileNotFoundException() noexcept override {}

    std::string exceptionMessage() const override { return "File not found: " + m_path; }

   private:
    std::string m_path;
};

/// Base validator exception class.
class ValidatorException : public UserExceptionBase {};

/// Exception thrown when a invalid game.xml is detected.
class GameValidatorException final : public ValidatorException {
   public:
    explicit GameValidatorException(std::string message) noexcept : m_message(std::move(message)) {}
    ~GameValidatorException() noexcept override {}

    std::string exceptionMessage() const override { return m_message; }

   private:
    std::string m_message;
};

}  // namespace common

using UserExceptionBaseT = ctb::common::UserExceptionBase;
using RuntimeExceptionT = ctb::common::RuntimeException;
using ValidatorExceptionT = ctb::common::ValidatorException;
using GameValidatorExceptionT = ctb::common::GameValidatorException;

}  // namespace ctb

#endif  // COMMON_EXCEPTIONS_HPP
