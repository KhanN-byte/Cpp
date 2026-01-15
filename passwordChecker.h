// passwordChecker.h
#ifndef DESKTOP_PASSWORDCHECKER_H
#define DESKTOP_PASSWORDCHECKER_H

#include <string>
#include <cstddef>

struct passwordAnalysis {
    std::size_t length = 0;

    bool min_length = false;     // >= 8
    bool max_length = false;     // <= 16 (per your original spec)
    bool uppercase = false;
    bool lowercase = false;
    bool digit = false;
    bool special_char = false;

    bool whitespace = false;
    bool sequence = false;
    bool common_pattern = false;
    std::size_t repeat_run = 0;

    bool is_valid = false;
};

// Simple strength output
enum class Strength { Weak, Medium, Strong };

struct StrengthResult {
    int score = 0;                 // 0–10
    Strength strength = Strength::Weak;
};

passwordAnalysis analyze(const std::string& pw);
StrengthResult rate_strength(const passwordAnalysis& a);

#endif