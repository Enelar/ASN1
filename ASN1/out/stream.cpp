#include "stream.h"

using namespace asn1;

stream::stream()
{

}

stream::stream(string parse)
: stream([parse]()
{
  stream ret;
  ret << parse;
  return ret;
}())
{
}

stream::stream(const stream &orig)
{
  (*this) = orig;
}

stream::stream(stream &&orig)
{
  (*this) = orig;
}

#include <sstream>

stream::operator string() const
{
  std::stringstream stream;
  for each (auto ent  in out)
    stream << convert<string>(*ent);
  return stream.str();
}

stream &stream::operator<<(entity *element)
{
  out.push_back(element);
  return *this;
}

stream &stream::operator<<(const entity &element)
{
  return (*this) << convert<string>(element);
}

stream &stream::operator<<(const string &parse)
{
  return (*this) << NEW raw(parse);
}

stream &stream::operator=(const stream &orig)
{
  if (this != &orig)
    for each (const auto entity in orig.out)
      (*this) << *entity;
  return *this;
}

stream &stream::operator=(stream &&orig)
{
  throw_assert(this != &orig);
  // maybe wrong, and use out.swap(orig.out)
  std::swap(out, orig.out);
  return *this;
}

const vector<entity *> &stream::Parse() const
{
  todo(Parse from string);
}

stream &stream::operator>>(entity *&ent)
{
  throw_sassert(!ent, "In case of possible errors please set pointer to null before call");
  auto VectorCrapCodePopFront = [&]()
  {
    decltype(out) tvec;
    std::swap(tvec, out);
    std::copy(tvec.begin() + 1, tvec.end(), out.begin());
    return tvec.front();
  };
  ent = VectorCrapCodePopFront();
  return *this;
}

stream::~stream()
{
  for each (auto ent in out)
    delete ent;
}