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

#include "Bibliography.hpp"

std::string Bibliography::clean_key(std::string key) const
{
  // allowed characters in key
  std::string allowed = "abcdefghijklmnopqrstuvwxyz"
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "1234567890:.-";

  // delete all not allowed keys
  while (true) {
    size_t pos = key.find_first_not_of(allowed);
    if (pos == std::string::npos)
      break;
    key.erase(pos, 1);
  }

  return key;
}


std::string Bibliography::get_lastname(std::string author) const
{
  // get the first author
  size_t found = author.find("and");
  if (found != std::string::npos)
    author = author.substr(0, found);

  // get last name
  found = author.find(",");
  if (found != std::string::npos)
    author = author.substr(0, found);
  else {
    std::stringstream author_ss(author);
    while (author_ss >> author);
  }


  if (author.empty()) {
    std::cerr << Strings::tr(Strings::ERR_EMPTY_AUTHOR) << std::endl;
    return "";
  }

  // first character upper case, rest lower case
  std::transform(author.begin()+1, author.end(), author.begin()+1, ::tolower);
  std::toupper(author.at(0));

  // return last name of first author
  return author;
}


bool Bibliography::is_numerical(const std::string& s) const
{
  size_t found = s.find_first_not_of("1234567890");
  return (found==std::string::npos);
}


void Bibliography::print_bib(std::ostream &os) const
{
  std::vector<std::string> empty;
  print_bib(empty, os);
}


void Bibliography::print_bib(std::vector<std::string> only,
    std::ostream &os) const
{
  bool print_all = only.empty();
  // convert list to lowercase
  if (!print_all)
    for (std::string& s : only)
      std::transform(s.begin(), s.end(), s.begin(), ::tolower);
  // iterate over all entries (use copies)
  for (bibEntry bEn : bib) {
    // delete all entries which are not printed
    if (!print_all) {
      bEn.element.erase(
          std::remove_if(bEn.element.begin(), bEn.element.end(),
            [&] (bibElement bEl) -> bool {
              std::transform(bEl.field.begin(), bEl.field.end(),
                bEl.field.begin(), ::tolower);
              for (const std::string &po : only) {
                if (po == bEl.field) 
                  return false;
              }
              return true;
            }),
          bEn.element.end());
    }

    // search for longest field name
    unsigned int longest_field = 0;
    if (right_aligned) {
      for (const bibElement &bEl : bEn.element)
        if (bEl.field.length() > longest_field)
          longest_field = bEl.field.length();
    }

    // print key
    os << '@' << bEn.type << '{' << bEn.key;
    // print elements
    for (const bibElement& bEl : bEn.element) {
      os << ",\n";
      // Use no field delimiters if value is a numeric
      bool print_delimiter = true;
      if (is_numerical(bEl.value))
        print_delimiter = false;
      // Or if the month field uses three-letter abbreviations
      std::string field = bEl.field;
      std::transform(field.begin(), field.end(), field.begin(), ::tolower);
      if ( (field == "month") &&
          Constants::is_valid_month_abbreviation(bEl.value) )
        print_delimiter = false;
      // construct line
      std::string line;
      if (right_aligned) {
        for (unsigned int i = 0, end = longest_field-field.length();
            i < end; ++i)
          line.push_back(' ');
      }
      line += intend + bEl.field + " = ";
      std::string intend_after_break;
      for (unsigned int i = 0, length = line.length(); i < length; ++i)
        intend_after_break.push_back(' ');
      if (print_delimiter)
        line += field_beg + bEl.value + field_end;
      else
        line += bEl.value;
      // break after 'linebreak' characters
      line = break_string(line, intend_after_break);
      os << line;
    }
    // finish entry
    os << "\n}\n" << std::endl;
  }
}


std::string Bibliography::break_string(std::string str,
    const std::string &intend) const
{
  // store each line in a vector
  std::vector<std::string> lines;
  // break until 'str' is small enough
  while (str.length() > linebreak) {
    // find the first space before linebreak
    for (unsigned int i = linebreak; i > 0; --i) {
      if (str[i] == ' ') {
        // replace it with a line break
        str[i] = '\n';
        // store line including line break in 'lines'
        lines.push_back(str.substr(0,i+1));
        // delete the line in 'str'
        str.erase(0,i+1);
        // add 'intend' to the beginning
        str = intend + str;
        // exit for loop
        break;
      }
    }
  }

  // create one string containing all lines
  std::string result("");
  for (const std::string &l: lines)
    result += l;
  // don't forget the rest in str
  result += str;
  return result;
}


