#include "entity.h"
#include "../length/length.h"
#include "../../internal_header.h"
#include <list>

using namespace asn1;
using std::string;
using std::list;

entity::entity(inditifier _type)
: type(_type)
{

}


string entity::EncodeLength(word length) const
{
  return length::EncodeLength(length);
}

string entity::EncodeLength() const
{
  return EncodeLength(Length());
}

string entity::EncodeConstructed(word chunk_max_size, const string &source)
{
#define FORCE_INDEFINITE_FORM 1 // require for CER encoding
  auto GetHeader = [this, source]()
  {
    auto GetConstructedInditifier = [this]()
    {
      inditifier constructed(this->type);
      constructed.is_constructed = true;
      return convert<string>(constructed);
    };
    auto ret = GetConstructedInditifier();
    ConCat(ret, FORCE_INDEFINITE_FORM ? length::EncodeIndifinite() : EncodeLength());
    return ret;
  };
  auto header = GetHeader();
  auto source_copy = source;
  std::vector<string> chunks;

  while (source_copy.length())
  {
    string chunk = type;
    word write_data = ax::Min(source_copy.length(), chunk_max_size);
    ConCat(chunk, EncodeLength(write_data));
    ConCat(chunk, source_copy, write_data);
    source_copy = Shift(source_copy, write_data);
  }

  if (chunks.size() == 1)
    return chunks[0];

  std::list<string> oper(std::make_move_iterator(chunks.begin()), std::make_move_iterator(chunks.end()));

  while (oper.size())
  {
    ConCat(header, oper.front());
    oper.pop_front();
  }
  ConCat(header, length::EncodeIndifiniteEnd());
  return header;
}