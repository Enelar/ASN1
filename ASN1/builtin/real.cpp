#include "real.h"
#include "../internal_header.h"
#include <list>

using namespace asn1::builtin_types;

namespace
{
  struct header
  {
    unsigned binary_enc_flags : 2;
    unsigned sign : 1;
    unsigned encode_base : 2;
    unsigned scaling_factor : 2;
    unsigned exponent_format : 2;

  };
  enum BASES
  {
    BASE2 = 0,
    BASE8 = 1,
    BASE16 = 16
  };

  struct real_parser
  {
    dbl content;
    header h;
    int mantess, exponent;
    int content_size;
    std::string ret;
    real_parser(dbl _content)
      : content(_content)
    {
      GenHeader();
      GenExponent();
    }

    static ui CalculateIntSize(ui i)
    {
      ui ret = 0;
      do
      {
        ret++;
        i >>= BITSINBYTE;
      } while (i & BYTE_MAX);
      return ret;
    }

    void GenHeader()
    {
      if (!content)
        content_size = 0;
      if (content == INFINITY || content == -INFINITY)
      { // ISO:IEC 8825-1:2003, page 8, 8.5
        content_size = 1;
        ub plus_infinity = 1 << 6;
        if (content == -INFINITY)
          plus_infinity++;
        ret += ax::convert<char>(plus_infinity);
      }
      h.binary_enc_flags = 2; // default is binary encoding 0b10
      if (content < 0)
        h.sign = 1; // ISO:IEC 8825-1:2003, page 8, 8.5.8
      REFACTOR h.encode_base = BASE2; // Simple test
      REFACTOR h.scaling_factor = 0; // I didnt understand standart clearly. Testing.
    }

    void GenExponent()
    {
      dbl mantess = content;
      exponent = [&mantess]()
      {
        dbl c = ax::Abs(mantess);
        bool decrement = c >= 1;
        ui exponent = 0;

        while (c >= 10 || c < 1)
        {
          if (decrement)
          {
            c /= 2;
            exponent++;
          }
          else
          {
            c *= 2;
            exponent++;
          }
        }
        mantess = c;
        return exponent;
      }();

      word exponent_size = CalculateIntSize(exponent);
      throw_assert(exponent_size);
      if (exponent_size > 3)
        h.exponent_format = 3;
      else
        h.exponent_format = exponent_size - 1;

      this->mantess = [](dbl mantess)
      {
        word test;
        test = ax::convert<word>(mantess);
        mantess *= GETMAXCOUNT(BITSINBYTE * (WORD_SIZE - 1)); // store max bits after point in word
        test = ax::convert<word>(mantess);

        return ax::convert<word>(mantess);
      }(mantess);
    }

    operator std::string() const
    {

    }

  private:

  };
}

real::operator std::string() const
{
  real_parser rp(content);

  std::string ret;
  ConCat(ret, type);
  ConCat(ret, EncodeLength());
  int only_head_length = ret.length();
  ret += *reinterpret_cast<char *>(&rp.h);

  auto EncodeNum = 
  [&]( ui num )
  {
    std::list<ub> res;

    auto Flush = [&]()
    {
      while (res.size())
      {
        ret.push_back(res.front());
        res.pop_front();
      }
    };
    ui num_size = real_parser::CalculateIntSize(num);
    auto GenExponentSequence = [&]()
    {
      ui exponent_copy = rp.exponent;

      for (word i = 0; i < num_size; i++)
      {
        res.push_front(exponent_copy & BYTE_MAX);
        exponent_copy >>= BITSINBYTE;
      }
    };

    if (num_size > 3)
    {
      rp.h.exponent_format = 3;
      ret += ax::convert<char>(num_size);
    }
    else
    {
      if (!num_size)
        num_size = 1;
      rp.h.exponent_format = num_size - 1;
    }

    GenExponentSequence();
    Flush();
  };


  length_undefined = false;
  content_length = ret.length() - only_head_length;

  return ret;
}

int real::Length() const
{
  if (!length_undefined)
    return content_length;
  if (content == 0)
    return length_undefined = false, content_length = 0;
  if (in_length)
    return content_length; // cross-call
  in_length = true;

  convert<std::string>(*this);

  throw_assert(!length_undefined);
  in_length = false;
  return content_length;
}

real::LENGTH_TYPE real::LengthType() const
{
  return LENGTH_TYPE::DEFINITE;
}

real::real(double _content)
: entity(
  {
    inditifier::CLASS::UNIVERSAL, 
    false,
    9
  }), content(_content)
{

}