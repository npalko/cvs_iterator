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

/*

// getline -- could be early
// " ",  ,  " """, " \n

  "" '


 "" "


  push open quote on
  scan to next " -- if hit \n keep going
 

 return *this;

 
 



*/

namespace csv {
typedef std::function<T (std::vector<std::string> const&)> Factory;


template<typename T>
class parser {
 public:
  parser () :
  parser (std::istream& is, Factory factory, char delim, int skip)
      : is_(is),
        factory_(factory),
        delim_(delim),
        skip_(skip) {
  }
  bool operator== (parser const& rhs) const {
    
  }
  bool operator!= (parser const& rhs) const {
    return !((*this) == rhs);
  }
  
 T current () {
   return current_;
 }
 void next () {
 
 
   if (is_ == nullptr) {
     return *this;
   }
   if (!is_->good()) {
     is_ = nullptr;
     return *this;
   }
   
   std::string line, field;
   std::vector<std::string> fields;
   
   while (skipped_ < skip_) {    // skip over file header
     std::getline(*is_, line);
     ++skipped_;
   }
   
   std::getline (*is_, line);
   std::stringstream linestream(line);
   while (std::getline (linestream, field, delim_)) {
     fields.push_back (field);
   }
   
   if (fields.empty()) {         // in the case of an empty file
     is_ = nullptr;
     return *this;
   }
   
   current_ = factory_ (fields);
 }
 private:
  std::istream* is_;
  Factory factory_;
  char delim_;
  int skip_;
  int skipped_;
  T current_;
}



template<typename T>
class iterator : public std::iterator<
          std::input_iterator_tag,
          T,
          std::size_t,
          T*,
          T&
          > {
 public:
  iterator () : parser_(nullptr) {}
  iterator (std::istream& is, Factory factory, char delim = ',', int skip = 0)
      : parser_(new parser(is, factory, delim, skip)) {
    ++(*this);
  }
  const T& operator* () const {
    return parser_->current ();
  }
  const T* operator-> () const {
    return parser_->current ();
  }
  bool operator== (iterator const& rhs) {
    return parser_ == rhs.parser_;
  }
  bool operator!= (iterator const& rhs) {
    return !((*this) == rhs);
  }
  iterator& operator++ () {
    parser_->next ();
    return *this;
  }
 private:
  std::unique_ptr<parser<T>> parser_;
};

};

#endif