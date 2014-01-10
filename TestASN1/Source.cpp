#include "../ASN1/asn1.h"
#include "../ASN1/builtin/real.h"

void main()
{
  asn1::builtin_types::real r;
  r.content = 4;
  std::string res = r;
}