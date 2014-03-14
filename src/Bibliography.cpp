#include "Bibliography.hpp"
#include "Constants.hpp"

using std::istream;
using std::string;
using std::stringstream;

istream& Bibliography::get_block(istream& is, string& str) const
{
    stringstream block;
    int depth = 1;
    for (char c; is.get(c);) {
      if (c == '{')
        ++depth;
      else if ((c == '}') && !--depth)
        break;
      block << c;
    }
    str = block.str();
    return is;
}

istream& Bibliography::get_unnested(istream& is, string& str) const
{
  stringstream unnested;
  int depth = 0;
  for (char c; is.get(c);) {
    if (c == '{')
      ++depth;
    else if (c == '}')
      --depth;
    else if ((c == ',') && !depth)
      break;
    unnested << c;
  }
  str = unnested.str();
  return is;
}

string Bibliography::trim(const string& str, const string& trimchar) const
{
  size_t first = str.find_first_not_of(trimchar);
  if (first == string::npos)
    return "";
  size_t last = str.find_last_not_of(trimchar);
  return str.substr(first, last-first+1);
}

string Bibliography::clean_string(string str, const string& delchar) const
{
  // Replace characters in 'delchar' with spaces
  for (char& c1 : str)
    for (char c2 : delchar)
      if (c1 == c2)
        c1 = ' ';

  // Remove double spaces
  auto del_from = std::unique(str.begin(), str.end(),
      [] (char c1, char c2) { return (c1 == c2 && c1 == ' '); } );
  str.erase(del_from, str.end());

  return str;
}

string Bibliography::clean_key(string key) const
{
  // allowed characters in key
  string allowed = "abcdefghijklmnopqrstuvwxyz"
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

istream& Bibliography::get_bibEntry(istream& is, bibEntry& bEn) const
{
  // Discard everything bevore the first @
  string tmp;
  std::getline(is, tmp, '@');

  // get type
  std::getline(is, bEn.type, '{');  

  // save block in stringstream
  string bEn_s;
  get_block(is, bEn_s);
  stringstream bEn_ss(bEn_s);

  // create bibEntry
  std::getline(bEn_ss, bEn.key, ',');
  while (true) {
    string bEl_s;
    bool last(false);
    // get one line ending with ',' however last line may not end with ','
    if (!get_unnested(bEn_ss, bEl_s)) {
      if (trim(bEl_s).empty()) break;
      else last = true;
    }
    // extract element
    stringstream bEl_ss(bEl_s);
    bibElement bEl;
    // field is the part before '='
    std::getline(bEl_ss, bEl.field, '=');
    bEl.field = trim(bEl.field);
    // value may be in {} or "" or without delimiter
    char delim = 
      bEl_ss.str()[bEl_ss.str().find_first_not_of(" \t\n", bEl_ss.tellg())];
    std::getline(bEl_ss, tmp, delim);
    if (delim == '{')
      get_block(bEl_ss, bEl.value);
    else if (delim == '"')
      std::getline(bEl_ss, bEl.value, delim);
    else {
      bEl_ss.unget();
      std::getline(bEl_ss, bEl.value);
    }
    bEl.value = clean_string(trim(bEl.value));
    bEn.element.push_back(bEl);
    if (last) break;
  }

  return is;
}

string Bibliography::get_lastname(string author) const
{
  // get the first author
  size_t found = author.find("and");
  if (found != string::npos)
    author = author.substr(0, found);
  author = trim(author);

  // get last name
  found = author.find(",");
  if (found != string::npos)
    author = author.substr(0, found);
  else {
    stringstream author_ss(author);
    while (author_ss >> author);
  }


  if (author.empty()) {
    std::cerr << "Author field is empty" << std::endl;
    return "";
  }

  // first character upper case, rest lower case
  std::transform(author.begin()+1, author.end(), author.begin()+1, ::tolower);
  std::toupper(author.at(0));

  // return last name of first author
  return author;
}

void Bibliography::print_bib() const
{
  std::vector<string> empty;
  print_bib(empty);
}

bool Bibliography::is_numerical(const string& s) const
{
  size_t found = s.find_first_not_of("1234567890");
  return (found==string::npos);
}


void Bibliography::print_bib(std::vector<string> print_only) const
{
  bool print_all = print_only.empty();
  // convert list to lowercase
  if (!print_all)
    for (string& s : print_only)
      std::transform(s.begin(), s.end(), s.begin(), ::tolower);
  // key
  for (const bibEntry& bEn : bib) {
    std::cout << '@' << bEn.type << '{' << bEn.key;
    // elements
    for (const bibElement& bEl : bEn.element) {
      // print only if field is in 'print_only' or if 'print_all' is set
      bool do_print = print_all;
      string field = bEl.field;
      std::transform(field.begin(), field.end(), field.begin(), ::tolower);
      if (!print_all) {
        for (const string& po : print_only)
          if (po == field) {
            do_print = true;
            break;
          }
      }
      if (!do_print) continue;
      std::cout << ",\n";
      // Use no field delimiters if value is a numeric
      bool print_delimiter = true;
      if (is_numerical(bEl.value))
        print_delimiter = false;
      // Or if the month field uses three-letter abbreviations
      if ( (field == "month") &&
          Constants::is_valid_month_abbreviation(bEl.value) )
        print_delimiter = false;
      string line = intend + bEl.field + " = ";
      if (print_delimiter)
        line += field_beg + bEl.value +  field_end;
      else
        line += bEl.value;
      // break after 'linebreak' characters and use double intend in next line
      if (line.length() >= linebreak)
        for (unsigned int i = line.length()-1; i > intend.length(); --i)
          if ((line[i] == ' ') && (i <= linebreak+1)) {
            line[i] = '\n';
            line.insert(i+1, intend+intend);
            break;
          }
      std::cout << line;
    }
    // finish entry
    std::cout << "\n}\n" << std::endl;
  }
}

Bibliography::Bibliography() :
  intend("  "),
  linebreak(79),
  field_beg('{'),
  field_end('}')
{ }

void Bibliography::add(istream &is)
{
  // add the stream to the bibliography
  for (bibEntry bE; get_bibEntry(is, bE); bE.element.clear())
    bib.push_back(bE);

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
        std::cerr << "Warning: Key \"" << it1->key
          << "\" defined more than once\n";
        is_consistent = false;
      }
    }
  }

  return is_consistent;
}

