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

#ifndef STRINGS_H
#define STRINGS_H


#include <array>
#include <unordered_map>
#include <string>

class Strings
{
  public:
    // strings to translate
    enum STR {
      OPT_USAGE,
      OPT_OUTPUT,
      OPT_CREATE,
      OPT_ONLY,
      OPT_SORT_BIB,
      OPT_SORT_ELEMENTS,
      OPT_ERASE_FIELD,
      OPT_SHOW_MISSING,
      OPT_MISSING_FIELDS,
      OPT_CHANGE_CASE,
      OPT_LINEBREAK,
      OPT_INTENDATION,
      OPT_DELIMITER,
      OPT_ABBREV_MONTH,
      OPT_HELP,
      OPT_VERSION,
      OPT_INPUT,
      OUT_VERSION,
      ERR_CHANGE_CASE,
      ERR_DELIMITER
    };

    // set the current locale
    void set_locale(std::string lang);

    // returns the string in the current language
    std::string tr(STR str) { return langs[current_lang][str]; }

  private:
    // supported language count
    static const int LANG_CNT = 2;

    // translated strings count
    static const int STR_CNT = 20;

    // supported languages
    enum LANG {
      LANG_EN,
      LANG_DE
    };
 
    // map to translage between a string the the enum
    static const std::unordered_map<std::string, LANG> lang_map;

    // current language
    LANG current_lang = LANG::LANG_EN;

    // strings in different languages
    static const std::array<std::string, STR_CNT> en;
    static const std::array<std::string, STR_CNT> de;

    // contains the arrays with the different languages
    static const std::array<std::array<std::string, STR_CNT>, LANG_CNT> langs;
};

#endif
