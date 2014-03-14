#include "Constants.hpp"

using std::string;

typedef std::vector<int> veci;
typedef std::pair<veci, veci> pveci;

const std::array<std::string, 27> Constants::standard_entry_fields {{
  /*00*/ "address",
  /*01*/ "annote",
  /*02*/ "author",
  /*03*/ "booktitle",
  /*04*/ "chapter",
  /*05*/ "crossref",
  /*06*/ "edition",
  /*07*/ "editor",
  /*08*/ "howpublished",
  /*09*/ "institution",
  /*10*/ "journal",
  /*11*/ "key",
  /*12*/ "month",
  /*13*/ "note",
  /*14*/ "number",
  /*15*/ "organization",
  /*16*/ "pages",
  /*17*/ "publisher",
  /*18*/ "school",
  /*19*/ "series",
  /*20*/ "title",
  /*21*/ "type",
  /*22*/ "volume",
  /*23*/ "year",
  /*24*/ "author editor",
  /*25*/ "volume number",
  /*26*/ "chapter pages"
}};

const std::map<string, pveci> Constants::standard_types {
  {"article", pveci(veci{2, 20, 10, 23}, veci{22, 14, 16, 12, 13})},
  {"book", pveci(veci{24, 20, 17, 23}, veci{25, 19, 0, 6, 12, 13})},
  {"booklet", pveci(veci{20}, veci{0, 8, 0, 12, 23, 13})},
  {"inbook", pveci(veci{24, 20, 26, 17, 23}, veci{25, 19, 21, 0, 6, 12, 13})},
  {"incollection", pveci(veci{2, 20, 3, 23},
      veci{7, 25, 19, 16, 0, 12, 15, 17, 13})},
  {"inproceedings", pveci(veci{2, 20, 3, 23},
      veci{7, 25, 19, 16, 0, 12, 15, 17, 13})},
  {"manual", pveci(veci{20}, veci{2, 16, 0, 6, 12, 23, 13})},
  {"masterthesis", pveci(veci{2, 20, 18, 23}, veci{21, 0, 12, 13})},
  {"misc", pveci(veci{}, veci{2, 20, 8, 12, 23, 13})},
  {"phdthesis", pveci(veci{2, 20, 18, 23}, veci{21, 0, 12, 13})},
  {"proceedings", pveci(veci{20, 23}, veci{7, 25, 19, 0, 17, 13, 12, 15})},
  {"techreport", pveci(veci{2, 20, 9, 23}, veci{21, 14, 0, 12, 13})},
  {"unpublished", pveci(veci{2, 20, 13}, veci{12, 23})}
};

const std::array<std::string, 12> Constants::month_abbreviations {{
  "jan", "feb", "mar", "apr", "may", "jun",
  "jul", "aug", "sep", "oct", "nov", "dec"
}};


std::vector<string> Constants::get_required_values(std::string type)
{
  std::transform(type.begin(), type.end(), type.begin(), ::tolower);
  std::vector<string> required;
  auto it = standard_types.find(type);
  if (it != standard_types.end())
    for (int i : it->second.first)
      required.push_back(standard_entry_fields[i]);
  return required;
}

std::vector<string> Constants::get_optional_values(std::string type)
{
  std::transform(type.begin(), type.end(), type.begin(), ::tolower);
  std::vector<string> optional;
  auto it = standard_types.find(type);
  if (it != standard_types.end())
    for (int i : it->second.second)
      optional.push_back(standard_entry_fields[i]);
  return optional;
}

bool Constants::is_valid_month_abbreviation(const std::string& s)
{
  for (const std::string& abbrev : month_abbreviations) 
    if (abbrev == s)
      return true;
  return false;
}

std::string Constants::find_month_abbreviation(const std::string& s)
{
  std::string _s = s.substr(0, 3);
  std::transform(_s.begin(), _s.end(), _s.begin(), ::tolower);
  for (const std::string& abbrev : month_abbreviations)
    if (abbrev == _s)
      return _s;
  return s;
}

