#include <fstream>
#include <string>
#include <iostream>
#include <cmath>

// Helper to perform mathematical floor division (handles negatives correctly)
long long floor_div(long long a, long long b) {
    long long res = a / b;
    // If there is a remainder and the result is negative, we need to subtract 1
    // to match mathematical floor logic (e.g., -10 / 100 should be -1, not 0)
    if (a % b < 0) res--; 
    return res;
}

int main() {
    long long dial = 50; // Use long long to prevent overflow on calculations
    std::ifstream file("input.txt");
    std::string str; 
    long long password = 0;

    while (std::getline(file, str))
    {   
        long long num = std::stoi(str.substr(1));
        
        if (str[0] == 'L') {
            // LEFT TURN LOGIC
            // We are moving from 'dial' down to 'dial - num'.
            // We want to count multiples of 100 in the range [dial - num, dial - 1].
            // (We subtract 1 from dial because we don't count the number we started on)
            
            long long start = dial - 1;
            long long end = dial - num;
            
            // Formula: floor(start/100) - floor((end - 1)/100)
            long long count = floor_div(start, 100) - floor_div(end - 1, 100);
            password += count;
            
            dial -= num;
        } else {
            // RIGHT TURN LOGIC
            // We are moving from 'dial' up to 'dial + num'.
            // We want to count multiples of 100 in the range [dial + 1, dial + num].
            // Integer division handles positive numbers automatically for this.
            
            password += (dial + num) / 100;
            dial += num;
        }

        // Normalize dial to be between 0-99 for the next step
        dial = (dial % 100 + 100) % 100;
    }
    
    std::cout << password << std::endl;
    return 0;
}