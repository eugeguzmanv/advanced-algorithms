//Integrated activity 1
//This code is property of Diego Alberto Carrillo Castro A01645484 & Eugenio Guzman Vargas A01621084


#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <utility> 

using namespace std;

// KMP Substring Search 

//Longest preffix suffix table
void lpsTable(const string &pat, vector<int> &lps) {
    if (pat.empty()) return;
    int len = 0;
    lps[0] = 0;
    int i = 1;
    while (i < pat.size()) {
        if (pat[i] == pat[len]) {
            len++;
            lps[i] = len;
            i++;
        } else {
            if (len != 0) {
                len = lps[len - 1];
            } else {
                lps[i] = 0;
                i++;
            }
        }
    }
}

// Main KMP function, returns the 0-based index of the first match, or -1 if not found
int KMPSearch(const string &txt, const string &pat) {
    if (pat.empty()) return 0;
    int n = txt.size();
    int m = pat.size();
    if (m > n) return -1;

    vector<int> lps(m);
    lpsTable(pat, lps);

    int i = 0; // index for txt
    int j = 0; // index for pat
    while (i < n) {
        if (txt[i] == pat[j]) {
            i++;
            j++;
        }
        if (j == m) {
            return i - j; // Match found, return start index
        } else if (i < n && txt[i] != pat[j]) {
            if (j != 0) {
                j = lps[j - 1];
            } else {
                i++;
            }
        }
    }
    return -1; // No match found
}

// Manacher's Algorithm  to find the longest palindrome 

// Preprocessing: adding sentinels to hanle even and odd lenghts
string preprocess(const string &s) {
    string t = "^#"; 
    for (char c : s) {
        t += c;
        t += "#";
    }
    t += "$";
    return t;
}

// Main Manacher Algorithm implementation
pair<int, int> longestPalindrome(const string &s) {
    if (s.empty()) return {0, 0};
    string T = preprocess(s);
    int n = T.length();
    vector<int> P(n, 0);
    int C = 0, R = 0;

    for (int i = 1; i < n - 1; i++) {
        int mirr = 2 * C - i;
        if (i < R) {
            P[i] = min(R - i, P[mirr]);
        }
        while (T[i + (1 + P[i])] == T[i - (1 + P[i])]) {
            P[i]++;
        }
        if (i + P[i] > R) {
            C = i;
            R = i + P[i];
        }
    }

    int maxLen = 0;
    int centerIndex = 0;
    for (int i = 1; i < n - 1; i++) {
        if (P[i] > maxLen) {
            maxLen = P[i];
            centerIndex = i;
        }
    }

    int start = (centerIndex - maxLen) / 2;
    return {start + 1, start + maxLen};
}

// Longest Common Substring using DP
pair<int, int> longestCommonSubstring(const string &A, const string &B) {
    int n = A.size(), m = B.size();
    if (n == 0 || m == 0) return {0, 0};
    vector<vector<int>> dp(n + 1, vector<int>(m + 1, 0));
    int maxLen = 0;
    int endIndexA = 0;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (A[i - 1] == B[j - 1]) {
                dp[i][j] = dp[i - 1][j - 1] + 1;
                if (dp[i][j] > maxLen) {
                    maxLen = dp[i][j];
                    endIndexA = i - 1; // 0-based end index in A
                }
            } else {
                dp[i][j] = 0;
            }
        }
    }
    if (maxLen == 0) return {0, 0};
    int startIndexA = endIndexA - maxLen + 1;
    return {startIndexA + 1, endIndexA + 1};
}



//  File Reading and Main Logic 
string readFileToString(const string& filePath) {
    ifstream file(filePath);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filePath << endl;
        return "";
    }
    stringstream buffer;
    buffer << file.rdbuf();
    string file_content = buffer.str();
    file_content.erase(remove(file_content.begin(), file_content.end(), '\n'), file_content.end());
    file_content.erase(remove(file_content.begin(), file_content.end(), '\r'), file_content.end());
    return file_content;
}

int main() {
    // Read files
    string t1 = readFileToString("transmission1.txt");
    string t2 = readFileToString("transmission2.txt");
    string m1 = readFileToString("mcode1.txt");
    string m2 = readFileToString("mcode2.txt");
    string m3 = readFileToString("mcode3.txt");

    //  Part 1 Output 
    int pos = KMPSearch(t1, m1);
    cout << (pos != -1 ? "true " + to_string(pos + 1) : "false") << endl;
    pos = KMPSearch(t1, m2);
    cout << (pos != -1 ? "true " + to_string(pos + 1) : "false") << endl;
    pos = KMPSearch(t1, m3);
    cout << (pos != -1 ? "true " + to_string(pos + 1) : "false") << endl;
    pos = KMPSearch(t2, m1);
    cout << (pos != -1 ? "true " + to_string(pos + 1) : "false") << endl;
    pos = KMPSearch(t2, m2);
    cout << (pos != -1 ? "true " + to_string(pos + 1) : "false") << endl;
    pos = KMPSearch(t2, m3);
    cout << (pos != -1 ? "true " + to_string(pos + 1) : "false") << endl;

    //  Part 2 Output 
    pair<int, int> p1_coords = longestPalindrome(t1);
    cout << p1_coords.first << " " << p1_coords.second << endl;
    pair<int, int> p2_coords = longestPalindrome(t2);
    cout << p2_coords.first << " " << p2_coords.second << endl;

    //  Part 3 Output 
    pair<int, int> lcs_coords = longestCommonSubstring(t1, t2);
    cout << lcs_coords.first << " " << lcs_coords.second << endl;

    return 0;
}