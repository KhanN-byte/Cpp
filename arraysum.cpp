//
// Created by haris on 12/23/2025.
// two sum

#include "arraysum.h"
#include <iostream>
#include <vector>

class Solution {
public:
    std::vector<int> twoSum(std::vector<int>& nums, int target) {
        for (int i = 0; i < nums.size(); i++) {
            for (int j = i + 1; j < nums.size(); j++) {
                if (nums[i] + nums[j] == target) {
                    return {i, j};
                }
            }
        }
        return {};
    }
};

int main() {
    Solution test;

    int n;
    std::cout << "Enter size of array: ";
    std::cin >> n;

    std::vector<int> nums(n);
    std::cout << "Enter " << n << " integers: ";
    for (int i = 0; i < n; i++) {
        std::cin >> nums[i];
    }

    int target;
    std::cout << "Enter the target: ";
    std::cin >> target;

    std::vector<int> results = test.twoSum(nums, target);

    if (!results.empty()) {
        std::cout << "Indexes are: "
                  << results[0] << " and " << results[1] << std::endl;
    } else {
        std::cout << "No two numbers add up to the target." << std::endl;
    }

    return 0;
}
