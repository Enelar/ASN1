#include "../builtin/builtin.h"
namespace asn1
{
  namespace builtin_types
  {
    struct real : entity
    {
      double content;
      operator std::string() const;
      int Length() const;
      LENGTH_TYPE LengthType() const;

      real(double content = 0);
    private:
      mutable bool in_length = false, length_undefined = true;
      mutable word content_length = 1;
    };
  }
}