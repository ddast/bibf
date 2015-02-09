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

#include <iostream>
#include <string>
#include <vector>

// Forward declaration of user-defined types
class bibEntry;

class Bibliography
{
  public:
    // Constructur
    Bibliography();

    // Destructor
    ~Bibliography();

    // Add the content of a stream to the bibliography
    void add(std::istream &is);

    // Create new entry with the standard fields
    void create_entry();

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

    // Set alignment
    void set_alignment(bool _right_aligned);

    // Print the bibliography to the stream 'os'
    void print_bib(std::ostream &os) const;

    // Print only the field defined in 'print_only' (case insensitive)
    void print_bib(std::vector<std::string> only, std::ostream &os) const;

    // Try to find the correct abbreviations for the month field
    void abbreviate_month();

  private:
    // Internal representation of the bibliography
    std::vector<bibEntry> *bib;

    // Use intendation, standard value "  "
    std::string intend;

    // Break lines after 'linebreak', standard value 79
    unsigned int linebreak;

    // Use these symbols to mark the begin and end of field value,
    // standard values '{' and '}'
    char field_beg;
    char field_end;

    // Use left or right alignment for the field names (default right)
    bool right_aligned;
    
    // Returns the last name of the first author
    std::string get_lastname(std::string author) const;

    // Returns the value of 'field' in the bibEntry 'bE'
    // 'field' is case insensitive
    std::string get_field_value(const bibEntry& bE, std::string field) const;

    // Removes all characters not allowed in the key of a bibtex entry
    std::string clean_key(std::string key) const;

    // Checks if the given string is a numerical value
    bool is_numerical(const std::string &s) const;

    // Do some basic consistency checking
    void check_consistency() const;

    // Insert line breaks into 'str' such that every line contains 'linebreak'
    // characters or less. Insert 'intend' before every new line.
    std::string break_string(std::string str, const std::string &intend) const;

    // Asks user to enter all fields in vector 'fields' and add them to the
    // bibEntry 'bEn'
    void ask_for_fields(bibEntry &bEn, const std::vector<std::string> &fields)
      const;

    // Delete redundant entries, i.e. entrys that are a subset of another entry
    void delete_redundant_entries();

};

#endif

