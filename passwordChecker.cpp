
#include "passwordChecker.h"
#include <cctype>
#include <string>
#include <algorithm>

passwordAnalysis analyze(const std::string& pw) {
    passwordAnalysis result{}; // zero/false init

    result.length = pw.size();

    const std::string specials = "!#@$%&";

    // For repeats
    char prev = '\0';
    size_t current_run = 0;

    for (size_t i = 0; i < pw.size(); ++i) {
        char c = pw[i];
        unsigned char uc = static_cast<unsigned char>(c);

        // Whitespace
        if (std::isspace(uc)) {
            result.whitespace = true;
        }

        // Category checks
        if (std::isupper(uc)) {
            result.uppercase = true;
        } else if (std::islower(uc)) {
            result.lowercase = true;
        } else if (std::isdigit(uc)) {
            result.digit = true;
        } else if (specials.find(c) != std::string::npos) {
            result.special_char = true;
        }

        // Repeat run tracking
        if (i == 0) {
            current_run = 1;
        } else {
            current_run = (c == prev) ? (current_run + 1) : 1;
        }
        if (current_run > result.repeat_run) result.repeat_run = current_run;
        prev = c;

        // Simple 3-char sequence detection (abc / cba / 123 / 321)
        if (i >= 2 && !result.sequence) {
            unsigned char a = static_cast<unsigned char>(pw[i - 2]);
            unsigned char b = static_cast<unsigned char>(pw[i - 1]);
            unsigned char d = static_cast<unsigned char>(pw[i]);

            // digits: 123 or 321
            if (std::isdigit(a) && std::isdigit(b) && std::isdigit(d)) {
                if ((b == a + 1 && d == b + 1) || (b + 1 == a && d + 1 == b)) {
                    result.sequence = true;
                }
            }

            // letters: abc or cba (case-insensitive)
            unsigned char la = static_cast<unsigned char>(std::tolower(a));
            unsigned char lb = static_cast<unsigned char>(std::tolower(b));
            unsigned char ld = static_cast<unsigned char>(std::tolower(d));

            if (std::isalpha(la) && std::isalpha(lb) && std::isalpha(ld)) {
                if ((lb == la + 1 && ld == lb + 1) || (lb + 1 == la && ld + 1 == lb)) {
                    result.sequence = true;
                }
            }
        }
    }

    // Length rules
    result.min_length = (result.length >= 8);
    result.max_length = (result.length <= 21);

    // Simple common pattern check (case-insensitive substring)
    std::string lower_pw;
    lower_pw.reserve(pw.size());
    for (char c : pw) {
        lower_pw.push_back(static_cast<char>(std::tolower(static_cast<unsigned char>(c))));
    }

    // Keep this list small; expand later if you want
    if (lower_pw.find("password") != std::string::npos ||
        lower_pw.find("qwerty")   != std::string::npos ||
        lower_pw.find("admin")    != std::string::npos ||
        lower_pw.find("123456")   != std::string::npos ||
        lower_pw.find("letmein")  != std::string::npos) {
        result.common_pattern = true;
    }

    // Overall validity
    result.is_valid =
        result.min_length &&
        result.max_length &&
        result.uppercase &&
        result.lowercase &&
        result.digit &&
        result.special_char &&
        !result.whitespace &&
        !result.sequence &&
        (result.repeat_run < 3) &&
        !result.common_pattern;

    return result;
}

StrengthResult rate_strength(const passwordAnalysis& a) {
    // Keep it simple: gates + small scoring
    StrengthResult out{};

    // If length is bad, it's weak
    if (!a.min_length || !a.max_length) {
        out.score = 0;
        out.strength = Strength::Weak;
        return out;
    }

    int score = 0;

    // + points for categories (max 6)
    score += a.uppercase ? 1 : 0;
    score += a.lowercase ? 1 : 0;
    score += a.digit ? 1 : 0;
    score += a.special_char ? 2 : 0; // special counts more
    score += (a.length >= 12) ? 1 : 0;

    // - penalties
    if (a.whitespace) score -= 1;
    if (a.sequence) score -= 2;
    if (a.repeat_run >= 3) score -= 2;

    // common pattern => cap hard
    if (a.common_pattern) score = std::min(score, 3);

    out.score = std::clamp(score, 0, 10);

    if (out.score >= 9) out.strength = Strength::Strong;
    else if (out.score >= 6) out.strength = Strength::Medium;
    else out.strength = Strength::Weak;

    return out;
}