Bibliography::Bibliography() :
  intend("  "),
  linebreak(79),
  field_beg('{'),
  field_end('}'),
  right_aligned(true)
{ }


void Bibliography::add(std::istream &is)
{
  // create parsing object
  Parser parser;

  // add the stream to the bibliography
  parser.add(is, bib);

  // do a consistency check
  check_consistency();
}

bool Bibliography::check_consistency() const
{
  // return true if the bibliography is consistent else false
  bool is_consistent = true;

  // check if every key is unique
  for (auto it1 = bib.begin(), end = bib.end(); it1 != end-1; ++it1) {
    for (auto it2 = it1+1; it2 != end; ++it2) {
      if (it1->key == it2->key) {
        std::cerr << Strings::tr(Strings::ERR_DOUBLE_KEY_1) << it1->key
          << Strings::tr(Strings::ERR_DOUBLE_KEY_2);
        is_consistent = false;
      }
    }
  }

  return is_consistent;
}


std::string Bibliography::get_field_value(const bibEntry &bE,
    std::string field) const
{
  // transform to lower case
  std::transform(field.begin(), field.end(), field.begin(), ::tolower);
  // search for entry
  for (const bibElement& bEl : bE.element) {
    std::string current = bEl.field;
    std::transform(current.begin(), current.end(), current.begin(), ::tolower);
    if (current == field)
      return bEl.value;
  }
  // return empty string if field was not found
  return "";
}

void Bibliography::create_keys()
{
  for (auto it = bib.begin(), end = bib.end(); it != end; ++it) {
    std::string author = get_lastname(get_field_value(*it, "author"));
    std::string year = get_field_value(*it, "year");
    if (year.length() >= 2)
      year = year.substr(year.length()-2, 2);
    it->key = author + year;
    char id = 'a';
    for (auto it2 = bib.begin(); it2 != it; ++it2)
      if (it->key == it2->key.substr(0,it2->key.length()-1))
        ++id;
    if (id > 'z') {
      std::cerr << Strings::tr(Strings::ERR_RAN_OUT_OF_IDS_1) << author
        << Strings::tr(Strings::ERR_RAN_OUT_OF_IDS_2) << year
        << Strings::tr(Strings::ERR_RAN_OUT_OF_IDS_3) << std::endl;
      break;
    }
    it->key += id;
    it->key = clean_key(it->key);
  }
}


void Bibliography::change_case(const char case_t, const char case_f)
{
  // operation for the types
  int (*touplo_t)(int);
  if (case_t == 'U')
    touplo_t= &std::toupper;
  else if (case_t == 'L')
    touplo_t= &std::tolower;
  else if (case_t == 'S')
    touplo_t= &std::tolower;
  else {
    std::cerr << Strings::tr(Strings::ERR_UNKNOWN_CHANGE_CASE_T)
      << case_t << std::endl;
    return;
  }

  // operation for the fields
  int (*touplo_f)(int);
  if (case_f == 'U')
    touplo_f= &std::toupper;
  else if (case_f == 'L')
    touplo_f= &std::tolower;
  else if (case_f == 'S')
    touplo_f= &std::tolower;
  else {
    std::cerr << Strings::tr(Strings::ERR_UNKNOWN_CHANGE_CASE_F)
      << case_f << std::endl;
    return;
  }

  for (bibEntry& bEn : bib) {
    std::transform(bEn.type.begin(), bEn.type.end(), bEn.type.begin(),
        touplo_t);
    if (case_t == 'S')
      bEn.type[0] = toupper(bEn.type[0]);
    for (bibElement& bEl : bEn.element) {
      std::transform(bEl.field.begin(), bEl.field.end(), bEl.field.begin(),
          touplo_f);
      if (case_f == 'S')
        bEl.field[0] = toupper(bEl.field[0]);
    }
  }
}


void Bibliography::erase_field(std::string field)
{
  std::transform(field.begin(), field.end(), field.begin(), ::tolower);
  auto compare = [&] (bibElement& el) -> bool {
    std::string cur = el.field;
    std::transform(cur.begin(), cur.end(), cur.begin(), ::tolower);
    if (cur == field) return true;
    else return false;
  };
  for (bibEntry& bEn : bib)
    bEn.element.erase( std::remove_if(bEn.element.begin(), bEn.element.end(),
        compare), bEn.element.end() );
}


