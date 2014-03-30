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
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

namespace po = boost::program_options;

using std::cout;
using std::cerr;
using std::endl;

std::vector<std::string> separate_string(std::string s)
{
  std::vector<std::string> result;
  std::istringstream ss(s);
  std::string word;
  while (std::getline(ss, word, ','))
    result.push_back(word);
  return result;
}

int main(int argc, char* argv[])
{
  try {
    // visible command line options
    po::options_description visible("Usage: bibf [OPTION]... [FILE]...");
    visible.add_options()
      ("output,o", po::value<std::string>(), "print to file instead of cout")
      ("create-keys,c", "create keys using last name of first author plus last"
        " two digits of the year plus [a,b,c...]")
      ("only,O", po::value<std::string>(),
        "print only the given fields;"
        " different fields must be separated by commas")
      ("sort-bib,s", po::value<std::string>(), 
        "sort the bibliography by the given citeria; valid values are 'type',"
        " 'key', 'firstauthor' and every string used as field identifier;"
        " different values must be separated by commas")
      ("sort-elements,S", "sort the elements of each entry alphabetically")
      ("erase-field,e", po::value<std::string>(),
        "erase the field in every entry;"
        " use comma to apply more than one value")
      ("show-missing,m", po::value<char>()->implicit_value('R'),
        "show missing required fields and also missing optional fields with"
        " option O")
      ("missing-fields,M", po::value<std::string>(),
        "show all entries that do not contain a specific field;"
        " use commas to search for more than one field")
      ("change-case", po::value<std::string>()->implicit_value("L"),
        "change the case of all types and keys to lower (L), upper (U) or"
        " start (S) case (default L); for different cases of types and cases"
        " use two characters (LU, UL, ...)")
      ("linebreak", po::value<unsigned int>(), "break lines after given"
        " amount of characters (default 79)")
      ("intendation", po::value<std::string>(),
        "change intendation for pretty printing (default two spaces)")
      ("delimiter", po::value<char>(), "set field delimiter, valid values"
        " are { or \"")
      ("abbrev-month", "try to find the correct abbreviation of the month")
      ("help", "display this help and exit")
      ("version", "output version information and exit")
    ;

    // hidden command line options
    po::options_description hidden;
    hidden.add_options()
      ("input-files", po::value< std::vector<std::string> >(),
       "bibtex files for input")
    ;
    // positional options are interpreted as input-file
    po::positional_options_description pod;
    pod.add("input-files", -1);

    // all options
    po::options_description opts;
    opts.add(visible).add(hidden);

    // create variables map
    po::variables_map vm;
    po::store(po::command_line_parser(argc, argv).
        options(opts).positional(pod).run(), vm);
    po::notify(vm);

    // help message
    if (vm.count("help")) {
      cout << visible << "\n";
      return 0;
    }

    // version message
    if (vm.count("version")) {
      cout << "bibf 0.1 Copyright (C) 2014 Dennis Dast\n\n"
        << "This program comes with ABSOLUTELY NO WARRANTY.\n"
        << "This is free software, and you are welcome to redistribute it"
        << " under certain conditions.\n";
      return 0;
    }

    // create empty Bibliography
    Bibliography bib;

    // input file
    if (vm.count("input-files")) {
      std::vector<std::string> filenames =
        vm["input-files"].as< std::vector<std::string> >();
      for (std::string &filename : filenames) {
        std::ifstream bibFile(filename.c_str());
        bib.add(bibFile);
        bibFile.close();
      }
    }
    else {
      bib.add(std::cin);
    }

    // set output file
    std::ofstream out;
    if (vm.count("output")) {
      out.open(vm["output"].as<std::string>());
    }

    // change case of field ids
    if (vm.count("change-case")) {
      std::string cases = vm["change-case"].as<std::string>();
      if (cases.length() == 1)
        bib.change_case(cases[0], cases[0]);
      else if (cases.length() == 2)
        bib.change_case(cases[0], cases[1]);
      else {
        std::cerr << "Malformatted option '--change-case', valid options"
          << " are one or two characters.\n";
        return 1;
      }
    }

    // linebreak
    if (vm.count("linebreak"))
      bib.set_linebreak(vm["linebreak"].as<unsigned int>());

    // intendation
    if (vm.count("intendation"))
      bib.set_intendation(vm["intendation"].as<std::string>());

    // delimiter
    if (vm.count("delimiter")) {
      char delim = vm["delimiter"].as<char>();
      if ((delim == '{') || (delim == '}'))
        bib.set_field_delimiter('{', '}');
      else if (delim == '"')
        bib.set_field_delimiter('"', '"');
      else
        std::cerr << "Illegal field delimiter: " << delim << "\n";
    }

    // abbreviate months
    if (vm.count("abbrev-month"))
      bib.abbreviate_month();

    // erase fields
    if (vm.count("erase-field")) {
      std::vector<std::string> erase_vec =
        separate_string(vm["erase-field"].as<std::string>());
      for (const std::string &erase : erase_vec)
        bib.erase_field(erase);
    }

    // create keys
    if (vm.count("create-keys"))
      bib.create_keys();

    // sort bibliography
    if (vm.count("sort-bib")) {
      std::vector<std::string> sort =
        separate_string( vm["sort-bib"].as<std::string>() );
      bib.sort_bib(vm["sort-bib"].as< std::vector<std::string> >());
    }

    // sort elements
    if (vm.count("sort-elements")) {
      bib.sort_elements();
    }

    // show missing fields
    if (vm.count("show-missing")) {
      char mode = vm["show-missing"].as<char>();
      bool only_required = true;
      if (mode == 'O')
        only_required = false;
      bib.show_missing_fields(only_required);
      return 0;
    }

    // show user defined missing fields
    if (vm.count("missing-fields")) {
      std::vector<std::string> fields =
        separate_string( vm["missing-fields"].as<std::string>() );
      bib.show_missing_fields(fields);
      return 0;
    }

    // print only given fields
    if (vm.count("only")) {
      std::vector<std::string> only =
        separate_string(vm["only"].as<std::string>());
      if (out.is_open())
        bib.print_bib( only, out);
      else
        bib.print_bib(only, std::cout);

      return 0;
    }

    // standard action, print bib
    if (out.is_open())
      bib.print_bib(out);
    else
      bib.print_bib(std::cout);

    // close output stream
    if (out.is_open())
      out.close();

  }
  catch(std::exception& e) {
    cerr << "error: " << e.what() << "\n";
    return 1;
  }
  catch(...) {
    cerr << "Unknown exception.\n";
    return 1;
  }

  return EXIT_SUCCESS;
}

