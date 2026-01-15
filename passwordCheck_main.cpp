#include <iostream>
#include <string>
#include <print>
#include "passwordChecker.h"

/*
 * Password Checker
 *
 * Rules:
 * - Length must be between 8 and 16 characters
 * - Must contain at least:
 *     • 1 uppercase letter
 *     • 1 lowercase letter
 *     • 1 digit
 *     • 1 special character from: ! # @ $ % &
 *
 * Disallowed Patterns:
 * - Common passwords or substrings
 *     (e.g., "password", "qwerty", "admin", "letmein", "123456")
 * - Sequential characters (abc, cba, 123, 321)
 * - Excessive repetition (3 or more of the same character in a row)
 * - Whitespace characters
 *
 * Validity:
 * - A password is VALID only if all rules are satisfied
 *
 * Strength Rating (0–10):
 * - Strong (9–10): Meets all requirements with good complexity
 * - Medium (6–8): Meets most requirements but lacks complexity
 * - Weak (0–5): Fails rules or contains risky patterns
 */


static const char* strength_to_string(Strength s) {
    switch (s) {
        case Strength::Weak:   return "Weak";
        case Strength::Medium: return "Medium";
        case Strength::Strong: return "Strong";
    }
    return "Unknown";
}

int main() {
    std::string passwordString;

    std::print("Enter password: ");
    std::getline(std::cin, passwordString);

    passwordAnalysis a = analyze(passwordString);
    StrengthResult sr = rate_strength(a);

    std::println("\nPassword Analysis:");
    std::println("Length: {}", a.length);
    std::println("Min Length (>=8): {}", a.min_length ? "Yes" : "No");
    std::println("Max Length (<=16): {}", a.max_length ? "Yes" : "No");
    std::println("Has Uppercase: {}", a.uppercase ? "Yes" : "No");
    std::println("Has Lowercase: {}", a.lowercase ? "Yes" : "No");
    std::println("Has Digit: {}", a.digit ? "Yes" : "No");
    std::println("Has Special (!#@$%&): {}", a.special_char ? "Yes" : "No");

    std::println("Has Whitespace: {}", a.whitespace ? "Yes" : "No");
    std::println("Has Sequence (abc/123): {}", a.sequence ? "Yes" : "No");
    std::println("Has Common Pattern: {}", a.common_pattern ? "Yes" : "No");
    std::println("Max Repeat Run: {}", a.repeat_run);

    std::println("\nOverall Validity: {}", a.is_valid ? "VALID" : "INVALID");
    std::println("Strength: {} (score: {}/10)", strength_to_string(sr.strength), sr.score);

    return 0;
}
