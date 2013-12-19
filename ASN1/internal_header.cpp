#include "internal_header.h"

std::string ConCat(std::string &a, const std::string &b, const word b_length)
{
  auto new_b = Shift(b, 0, b.length() - b_length);
  return ConCat(a, new_b);
}


std::string ConCat(std::string &a, const std::string &b)
{
  std::stringstream stream;
  stream << a << b;
  return a = stream.str();
}


std::string Shift(std::string base, const word left, const word right)
{
  std::string ret;

  word new_length = base.length() - right - left;
  ret.reserve(new_length);

  for (word i = 0; i < new_length; i++)
    ret += base[i + left];
  return ret;
}