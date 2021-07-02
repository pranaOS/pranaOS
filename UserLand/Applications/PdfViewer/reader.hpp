#ifndef __READER_HPP__
#define __READER_HPP__

#include <iostream>
#include <string>

namespace jklim
{

class reader
{
public:
  reader();
  reader(std::string const& _filepath);
  virtual ~reader();

public:
  int open(std::string const& _filepath);
  int close();

  // just test
  friend std::ostream& operator << (std::ostream& _os, reader const& _obj);

private:
  class implement;
  implement* impl;
};

} // namespace jklim

#endif // __READER_HPP__