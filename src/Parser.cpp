/*  bibf - a simple bibtex pretty printer
 *
 *  Copyright (C) 2014 Dennis Dast <mail@ddast.de>
 *
 *  This file is part of bibf.
 *
 *  bibf is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  bibf is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with bibf.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <algorithm>
#include <sstream>
#include "DataStructure.hpp"
#include "Parser.hpp"

void Parser::add(std::istream &is, std::vector<bibEntry> &bib)
{
  // add the stream to 'bib'
  for (bibEntry bE; get_bibEntry(is, bE); bE.element.clear())
    bib.push_back(bE);
}


std::string Parser::clean_string(std::string str) const
{
  // Remove the characters \f, \n, \r, \t, \v
  auto del_from = std::remove_if(str.begin(), str.end(),
      [] (char c) -> bool { return (isspace(c) && c != ' '); });
  str.erase(del_from, str.end());

  // Remove double spaces
  del_from = std::unique(str.begin(), str.end(),
      [] (char c1, char c2) { return (c1 == c2 && c1 == ' '); } );
  str.erase(del_from, str.end());

  // Delete leading and ending spaces
  size_t first = str.find_first_not_of(" ");
  if (first == std::string::npos)
    return "";
  size_t last = str.find_last_not_of(" ");
  str = str.substr(first, last-first+1);

  return str;
}


std::istream& Parser::get_block(std::istream& is, std::string& str) const
{
    std::stringstream block;
    int depth = 1;
    for (char c; is.get(c);) {
      if (c == '{')
        ++depth;
      else if ((c == '}') && !--depth)
        break;
      block << c;
    }
    str = block.str();
    return is;
}

std::istream& Parser::get_unnested(std::istream& is, std::string& str) const
{
  std::stringstream unnested;
  int depth(0);
  bool use_quotes = false;
  bool is_escaped = false;

  // iterate over all characters
  for (char c; is.get(c);) {

    // a leading quotation mark increases the depth, an ending one decreases it
    if (c == '"' && !is_escaped) {
      if (depth == 0) {
        ++depth;
        use_quotes = true;
      }
      else if ( (depth == 1) && (use_quotes) ) {
        --depth;
      }
    }

    // increase depth at { and decrease at }
    else if (c == '{') {
      ++depth;
    }
    else if (c == '}') {
      --depth;
    }

    // stop if we are at depth zero and a comma is found
    else if ((c == ',') && !depth) {
      break;
    }

    // save if escape character is used
    if (c == '\\') {
      is_escaped = true;
    } else {
      is_escaped = false;
    }

    // add character to unnested
    unnested << c;
  }

  str = unnested.str();
  return is;
}

std::istream& Parser::get_bibEntry(std::istream& is, bibEntry& bEn) const
{
  // Discard everything bevore the first @
  std::string tmp;
  std::getline(is, tmp, '@');

  // get type
  std::getline(is, bEn.type, '{');  
  bEn.type = clean_string(bEn.type);

  // save block in stringstream
  std::string bEn_s;
  get_block(is, bEn_s);
  std::stringstream bEn_ss(bEn_s);

  // create bibEntry
  auto old_position = bEn_ss.tellg();
  std::getline(bEn_ss, bEn.key, ',');
  bEn.key = clean_string(bEn.key);
  if (bEn.key.find('=') != std::string::npos) {
    bEn.key = "";
    bEn_ss.seekg(old_position);
  }
  while (true) {
    std::string bEl_s;
    bool last(false);
    // get one line ending with ',' however last line may not end with ','
    if (!get_unnested(bEn_ss, bEl_s)) {
      if (bEl_s.empty()) {
        break;
      } else {
        last = true;
      }
    }
    bEl_s = clean_string(bEl_s);
    // extract element
    std::stringstream bEl_ss(bEl_s);
    bibElement bEl;
    // field is the part before '='
    std::getline(bEl_ss, bEl.field, '=');
    bEl.field = clean_string(bEl.field);
    // 'delim' is the first printable character that is not a space
    std::string bEl_str = bEl_ss.str().substr(bEl_ss.tellg());
    char delim = ' ';
    for (char& c : bEl_str) {
      if ((!isspace(c)) && (isprint(c))) {
        delim = c;
        break;
      }
    }
    // value may be in {} or "" or without delimiter
    std::getline(bEl_ss, tmp, delim);
    if (delim == '{') {
      get_block(bEl_ss, bEl.value);
    } else if (delim == '"') {
      std::getline(bEl_ss, bEl.value);
      auto pos = bEl.value.find_last_of(delim);
      if (pos != std::string::npos) {
        bEl.value = bEl.value.substr(0, pos);
      }
    }
    else {
      bEl_ss.unget();
      std::getline(bEl_ss, bEl.value);
    }
    bEl.value = clean_string(bEl.value);
    bEn.element.push_back(bEl);
    if (last) break;
  }

  return is;
}

