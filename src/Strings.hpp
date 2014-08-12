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
      OPT_ALIGN_LEFT,
      OPT_ABBREV_MONTH,
      OPT_NEW_ENTRY,
      OPT_HELP,
      OPT_VERSION,
      OPT_INPUT,
      OUT_VERSION,
      OUT_MISSES_REQUIRED,
      OUT_MISSES_OPTIONAL,
      OUT_MISSING_FIELD,
      OUT_CREATE_ENTRY_REQ,
      OUT_CREATE_ENTRY_OPT,
      OUT_CREATE_ENTRY_UNKNOWN,
      OUT_CREATE_ENTRY_ARB,
      ERR_CHANGE_CASE,
      ERR_DELIMITER,
      ERR_EMPTY_AUTHOR,
      ERR_DOUBLE_KEY_1,
      ERR_DOUBLE_KEY_2,
      ERR_RAN_OUT_OF_IDS_1,
      ERR_RAN_OUT_OF_IDS_2,
      ERR_RAN_OUT_OF_IDS_3,
      ERR_UNKNOWN_CHANGE_CASE_T,
      ERR_UNKNOWN_CHANGE_CASE_F,
      ERR_ILLEGAL_FIELD_DELIMITER_BEG,
      ERR_ILLEGAL_FIELD_DELIMITER_END,
      ERR_CREATE_ENTRY_TYPE_NEEDED,
      STR_CNT
    };

    // set the current locale
    static void set_locale(std::string lang);

    // returns the string in the current language
    static std::string tr(STR str) { return langs[current_lang][str]; }

  private:
    // supported languages
    enum LANG {
      LANG_EN,
      LANG_DE,
      LANG_CNT
    };

    // program version
    static const std::string version;

    // map to translage between a string the the enum
    static const std::unordered_map<std::string, LANG> lang_map;

    // current language (default english)
    static LANG current_lang;

    // strings in different languages
    static const std::array<std::string, STR_CNT> en;
    static const std::array<std::string, STR_CNT> de;

    // contains the arrays with the different languages
    static const std::array<std::array<std::string, STR_CNT>, LANG_CNT> langs;
};

#endif
