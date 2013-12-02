#include "inditifier.h"

using namespace asn1;

inditifier::inditifier(CLASS _type_class, bool _constructed, int _type_oid )
: type_class(_type_class), is_constructed(_constructed), tag_number(_type_oid)
{

}

inditifier::inditifier(int _type_oid)
: inditifier(inditifier::UNIVERSAL, false, _type_oid)
{

}

namespace
{
  struct raw_inditifier_byte
  {
    ub oid_if_simple_type : 5;
    ub is_constructed : 1;
    ub class_type : 2;

    raw_inditifier_byte(const ub &t)
    {
      class_type = t >> 6;
      is_constructed = t >> 5;
      oid_if_simple_type = t & GETMAXVALUE(5);
    }
    raw_inditifier_byte()
    {

    }
    operator ub() const
    {
      ub ret;
      ret = class_type;
      ret <<= 1;
      ret |= is_constructed;
      ret <<= 5;
      ret |= oid_if_simple_type;
      return ret;
    }
  };
}

inditifier::inditifier(std::string str)
{
  ub byte = str[0];
  raw_inditifier_byte rb(byte);
  type_class = (inditifier::CLASS)rb.class_type;
  is_constructed = rb.is_constructed;
  throw_sassert(rb.oid_if_simple_type && rb.oid_if_simple_type != GETMAXVALUE(5), "TODO: subsequent inditifier tag octets");
  tag_number = rb.oid_if_simple_type;
}

inditifier::operator std::string() const
{
  raw_inditifier_byte rb;
  rb.class_type = type_class;
  rb.is_constructed = is_constructed;
  throw_sassert(tag_number && tag_number < 31, "TODO: subsequent inditifier tag octets");
  rb.oid_if_simple_type = tag_number;
  return std::to_string(rb);
}

int inditifier::OffsetToLength(const std::string &a)
{ // ISO:IEC 8825-1:2003, 8.1.2.4.3
  ub byte = a[0];
  if ((byte & GETMAXVALUE(5)) != GETMAXVALUE(5))
    return 1;

  int i = 0;
  do
    byte = a[++i];
  while (byte & GETMAXCOUNT(7));

  return i;
}