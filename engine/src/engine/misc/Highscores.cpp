// This file is part of CaptureTheBanana++.
//
// Copyright (c) 2018 the CaptureTheBanana++ contributors (see CONTRIBUTORS.md)
// This file is licensed under the MIT license; see LICENSE file in the root of this
// project for details.

#include <algorithm>
#include <fstream>
#include <sstream>
#include <utility>

#include <boost/filesystem.hpp>

#include "engine/misc/Highscores.hpp"

using namespace boost::filesystem;

namespace ctb {
namespace engine {

std::vector<std::pair<std::string, uint64_t>> Highscores::highscores;

// 10 Scores should be enough
uint32_t Highscores::MAX_HIGHSCORES = 10;

// The format of the highscore file:
// Every entry consists of two lines. The first line is the name of the highscore owner.
// The second line is tha actual score. We have to do this with seperate lines, because
// the name is a sting and may contains numbers. During parsing the file, the name length
// is cut to 16 characters.
std::string Highscores::m_highscorePath;

void Highscores::init(const std::string& highscorePath) {
    m_highscorePath = highscorePath;
    path p(highscorePath);
    // Load the file, if it ok
    if (exists(p) && is_regular_file(p)) {
        std::ifstream infile(p.string());
        std::string line, name;
        int64_t score;
        bool first = true;
        // read alternating the name and the score. first=true is for the name, false
        // for the score
        while (std::getline(infile, line)) {
            if (first) {
                name = line;
                // Cut name to 16 chars.
                if (name.length() > 16) {
                    name.erase(16, name.length());
                }
            } else {
                std::istringstream ss(line);
                ss >> score;
                // Just take valid scores. May be negative..
                // If the score is wrong, skip this entry.
                if (score > 0) {
                    highscores.emplace_back(name, static_cast<uint64_t>(score));
                }
            }
            first = !first;
        }
        infile.close();
        sortScores();
    }
}

void Highscores::sortScores() {
    std::sort(highscores.begin(), highscores.end(),
              [](auto& left, auto& right) { return left.second > right.second; });
}

std::vector<std::pair<std::string, uint64_t>>& Highscores::getScores() {
    return highscores;
}

uint32_t Highscores::isHighscore(uint64_t score) {
    if (score == 0) {
        return 0;
    }
    // Check if it can be inserted into the highscores.
    unsigned int place = 1;
    for (auto& entry : highscores) {
        if (score > entry.second) {
            return place;
        }
        place++;
    }
    // Couldn't be inserted. But the list may has less then MAX entries..
    if (place <= MAX_HIGHSCORES) {
        return place;
    }
    // No new highscore :(
    return 0;
}

void Highscores::insert(const std::string& name, const uint64_t score) {
    // check, if the score is valied
    if (isHighscore(score)) {
        // Add the score
        highscores.emplace_back(name, score);
        sortScores();
        // if now there are more then max entries in highscores remove the
        // last one, because this person has the lowest score
        if (highscores.size() > MAX_HIGHSCORES) {
            highscores.pop_back();
        }
        save();
    }
}

std::string Highscores::formatScore(const uint64_t score, const uint32_t padding) {
    // convert the score to string. Calc the difference between the length
    // and padding, so this number of zeros hav to be added.
    std::string s = std::to_string(score);
    int diff = static_cast<int>(padding) - static_cast<int>(s.length());
    for (; diff > 0; diff--) {
        // s = "0" + s;
        s.insert(0, 1, '0');
    }
    return s;
}

void Highscores::save() {
    std::ofstream file(m_highscorePath, std::ios_base::trunc);

    if (file.is_open()) {
        for (auto& entry : highscores) {
            file << entry.first << std::endl;
            file << entry.second << std::endl;
        }
    }
    file.close();
}

}  // namespace engine
}  // namespace ctb
