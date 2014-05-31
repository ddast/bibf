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

#include "Parser.hpp"

using std::istream;
using std::string;
using std::stringstream;


void Parser::add(istream &is, std::vector<bibEntry> &bib)
{
  // add the stream to 'bib'
  for (bibEntry bE; get_bibEntry(is, bE); bE.element.clear())
    bib.push_back(bE);
}


string Parser::clean_string(string str) const
{
  // Replace all nonprintable characters with spaces
  auto del_from = std::remove_if(str.begin(), str.end(),
      [] (char c) -> bool { return !isprint(c); });
  str.erase(del_from, str.end());

  // Remove double spaces
  del_from = std::unique(str.begin(), str.end(),
      [] (char c1, char c2) { return (c1 == c2 && c1 == ' '); } );
  str.erase(del_from, str.end());

  // Delete leading and ending spaces
  size_t first = str.find_first_not_of(" ");
  if (first == string::npos)
    return "";
  size_t last = str.find_last_not_of(" ");
  str = str.substr(first, last-first+1);

  return str;
}


istream& Parser::get_block(istream& is, string& str) const
{
    stringstream block;
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

istream& Parser::get_unnested(istream& is, string& str) const
{
  stringstream unnested;
  int depth = 0;
  for (char c; is.get(c);) {
    if (c == '{')
      ++depth;
    else if (c == '}')
      --depth;
    else if ((c == ',') && !depth)
      break;
    unnested << c;
  }
  str = unnested.str();
  return is;
}

istream& Parser::get_bibEntry(istream& is, bibEntry& bEn) const
{
  // Discard everything bevore the first @
  string tmp;
  std::getline(is, tmp, '@');

  // get type
  std::getline(is, bEn.type, '{');  

  // save block in stringstream
  string bEn_s;
  get_block(is, bEn_s);
  stringstream bEn_ss(bEn_s);

  // create bibEntry
  std::getline(bEn_ss, bEn.key, ',');
  while (true) {
    string bEl_s;
    bool last(false);
    // get one line ending with ',' however last line may not end with ','
    if (!get_unnested(bEn_ss, bEl_s)) {
      if (clean_string(bEl_s).empty()) break;
      else last = true;
    }
    // extract element
    stringstream bEl_ss(bEl_s);
    bibElement bEl;
    // field is the part before '='
    std::getline(bEl_ss, bEl.field, '=');
    bEl.field = clean_string(bEl.field);
    // 'delim' is the first printable character that is not a space
    string bEl_str = bEl_ss.str().substr(bEl_ss.tellg());
    char delim = ' ';
    for (char& c : bEl_str) {
      if ((!isspace(c)) && (isprint(c))) {
        delim = c;
        break;
      }
    }
    // value may be in {} or "" or without delimiter
    std::getline(bEl_ss, tmp, delim);
    if (delim == '{')
      get_block(bEl_ss, bEl.value);
    else if (delim == '"')
      std::getline(bEl_ss, bEl.value, delim);
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

