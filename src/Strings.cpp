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

#include "Strings.hpp"

const std::unordered_map<std::string, Strings::LANG> Strings::lang_map {
  {"en", Strings::LANG::LANG_EN},
  {"de", Strings::LANG::LANG_DE}
};


const std::array<std::string, Strings::STR_CNT> Strings::en {{
  "Usage: bibf [OPTION]... [FILE]...",
  "print to file instead of cout",
  "create keys using last name of first author plus last"
    " two digits of the year plus [a,b,c...]",
  "print only the given fields;"
    " different fields must be separated by commas",
  "sort the bibliography by the given citeria; valid values are 'type',"
    " 'key', 'firstauthor' and every string used as field identifier;"
    " different values must be separated by commas",
  "sort the elements of each entry alphabetically",
  "erase the field in every entry; use comma to apply more than one value",
  "show missing required fields and also missing optional fields with"
    " option O",
  "show all entries that do not contain a specific field;"
    " use commas to search for more than one field",
  "change the case of all types and keys to lower (L), upper (U) or"
    " start (S) case (default L); for different cases of types and cases"
    " use two characters (LU, UL, ...)",
  "break lines after given amount of characters (default 79)",
  "change intendation for pretty printing (default two spaces)",
  "set field delimiter, valid values are { or \"",
  "try to find the correct abbreviation of the month",
  "display this help and exit",
  "output version information and exit",
  "bibtex files for input",
  "bibf 0.1 Copyright (C) 2014 Dennis Dast\n\n"
    "This program comes with ABSOLUTELY NO WARRANTY.\n"
    "This is free software, and you are welcome to redistribute it"
    " under certain conditions.\n",
  "Malformatted option '--change-case', valid options"
    " are one or two characters.\n",
  "Illegal field delimiter: "
}};

const std::array<std::string, Strings::STR_CNT> Strings::de {{
  "D1",
  "D2",
  "D3",
  "D4",
  "D5",
  "D6",
  "D7",
  "D8",
  "D9",
  "D10",
  "D11",
  "D12",
  "D13",
  "D14",
  "D15",
  "D16"
}};

const std::array<std::array<std::string, Strings::STR_CNT>, Strings::LANG_CNT>
    Strings::langs {{
  Strings::en,
  Strings::de
}};

void Strings::set_locale(std::string lang)
{
  try {
    current_lang = lang_map.at(lang);
  }
  catch(std::out_of_range& e) {
    current_lang = LANG_EN;
  }
}
