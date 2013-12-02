#pragma once
#include "inditifier.h"
namespace asn1
{
  enum inditifier::CLASS
  {
    UNIVERSAL = 0,
    APPLICATION = 1,
    CONTEXT_SPECIFIC = 2,
    PRIVATE = 3
  };
}
