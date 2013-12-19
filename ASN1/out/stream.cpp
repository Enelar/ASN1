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

  for (auto ent : out)
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
    for (const auto entity : orig.out)
      (*this) << *entity;
  return *this;
}

stream &stream::operator=(stream &&orig)
{
  throw_assert(this != &orig);
  std::swap(out, orig.out); MAYBEWRONG // out.swap(orig.out)
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
    std::swap(tvec, out); MAYBEWRONG // out.swap(orig.out)
    std::copy(tvec.begin() + 1, tvec.end(), out.begin());
    return tvec.front();
  };
  ent = VectorCrapCodePopFront();
  return *this;
}

stream::~stream()
{
  for (auto ent : out)
    delete ent;
}