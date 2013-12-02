#include "raw.h"
#include "../../internal_header.h"

using namespace asn1;

raw::raw(std::string a)
: entity(inditifier(a))
{
  a = Shift(a, inditifier::OffsetToLength(a));
  if (length::IsIndefinite(a))
    length_type = LENGTH_TYPE::INDEFINITE;
  else
  {
    length_type = LENGTH_TYPE::DEFINITE;
    length = length::DecodeLength(a);
  }
  a = Shift(a, length::SizeofLength(a));
  if (length_type == LENGTH_TYPE::INDEFINITE)
  {
    _ASSERT(a[a.length() - 2] == 0); // end of contents flag
    _ASSERT(a[a.length() - 1] == 0); // zero length inditifier
    a = Shift(a, 0, 2);
  }
  content = a;
}

raw::operator std::string() const
{
  std::string ret = type;
  ConCat(ret, EncodeLength(Length()));
  return ConCat(ret, content);
}

int raw::Length() const
{
  return length;
}

raw::LENGTH_TYPE raw::LengthType() const
{
  return LENGTH_TYPE::DEFINITE;
}