void Bibliography::sort_bib(std::vector<std::string> criteria)
{
  // check if 'bE1' is smaller than 'bE2' using the given criteria
  auto cmp_after_criteria =
    [&] (const bibEntry& bE1, const bibEntry& bE2) -> bool
    {
      for (std::string& cur_crit : criteria) {
        std::transform(cur_crit.begin(), cur_crit.end(), cur_crit.begin(),
            ::tolower);
        int comp = 0;
        if (cur_crit == "type")
          comp = bE1.type.compare(bE2.type);
        else if (cur_crit == "key")
          comp = bE1.key.compare(bE2.key);
        else if (cur_crit == "firstauthor") {
          std::string str1 = get_field_value(bE1, "author");
          str1 = get_lastname(str1);
          std::string str2 = get_field_value(bE2, "author");
          str2 = get_lastname(str2);
          comp = str1.compare(str2);
        }
        else {
          std::string str1 = get_field_value(bE1, cur_crit);
          std::string str2 = get_field_value(bE2, cur_crit);
          comp = str1.compare(str2);
        }
        if (comp < 0) return true;
        if (comp > 0) return false;
      }
      return false;
    };
  
  // sort bibliography
  std::sort(bib.begin(), bib.end(), cmp_after_criteria);
  //bib.sort(cmp_after_criteria);
}


void Bibliography::sort_elements()
{
  // define compare function
  auto compare_field =
    [] (const bibElement& bEl1, const bibElement& bEl2) -> bool 
    {
      if (bEl1.field < bEl2.field)
        return true;
      return false;
    };

  // sort elements in each entry
  for (bibEntry& bEn : bib) {
    std::sort(bEn.element.begin(), bEn.element.end(), compare_field);
  }
}


void Bibliography::set_intendation(const std::string& str)
{
  // check for consitency and set intendation
  intend = str;
  if (intend.length()+1 >= linebreak)
    linebreak = intend.length()+1;
}


void Bibliography::set_linebreak(unsigned int i)
{
  // check for consistency and set linebreak
  if (i > 0)
    linebreak = i > intend.length() ? i : intend.length()+1;
  else
    linebreak = 0;
}


void Bibliography::set_alignment(bool _right_aligned)
{
  right_aligned = _right_aligned;
}


void Bibliography::set_field_delimiter(char beg, char end)
{
  // only {} and '' are valid delimiters but always set the delimiters
  field_beg = beg;
  field_end = end;
  if (!( (beg == '{') || (beg == '"') ))
    std::cerr << Strings::tr(Strings::ERR_ILLEGAL_FIELD_DELIMITER_BEG)
      << beg << std::endl;
  if (!( (end == '}') || (end == '"') ))
    std::cerr << Strings::tr(Strings::ERR_ILLEGAL_FIELD_DELIMITER_END)
      << beg << std::endl;
}


void Bibliography::show_missing_fields(bool only_required) const
{
  // check for missing required fields
  for (const bibEntry& bEn : bib) {
    std::vector<std::string> required =
      Constants::get_required_values(bEn.type);
    for (const std::string& current : required) {
      bool has_required_field = false;
      std::istringstream current_ss(current);
      for (std::string _current; std::getline(current_ss, _current, ' '); )
        if (!(get_field_value(bEn, _current).empty()))
          has_required_field = true;
      if (!has_required_field)
        std::cout << bEn.key << Strings::tr(Strings::OUT_MISSES_REQUIRED)
          << current << "\"" << std::endl;
    }
    if (only_required)
      continue;
    // check for missing optional fields
    std::vector<std::string> optional =
      Constants::get_optional_values(bEn.type);
    for (const std::string& current : optional) {
      bool has_optional_field = false;
      std::istringstream current_ss(current);
      for (std::string _current; std::getline(current_ss, _current, ' '); )
        if (!(get_field_value(bEn, _current).empty()))
          has_optional_field = true;
      if (!has_optional_field)
        std::cout << bEn.key << Strings::tr(Strings::OUT_MISSES_OPTIONAL)
          << current << "\"" << std::endl;
    }
  }
}


void Bibliography::show_missing_fields(std::vector<std::string> fields) const
{
  for (const bibEntry& bEn : bib) {
    for (std::string& current : fields) {
      if (get_field_value(bEn, current).empty())
        std::cout << bEn.key << Strings::tr(Strings::OUT_MISSING_FIELD)
          << current << "\"" << std::endl;
    }
  }
}


void Bibliography::abbreviate_month()
{
  // try to find the correct abbreviation
  for (bibEntry& bEn : bib) {
    for (bibElement& bEl : bEn.element) {
      std::string field = bEl.field;
      std::transform(field.begin(), field.end(), field.begin(), ::tolower);
      if (field == "month")
        bEl.value = Constants::find_month_abbreviation(bEl.value);
    }
  }
}

