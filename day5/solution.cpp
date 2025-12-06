#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <queue>

using large = unsigned long long;

int main() {
   std::ifstream file("input.txt");
   std::string line;

   // Construct intervals
   std::map<large, large> intervalEnd;
   std::priority_queue<large, std::vector<large>, std::greater<large>> intervalStart;
   while (std::getline(file, line)) {
       if (line.empty()) break;
       char delimiter = line.find('-');
       large start = std::stoull(line.substr(0, delimiter));
       large end = std::stoull(line.substr(delimiter + 1));
       if (intervalEnd.count(start)) {
           intervalEnd[start] = std::max(intervalEnd[start], end);
       } else {
           intervalEnd[start] = end;
           intervalStart.push(start);
       }
   }

   // Merge intervals
   large consideredFresh = 0;
   std::map<large, large> mergedEnds;
   // could avoid using priority queue here as it is already sorted
   std::priority_queue<large, std::vector<large>, std::greater<large>> mergedStarts; 
   while (!intervalStart.empty()) {
       large start = intervalStart.top();
       intervalStart.pop();
       large end = intervalEnd[start];
       while (!intervalStart.empty() && intervalStart.top() <= end) {
           end = std::max(end, intervalEnd[intervalStart.top()]);
           intervalStart.pop();
       }
       mergedStarts.push(start);
       mergedEnds[start] = end;
       consideredFresh += (end - start + 1);
   }

   // Check inclusion
   std::priority_queue<large, std::vector<large>, std::greater<large>> queries;
   while (std::getline(file, line)) {
       if (line.empty()) break;
       large query = std::stoull(line);
       queries.push(query);
   }

   // Count fresh
   int fresh = 0;
   large start = mergedStarts.top();
   mergedStarts.pop();
   while (!queries.empty()) {
        large query = queries.top();
        std::cout << "Checking query: " << query << " at interval starting at: " << start << std::endl;
        queries.pop();
        if (query < start) continue;
        while (!mergedStarts.empty() && mergedEnds[start] < query) {
            start = mergedStarts.top();
            mergedStarts.pop();
        }
        std::cout << "Current interval: [" << start << ", " << mergedEnds[start] << "]" << std::endl;
        if (query >= start && query <= mergedEnds[start]) {
            std::cout << "Query " << query << " is fresh." << std::endl;
            fresh++;
        }
   }

   std::cout << "Number of fresh ingredients: " << fresh << std::endl;
   std::cout << "Total considered freshness value: " << consideredFresh << std::endl;

   return 0;
}