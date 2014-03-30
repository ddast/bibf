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

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <array>
#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <algorithm>

class Constants
{
  public:
    // returns the required fields of the given type (case insensitive)
    static std::vector<std::string> get_required_values(std::string type);

    // returns the optional fields of the given type (case insensitive)
    static std::vector<std::string> get_optional_values(std::string type);

    // checks if 's' is a valid month abbreviation
    static bool is_valid_month_abbreviation(const std::string& s);

    // tries to find the matching abbreviation to 's'
    static std::string find_month_abbreviation(const std::string& s);

  private:
    // Standard entry fields
    static const std::array<std::string, 27> standard_entry_fields;

    // Required and optional fields for every type
    static const std::map< std::string, std::pair< std::vector<int>,
                 std::vector<int> > > standard_types;
    
    // Three-letter abbreviations for the months
    static const std::array<std::string, 12> month_abbreviations;

};

#endif
