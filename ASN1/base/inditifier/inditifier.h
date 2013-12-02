#pragma once

#include "../../stdafx.h"
#include <string>

namespace asn1
{
  struct inditifier
  {
    enum CLASS;

    CLASS type_class;
    
    bool is_constructed;
    int tag_number;

    inditifier(CLASS, bool, int);
    inditifier(int);
    inditifier(std::string);
    inditifier(const inditifier &) = default;

    operator std::string() const;

    static int OffsetToLength(const std::string &);
  };
};

#include "inditifier_class.h"