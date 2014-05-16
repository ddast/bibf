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

#ifndef PARSER_H
#define PARSER_H

class Parser
{
  public:

  private:
    // Extracts characters from 'is' and stores them into 'str' until the block
    // ends. A block is denoted by '}' and the block may contain pairs of
    // parenthesis.
    std::istream& get_block(std::istream& is, std::string& str) const;
    
    // Extracts characters from 'is' and stores them into 'str' until an
    // unnested ',' is found, i.e. one that is not inside parenthesis.
    std::istream& get_unnested(std::istream& is, std::string& str) const;
    
    // Reads one bibtex entry from 'is' and stores it into 'bEn'
    std::istream& get_bibEntry(std::istream& is, bibEntry& bEn) const;
};

#endif
