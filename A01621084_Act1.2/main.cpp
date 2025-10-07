//This code is property of Eugenio Guzman Vargas A01621084
//Activity 1.2 Hashing strings


#include <iostream>
#include <vector>
#include <string>
#include <fstream>

//Declaration of function to read each file and store the lines in a vector
std::vector<std::string> read_input_file(const std::string& filename) {
  std::vector<std::string> strings_from_file; //vector where we´ll store the string
  std::string line; 
  std::ifstream file_stream(filename);
  
  if (!file_stream.is_open()) {
      std::cerr << "Error: Could not open input file: " << filename << std::endl;
      // Checking correct file opening for debugging
      return strings_from_file; 
  }
  while (std::getline(file_stream, line)) {
    if (!line.empty() && line.back() == '\r') {
        line.pop_back(); //this fixes bug for files written from windows notepad
    }
    strings_from_file.push_back(line); //push back every single line
  }

  return strings_from_file;
};

unsigned long long horner_hash(const std::string& s, int p, int m) {
    unsigned long long current_hash = 0;
    for (char c : s){
    current_hash = (current_hash * p + c) % m;
    }
    return current_hash;
};

void write_output_file(const std::string& filename, const std::vector<unsigned long long>& hashes) {
    // create an output file stream object, opening the file
    std::ofstream output_file(filename);
  
    if (!output_file.is_open()) {
      std::cerr << "Error: Could not open output file: " << filename << std::endl;
      return; // checking output file for debugging
  }

    // Iterate the vector containing the hashes
    for (unsigned long long hash_val : hashes) {
        // Write the hash value to the file, followed by a newline
        output_file << hash_val << '\n';
    }
    
}

int main() {
    std::string input_file = "input_strings.txt";
    std::string output_file = "output_hashes.txt";

    std::vector<std::string> strings = read_input_file(input_file);
    int p = 31;
    int m = 1000000009;
    std::vector<unsigned long long> hash_values;

    for (const std::string& str : strings) {
        hash_values.push_back(horner_hash(str, p, m));
    }

    write_output_file(output_file, hash_values);

    return 0;
}

/*
* This code is correct and adequate because it provides a complete 
*and well-structured solution for hashing strings from a file and 
*writing the hashes to another file.

* - Robust File Inputting and Outputting: The 'read_input_file' and 'write_output_file' functions
* are  check if the files are successfully opened and handle potential
* errors by printing a message to. This is a crucial best practice for file handling.

* - Efficient Hashing Algorithm:  The 'horner_hash' function correctly
* implements Horner's method, which is a standard and efficient algorithm
* for polynomial string hashing. It iterates through the string once, performing
* a constant number of operations per character. The use of a prime number 'p'
* and a large prime modulus 'm' is a good choice to minimize hash collisions. The complexity of this function of O(n)

* - Clear and Modular Design: The code is broken down into separate, well-defined functions
* This makes the code easy to read, understand, and maintain. The 'main' function
* acts as the orchestrator, calling these functions in a logical sequence to
* perform the complete task.

* - Handling of Newlines: The 'read_input_file' function includes a line
* 'if (!line.empty() && line.back() == '\r') { line.pop_back(); }''. This
* handles the carriage return character (`\r`) that can be present in files created on Windows
* ensuring the code works correctly across different operating systems.
*/
