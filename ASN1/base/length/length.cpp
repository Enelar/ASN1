#include "length.h"
#include <vector>

using namespace asn1;

std::string length::EncodeIndifinite()
{
  std::string ret;
  ret.push_back(GETMAXCOUNT(long_flag_pos));
  return ret;
}

std::string length::EncodeLength(word length)
{
  auto ToString = [](const std::vector<ub> &a)
  {
    std::string t;
    for (ub ch : a)
      t.push_back(ch);
    return t;
  };
  if (length < 128)
    return ToString({ convert<ub>(length) });
  std::vector<ub> t;

  while (length != 0)
  {
    ub temp = length & GETMAXVALUE(length_content_in_octet);
    t.push_back(temp);
    length >>= length_content_in_octet;
  }
  std::reverse(t.begin(), t.end());
  return ToString(t);
}

word length::DecodeLength(std::string)
{
  todo(Decode Length);
}

word length::SizeofLength(const std::string &str)
{
  if (!(str[0] & GETMAXCOUNT(long_flag_pos)))
    return 1;

  ub length_size = str[0] & GETMAXVALUE(long_flag_pos);
  word this_header_byte_size = 1;
  // Note: For indefinite form its true - length fills in one byte (length_size == 0)
  return length_size + this_header_byte_size;
}

bool length::IsIndefinite(const std::string &str)
{
  return str[0] == GETMAXCOUNT(long_flag_pos);
}

std::string length::EncodeIndifiniteEnd()
{
  std::string ret;
  ret.push_back(0);
  ret.push_back(0);
  return ret;
}
