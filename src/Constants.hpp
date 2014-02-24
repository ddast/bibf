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
