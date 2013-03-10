//                     Copyright Nicholas Palko 2012
//      Distributed under the Boost Software License, Version 1.0.
//          (See accompanying file LICENSE_1_0.txt or copy at
//                 http://www.boost.org/LICENSE_1_0.txt)

// http://tools.ietf.org/html/rfc4180

#ifndef CSV_ITERATOR_HPP
#define CSV_ITERATOR_HPP

#include <functional>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>


namespace csv {

template<typename T>
class iterator : public std::iterator<
          std::input_iterator_tag,
          T,
          std::size_t,
          T*,
          T&
          > {
 public:
  typedef std::function<T (std::vector<std::string> const&)> Factory;
  
  iterator () : is_(nullptr) {}
  iterator (std::istream& is, Factory factory, char delim = ',', int skip = 0)
      : is_(&is),
        factory_(factory),
        delim_(delim),
        skip_(skip),
        skipped_(0)
  {
    ++(*this);
  }

  const T& operator*()  const { return current_; }

  const T* operator->() const { return current_; }
  
  bool operator== (iterator const& rhs)
  {
    return (is_ == nullptr) && (rhs.is_ == nullptr);
  }

  bool operator!=(iterator const& rhs) { return !((*this) == rhs); }

  iterator& operator++ ()
  {
    if (is_ == nullptr) {
      return *this;
    }
    if (!is_->good()) {
      is_ = nullptr;
      return *this;
    }
    
    std::string line, field;
    std::vector<std::string> fields;
    
    // skip over file header 
    while (skipped_ < skip_) {
      std::getline(*is_, line);
      ++skipped_;
    }
    
    std::getline(*is_, line);
    std::stringstream linestream(line);
    while (std::getline(linestream, field, delim_)) {
      fields.push_back(field);
    }
    
    // in the case of an empty file
    if (fields.empty()) { 
      is_ = nullptr;
      return *this;
    }
    
    current_ = factory_(fields);
    return *this;
  }
 private:
  std::istream* is_;
  Factory factory_;
  char delim_;
  int skip_;
  int skipped_;
  T current_;
};

};

#endif