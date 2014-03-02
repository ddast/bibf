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

    // Changes every bibElement.field in 'bib' to lower (uplo=='L')
    // or upper case (uplo=='U')
    void change_field_case(const char uplo);

    // Delete the field 'field' in every bibEntry (case insensitive)
    void erase_field(std::string field);

    // Sort the bibliography after the sort criteria given in 'criteria'
    // valid values are "type", "key", "firstauthor" and every string used
    // as a field name
    void sort_bib(std::vector<std::string> criteria);

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
    void print_bib() const;

    // Print only the field defined in 'print_only' (case insensitive)
    void print_bib(std::vector<std::string> print_only) const;

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
    
    // Deletes leading and ending characters defined in 'trimchar' from 'str'
    std::string trim(const std::string& str,
        const std::string& trimchar = " \t\n") const;
    
    // Reads one bibtex entry from 'is' and stores it into 'bEn'
    std::istream& get_bibEntry(std::istream& is, bibEntry& bEn) const;

    // Returns the last name of the first author
    std::string get_lastname(std::string author) const;

    // Returns the value of 'field' in the bibEntry 'bE'
    // 'field' is case insensitive
    std::string get_field_value(const bibEntry& bE, std::string field) const;

    // Replaces characters defined in 'delchar' of 'str' with spaces and
    // deletes all double spaces
    std::string clean_string(std::string str,
        const std::string& delchar = "\t\n") const;

    // Checks if the given string is a numerical value
    bool is_numerical(const std::string &s) const;

    // Do some basic consistency checking
    bool check_consistency() const;

};

#endif

