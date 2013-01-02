csv_iterator
===

Header only library that provides an iterator interface to CSV files.

Example:
---

    #include "csv_iterator.hpp"
    #include <iostream>
    #include <fstream>
    #include <string>
    #include <vector>

    struct Wire {int a; std::string b; int c};
    std::ostream& operator<< (std::ostream& os, const Wire& w) {
      return os << w.a << " " << w.b << " " << w.c;
    }

    int main(int argc, char** argv) {
      using std::string;
      using std::vector;
      using std::stoi;
  
      // provide a factory to construct the Wire object from the component 
      // fields of the CSV file. 
  
      auto factory = [] (vector<string> const& fields) {
        return Wire { stoi(data[0]), data[1], stoi(data[2]) };
      };
  
      typedef csv::iterator<Wire> Iter;
      std::ifstream in ("test/mixed.csv");
      std::copy(Iter(in, factory),
                Iter(),
                std::ostream_iterator<Wire>(std::cout, "\n"));
    }
 