string Bibliography::get_field_value(const bibEntry &bE, string field) const
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
    string author = get_lastname(get_field_value(*it, "author"));
    string year = get_field_value(*it, "year");
    if (year.length() >= 2)
      year = year.substr(year.length()-2, 2);
    it->key = author + year;
    char id = 'a';
    for (auto it2 = bib.begin(); it2 != it; ++it2)
      if (it->key == it2->key.substr(0,it2->key.length()-1))
        ++id;
    if (id > 'z') {
      std::cerr << "Bibliography::create_keys(): Author " << author
        << " has more than 26 entries in the year " << year
        << ". Ran out of identifiers for creating the keys." << std::endl;
      break;
    }
    it->key += id;
    it->key = clean_key(it->key);
  }
}

void Bibliography::change_field_case(const char uplo)
{
  int (*touplo)(int);
  if (uplo == 'U')
    touplo = &std::toupper;
  else if (uplo == 'L')
    touplo = &std::tolower;
  else {
    std::cerr << "Bibliography::change_field_case : uplo must be 'U' or 'L' "
      << "but is " << uplo << std::endl;
    return;
  }
  for (bibEntry& bEn : bib)
    for (bibElement& bEl : bEn.element)
      std::transform(bEl.field.begin(), bEl.field.end(), bEl.field.begin(),
          touplo);
}

void Bibliography::erase_field(string field)
{
  std::transform(field.begin(), field.end(), field.begin(), ::tolower);
  auto compare = [&] (bibElement& el) -> bool {
    string cur = el.field;
    std::transform(cur.begin(), cur.end(), cur.begin(), ::tolower);
    if (cur == field) return true;
    else return false;
  };
  for (bibEntry& bEn : bib)
    bEn.element.erase( std::remove_if(bEn.element.begin(), bEn.element.end(),
        compare), bEn.element.end() );
}

void Bibliography::sort_bib(std::vector<string> criteria)
{
  // check if 'bE1' is smaller than 'bE2' using the given criteria
  auto cmp_after_criteria =
    [&] (const bibEntry& bE1, const bibEntry& bE2) -> bool {
      for (string& cur_crit : criteria) {
        std::transform(cur_crit.begin(), cur_crit.end(), cur_crit.begin(),
            ::tolower);
        int comp = 0;
        if (cur_crit == "type")
          comp = bE1.type.compare(bE2.type);
        else if (cur_crit == "key")
          comp = bE1.key.compare(bE2.key);
        else if (cur_crit == "firstauthor") {
          string str1 = get_field_value(bE1, "author");
          str1 = get_lastname(str1);
          string str2 = get_field_value(bE2, "author");
          str2 = get_lastname(str2);
          comp = str1.compare(str2);
        }
        else {
          string str1 = get_field_value(bE1, cur_crit);
          string str2 = get_field_value(bE2, cur_crit);
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

void Bibliography::set_field_delimiter(char beg, char end)
{
  // only {} and '' are valid delimiters but always set the delimiters
  field_beg = beg;
  field_end = end;
  if (!( (beg == '{') || (beg == '"') ))
    std::cerr << "Warning: set_field_delimiter called with illegal "
      << "character as begin field delimiter: " << beg << std::endl;
  if (!( (end == '}') || (end == '"') ))
    std::cerr << "Warning: set_field_delimiter called with illegal "
      << "character as end field delimiter: " << beg << std::endl;
}

void Bibliography::show_missing_fields(bool only_required) const
{
  // check for missing required fields
  for (const bibEntry& bEn : bib) {
    std::vector<string> required = Constants::get_required_values(bEn.type);
    for (const string& current : required) {
      bool has_required_field = false;
      std::istringstream current_ss(current);
      for (string _current; std::getline(current_ss, _current, ' '); )
        if (!(get_field_value(bEn, _current).empty()))
          has_required_field = true;
      if (!has_required_field)
        std::cout << bEn.key << " misses required field \"" << current << "\""
          << std::endl;
    }
    if (only_required)
      continue;
    // check for missing optional fields
    std::vector<string> optional = Constants::get_optional_values(bEn.type);
    for (const string& current : optional) {
      bool has_optional_field = false;
      std::istringstream current_ss(current);
      for (string _current; std::getline(current_ss, _current, ' '); )
        if (!(get_field_value(bEn, _current).empty()))
          has_optional_field = true;
      if (!has_optional_field)
        std::cout << bEn.key << " misses optional field \"" << current << "\""
          << std::endl;
    }
  }
}

void Bibliography::show_missing_fields(std::vector<string> fields) const
{
  for (const bibEntry& bEn : bib) {
    for (string& current : fields) {
      if (get_field_value(bEn, current).empty())
        std::cout << bEn.key << " misses field \"" << current << "\""
          << std::endl;
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

