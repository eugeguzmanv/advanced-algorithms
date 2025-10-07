//Eugenio Guzman Vargas A01621084
//

//Problem 1: Robot climbing stairs: 
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <iostream>
using namespace std;


//Problem 1: Anagrams
// Time complexity: O(n) where n = length of strings (single pass to build counts)
// Space complexity: O(1) (26-element map, constant extra space [could depend on the alphabet, but is still constant])
bool areAnagrams(string s, string t){ //receives two strings as input

    //if the length of both strings is not the same, they can't be anagrams
    if(s.size() != t.size()) return false;

    //hash map where all letters of the english alphabet map to a prime number
    unordered_map<char, int> prime_map = {
    {'a', 2}, {'b', 3}, {'c', 5}, {'d', 7}, {'e', 11},
    {'f', 13}, {'g', 17}, {'h', 19}, {'i', 23}, {'j', 29},
    {'k', 31}, {'l', 37}, {'m', 41}, {'n', 43}, {'o', 47},
    {'p', 53}, {'q', 59}, {'r', 61}, {'s', 67}, {'t', 71},
    {'u', 73}, {'v', 79}, {'w', 83}, {'x', 89}, {'y', 97},
    {'z', 101}
    };//we assume that the valid language are lowercase english letters. For a different language, simply modify this map.

    //iterate through both strings and get the product of the mapping of their characters
    long long product_s = 1;
    long long product_t = 1; 
    for(int i = 0; i < s.size(); i++){
        product_s = product_s * prime_map[s[i]];
        product_t = product_t * prime_map[t[i]];
    }; 

    return(product_s == product_t); //if the product is the same, the strings are anagrams


}

//Problem 2.1: Robot climibing stairs
// Time complexity: O(n) due to single loop up to n
// Space complexity: O(n) due to DP array
int climbingStairs(int n){ //recieves an integer as an input
    vector<int> arr(n + 1); //array of size n + 1, because of 0-indexing
    //Precomputing of base cases: 
    arr[0] = 0; 
    arr[1] = 1; 
    arr[2] = 2;
    arr[3] = 3; 

    if (n <= 3){ 
        return arr[n];
    };

    for(int i = 4; i < arr.size(); i++){ //iterate through the entire array
        arr[i] = arr[i - 1] + arr[i - 2];//DP memorization
    };
    return arr[n];

}

//Problem 2.2: Isomorphic strings
// Time complexity: O(n) where n = length of strings (single pass)
// Space complexity: O(1) (at most 26 mappings and a set for lowercase letters)
bool isomorphicStrings(string s, string t){
    if(s.size() != t.size()) return false;

    unordered_map<char,char> map;//hash map to map each character of s to one character of t
    unordered_set<char> used; //hash set to ensure no character in t is matched to more than one character in s 

    for(int i = 0; i < s.size(); i++){
        if(map.find(s[i]) != map.end()){ 
            if(map[s[i]] != t[i]) return false; //if the current character of s has already been mapped, and its value d
        } else{
            if(used.find(t[i]) != used.end()) return false;
            map[s[i]] = t[i];
            used.insert(t[i]);
        };
    }
    return true; 
}

//Test cases: 
int main(){
    // Test areAnagrams 
    cout << "areAnagrams('listen', 'silent') -> " << (areAnagrams("listen","silent") ? "true" : "false") << " (expected true)" << endl;
    cout << "areAnagrams('hello', 'bello') -> " << (areAnagrams("hello","bello") ? "true" : "false") << " (expected false)" << endl;
    cout << "areAnagrams('', '') -> " << (areAnagrams("","") ? "true" : "false") << " (expected true)" << endl; 
    cout << "areAnagrams('aabbcc', 'abcabc') -> " << (areAnagrams("aabbcc","abcabc") ? "true" : "false") << " (expected true)" << endl;

    // Test climbingStairs 
    cout << "climbingStairs(0) -> " << climbingStairs(0) << " (expected 1)" << endl;
    cout << "climbingStairs(1) -> " << climbingStairs(1) << " (expected 1)" << endl;
    cout << "climbingStairs(3) -> " << climbingStairs(3) << " (expected 3)" << endl;
    cout << "climbingStairs(10) -> " << climbingStairs(10) << " (expected 89)" << endl; 
    cout << "climbingStairs(20) -> " << climbingStairs(20) << " (expected 10946)" << endl;

    // Test isomorphicStrings 
    cout << "isomorphicStrings('aab','xxy') -> " << (isomorphicStrings("aab","xxy") ? "true" : "false") << " (expected true)" << endl;
    cout << "isomorphicStrings('aab','xyz') -> " << (isomorphicStrings("aab","xyz") ? "true" : "false") << " (expected false)" << endl;
    cout << "isomorphicStrings('paper','title') -> " << (isomorphicStrings("paper","title") ? "true" : "false") << " (expected true)" << endl;
    cout << "isomorphicStrings('foo','bar') -> " << (isomorphicStrings("foo","bar") ? "true" : "false") << " (expected false)" << endl;

    return 0;
}
