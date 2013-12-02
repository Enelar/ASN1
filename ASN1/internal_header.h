#pragma once

#include "stdafx.h"
#include <string>
#include <sstream>


std::string ConCat(std::string &a, const std::string &b, const word b_length);


std::string ConCat(std::string &a, const std::string &b);


std::string Shift(std::string base, const word left, const word right = 0);