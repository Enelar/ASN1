#include "boolean.h"
#include "../internal_header.h"

using namespace asn1::builtin_types;

boolean::operator std::string() const
{
  std::string ret;

  ConCat(ret, type);
  ConCat(ret, EncodeLength());
  entity::byte _content = 0;
  if (content)
    _content = 0xFF; // as explained in ISO:IEC 8825-1:2003, page 18
  ConCat(ret, std::string({ (char)_content }));
  return ret;
}

int boolean::Length() const
{
  const int SIZEOF_BOOL_ENCODED_IN_ASN = 1;
  return SIZEOF_BOOL_ENCODED_IN_ASN;
}

boolean::LENGTH_TYPE boolean::LengthType() const
{
  return LENGTH_TYPE::DEFINITE;
}