// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#ifndef ENGINE_MISC_HIGHSCORES_HPP
#define ENGINE_MISC_HIGHSCORES_HPP

#include <string>
#include <vector>

namespace ctb {
namespace engine {

/// Manage save and load of highscores
class Highscores {
   public:
    /// Static-only class:
    Highscores() = delete;

    Highscores(const Highscores&) = delete;

    Highscores& operator=(const Highscores&) = delete;

    /// init highscore system
    /// \param highscorePath The file for read and write highscores to
    static void init(const std::string& highscorePath);

    /// \brief Get all highscores
    /// \return sorted vector of highscore names paired with the actual score.
    static std::vector<std::pair<std::string, uint64_t>>& getScores();

    /// Check, if the given score could be inserted in the highscore list
    /// \return The position in the highscore list, if this score could be inserted
    ///         or 0 if this is no score.
    static uint32_t isHighscore(const uint64_t score);

    /// Inserts the score witht the associated name into the highscore list
    /// \param name The name of the lucky highscorer
    /// \param score The new score to insert
    static void insert(const std::string& name, const uint64_t score);

    /// \brief Unify formats highscores for the user. The score is padded to the length
    ///        given by padding with zeros. E.g. 329 will be "000392" with a padding of 6.
    ///        If the score has more digits then the padding, the score will just be
    ///        converted to string.
    /// \param score The score to format
    /// \param padding The length of the string, if the digits of score is less or equal
    ///        to padding.
    /// \return The score as a string padded with zeros.
    static std::string formatScore(const uint64_t score, const uint32_t padding = 6);

    /// Saves the list to the highscore file provided at init.
    static void save();

   private:
    /// The number of max highsores saved.
    static uint32_t MAX_HIGHSCORES;

    /// The Path of the highscore file.
    static std::string m_highscorePath;

    /// The actual highscores. This vector has always to be sorted!
    static std::vector<std::pair<std::string, uint64_t>> highscores;

    /// Helper function to sort the highscore vector. Should be called after all
    /// modifications of this vector.
    static void sortScores();
};

}  // namespace engine
}  // namespace ctb

#endif
