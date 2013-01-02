csv_iterator
===


Example:
---

    #include "csv_iterator.hpp"
    #include <ostream>
    #include <fstream>
    #include <string>
    #include <vector>

    struct Wire {int a; std::string b; int c};
    std::ostream& operator<< (std::ostream& os, const Wire& w) {
      return os << w.a << " " << w.b << " " << w.c;
    }
    struct WireFactory {
      Wire operator () (const std::vector<std::string>& data) {
        return Wire {stoi(data[0]),data[1],stoi(data[2])};
      }
    };

    int main() {
      typedev csv::iterator<Wire,WireFactory> WireIter;

      std::ifstream in("test/resource/mixed.csv");
      WireIter it(in, WireFactory());
      std::copy (it, WireIter(), std::ostream_iterator<Wire>(std::cout, "\n"));
  }
 
