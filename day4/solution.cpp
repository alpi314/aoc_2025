#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <queue>
#include <set>

// define type for generic 2d vector
template<typename T>
using vec2d = std::vector<std::vector<T>>;

void countAround(vec2d<bool>& rolls, vec2d<int>& counts, int row, int col) {
    for (int dr = -1; dr <= 1; dr++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (dr == 0 && dc == 0) continue;
            int r = row + dr;
            int c = col + dc;
            if (r >= 0 && r < rolls.size() && c >= 0 && c < rolls[0].size() && rolls[r][c]) {
                counts[row][col] += 1;
            }
        }
    }
}

int main() {
    std::ifstream file("input.txt");
    std::string bank; 
    
    vec2d<bool> rolls;
    while (std::getline(file, bank))
    {   
        std::vector<bool> roll;
        for (char c : bank) {
            roll.push_back(c == '@');
        }
        rolls.push_back(roll);
    }
    
    std::set<std::pair<int, int>> removed;
    std::queue <std::pair<int, int>> accessible;
    vec2d<int> accessCount(rolls.size(), std::vector<int>(rolls[0].size(), 0));
    for (int r = 0; r < rolls.size(); r++) {
        for (int c = 0; c < rolls[0].size(); c++) {
            if (!rolls[r][c]) continue;
            countAround(rolls, accessCount, r, c);
            if (accessCount[r][c] < 4) {
                accessible.push({r, c});
                removed.insert({r, c});
            }
        }
    }
    
    std::cout << "Directly accessible rolls: " << accessible.size() << std::endl;

    while (!accessible.empty()) {
        auto [r, c] = accessible.front();
        accessible.pop();
        for (int dr = -1; dr <= 1; dr++) {
            for (int dc = -1; dc <= 1; dc++) {
                if (dr == 0 && dc == 0) continue;
                int nr = r + dr;
                int nc = c + dc;
                if (nr >= 0 && nr < rolls.size() && nc >= 0 && nc < rolls[0].size() && rolls[nr][nc]) {
                    if (removed.find({nr, nc}) != removed.end()) continue;
                    accessCount[nr][nc]--;
                    if (accessCount[nr][nc] < 4) {
                        removed.insert({nr, nc});
                        accessible.push({nr, nc});
                    }
                }
            }
        }
    }

    std::cout << "Total accessible rolls after cascade: " << removed.size() << std::endl;

    return 0;

}