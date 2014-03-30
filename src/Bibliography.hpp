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

#ifndef BIBLIOGRAPHY_H
#define BIBLIOGRAPHY_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

class Bibliography
{
  public:
    // Constructur only defines some standard values
    Bibliography();

    // Add the content of a stream to the bibliography
    void add(std::istream &is);

    // Changes all keys to the scheme:
    // last name of the first author + last two digits of the year + {a,b,c...}
    void create_keys();

    // Changes every bibEntry.type in 'bib' to lower (case_t=='L'),
    // upper case (case_t=='U') or start case (case_t='S')
    // Change the case of every bibEntry.type to 'case_t'
    // and the case of every bibElement.field to 'case_f'
    // Valid values for 'case_{t,f}': U (upper), L (lower), S (start)
    void change_case(const char case_t, const char case_f);

    // Delete the field 'field' in every bibEntry (case insensitive)
    void erase_field(std::string field);

    // Sort the bibliography after the sort criteria given in 'criteria'
    // valid values are "type", "key", "firstauthor" and every string used
    // as a field name
    void sort_bib(std::vector<std::string> criteria);

    // Sort the elements of every entry in alphabetical order
    void sort_elements();

    // Prints missing required fields of each entry
    // if 'only_required' == false optional missing fields are shown too
    void show_missing_fields(bool only_required = true) const;

    // Show all entries that do not define the fields in 'fields'
    void show_missing_fields(std::vector<std::string> fields) const;

    // Set intendation used before every bibElement
    void set_intendation(const std::string &str);

    // Set column after which lines are broken, '0' does no linebreak
    void set_linebreak(unsigned int i);

    // Set field delimiter
    void set_field_delimiter(char beg, char end);

    // Print the bibliography to std::cout
    void print_bib(std::ostream &os) const;

    // Print only the field defined in 'print_only' (case insensitive)
    void print_bib(std::vector<std::string> only, std::ostream &os) const;

    // Try to find the correct abbreviations for the month field
    void abbreviate_month();

  private:
    struct bibElement
    {
      std::string field;
      std::string value;
    };
    
    struct bibEntry
    {
      std::string type;
      std::string key;
      std::vector<bibElement> element;
    };

    // Internal representation of the bibliography
    std::vector<bibEntry> bib;

    // Use intendation, standard value "  "
    std::string intend;

    // Break lines after 'linebreak', standard value 79
    unsigned int linebreak;

    // Use these symbols to mark the begin and end of field value,
    // standard values '{' and '}'
    char field_beg;
    char field_end;
    
    // Extracts characters from 'is' and stores them into 'str' until the block
    // ends. A block is denoted by '}' and the block may contain pairs of
    // parenthesis.
    std::istream& get_block(std::istream& is, std::string& str) const;
    
    // Extracts characters from 'is' and stores them into 'str' until an
    // unnested ',' is found, i.e. one that is not inside parenthesis.
    std::istream& get_unnested(std::istream& is, std::string& str) const;
    
    // Reads one bibtex entry from 'is' and stores it into 'bEn'
    std::istream& get_bibEntry(std::istream& is, bibEntry& bEn) const;

    // Returns the last name of the first author
    std::string get_lastname(std::string author) const;

    // Returns the value of 'field' in the bibEntry 'bE'
    // 'field' is case insensitive
    std::string get_field_value(const bibEntry& bE, std::string field) const;

    // Deletes all double spaces, leading/ending spaces and nonprintable
    // characters in 'str'
    std::string clean_string(std::string str) const;

    std::string clean_key(std::string key) const;

    // Checks if the given string is a numerical value
    bool is_numerical(const std::string &s) const;

    // Do some basic consistency checking
    bool check_consistency() const;

};

#endif

