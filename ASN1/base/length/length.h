#pragma once

#include "../../stdafx.h"

namespace asn1
{
  struct length
  {
    enum LENGTH_TYPE
    {
      DEFINITE,
      INDEFINITE
    };

    static std::string EncodeLength(word);
    static std::string EncodeIndifinite();
    static std::string EncodeIndifiniteEnd();
    static word DecodeLength(std::string);
    static word SizeofLength(const std::string &);
    static bool IsIndefinite(const std::string &);
  private:
    static const int long_flag_pos = BITSINBYTE - 1;
    static const int length_content_in_octet = BITSINBYTE;
  };
}