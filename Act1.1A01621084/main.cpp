//This code is property of Eugenio Guzmán Vargas A01621084
//Advanced algorithms: Activity 1.1 String searching

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <tuple>
using namespace std;

//Variation of Manachers algorithm to return the number of palindomres, the lenght of the longest palindomre, and the start position of the longest palindrome. Original Manacher implementation found in: https://www.naukri.com/code360/library/manacher-s-algorithm-3640


// Function returns: <int num. of palindormes, int lenght of longest, int start of longest>
tuple<int, int, int> manachersAlgorithm(const string& s) {
if (s.empty()) {
return make_tuple(0, 0, 0); //the input is empty 
}

    // Start with a character '^' to act as a left boundary.
string T = "^#"; 
for (char c : s) {
T += c;
T += "#"; // adding # after each character to be able to handle even and odd lenght of s
}
    // End with a character '$' to act as a right boundary.
    T += "$";

int n = T.length();
vector<int> P(n, 0); // n lenght, initialized in all 0s
int C = 0; // center of the current longest palindrome
int R = 0; // right boundary of the current longest palindrome
int maxLen = 0;
int centerIndex = 0;
int totalPalindromes = 0;

//Manacher´s algorithm logic
for (int i = 1; i < n - 1; ++i) {
int i_mirror = 2 * C - i; // mirror of i
if (R > i) {
P[i] = min(R - i, P[i_mirror]);
} else {
P[i] = 0;
}

// Attempt to expand palindrome centered at i (this is now safe)
while (T[i + 1 + P[i]] == T[i - 1 - P[i]]) {
P[i]++;
}

// If palindrome centered at i expands past R, adjust center and right boundary
if (i + P[i] > R) {
C = i;
R = i + P[i];
}

// Add the number of palindromes centered at the current position
// A palindrome of radius P[i] in the transformed string corresponds to (P[i] + 1) / 2 unique palindromic substrings in the original string.
totalPalindromes += (P[i] + 1) / 2;

// Update the longest palindrome found so far
if (P[i] > maxLen) {
maxLen = P[i];
centerIndex = i;
}
}

// Convert back to original string coordinates
int startPos = (centerIndex - maxLen) / 2;
return make_tuple(totalPalindromes, maxLen, startPos);
}


void printLongestPalindrome(const std::string& originalString, int startPos, int length) {
// Only print if a palindrome was found 
if (length > 0) {
std::cout << "Longest palindrome found: " << originalString.substr(startPos, length) << std::endl;
} else {
std::cout << "Longest palindrome found: (none)" << std::endl;
}
}

int main() {
// Array of file names to process
//To try different test cases, edit the content of the vector correspondingly to the names of the test files.
vector<string> fileNames = {"test01.txt", "test02.txt", "test03.txt"};


//Open, read, and parse each file
for (const auto& fileName : fileNames) {
ifstream inputFile(fileName);
string fileContent((istreambuf_iterator<char>(inputFile)), istreambuf_iterator<char>());
inputFile.close();
    
// Remove trailing whitespace (like '\n') from windows Notepad

while (!fileContent.empty() && isspace(fileContent.back())) {
        fileContent.pop_back();
    }
// Call our main function for each test case
tuple<int, int, int> results = manachersAlgorithm(fileContent);

// Print the results in the required format
cout << "Input: " << fileName << endl;
cout << "Number of palindromes: " << get<0>(results) << endl;
cout << "Length of the longest palindrome: " << get<1>(results) << endl;
cout << "Starting position of the longest palindrome: " << get<2>(results) << endl;
printLongestPalindrome(fileContent, std::get<2>(results), std::get<1>(results));
cout<<"*************************************"<<endl;

}

return 0;
}