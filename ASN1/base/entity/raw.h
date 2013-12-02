#include "entity.h"

namespace asn1
{
  struct raw : entity
  {
    LENGTH_TYPE length_type;
    int length;
    std::string content;

    raw(std::string a);

    virtual operator std::string() const;
    virtual int Length() const;
    virtual LENGTH_TYPE LengthType() const;
  };
}