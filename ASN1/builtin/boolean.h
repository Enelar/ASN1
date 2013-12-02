#include "../builtin/builtin.h"
namespace asn1
{
  namespace builtin_types
  {
    struct boolean : entity
    {
      bool content;
      operator std::string() const;
      int Length() const;
      LENGTH_TYPE LengthType() const;
    };
  }

  template<>
  struct builtin<bool> : builtin_types::boolean
  {};
}