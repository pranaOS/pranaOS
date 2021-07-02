#ifndef __COMMON_HPP__
#define __COMMON_HPP__

#include "error_code.hpp"
#include "reserved_code.hpp"

#include <functional>

namespace jklim
{

// Init, Release is functor
template<class Init, class Release>
struct atlast
{
  atlast(Init const& i, Release const& r)
    : _init(i)
    , _release(r)
  {
    _init();
  }
  ~atlast()
  {
    _release();
  }
private:
  Init _init;
  Release _release;
};

using atlast_easy = atlast<std::function<void()>, std::function<void()>>;

// singleton
template<class T>
class singleton
{
public:
  static T& GetInstance()
  {
    static T instance;
    return instance;
  }
protected:
  singleton()
  {
  }
  virtual~singleton()
  {
  }
};

} // namespace

#endif // __COMMON_HPP__