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

#include <cstdlib>
#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <boost/program_options.hpp>
#include "Bibliography.hpp"
#include "Strings.hpp"
#include "bibf.hpp"

namespace po = boost::program_options;

std::vector<std::string> separate_string(std::string s)
{
  std::vector<std::string> result;
  std::istringstream ss(s);
  std::string word;
  while (std::getline(ss, word, ','))
    result.push_back(word);
  return result;
}

void localize_strings()
{
  // read environment variable LANG
  std::string lang = std::getenv("LANG");
  // if LANG is not set use default language (english)
  if (lang.empty())
    return;
  // extract the part before _
  auto i = lang.find("_");
  // use default language if LANG has unknown format
  if (i == std::string::npos)
    return;
  lang = lang.substr(0, i);
  // set language
  Strings::set_locale(lang);
}

int main(int argc, char* argv[])
{
  try {
    // set language
    localize_strings();

    // visible command line options
    po::options_description visible(Strings::tr(Strings::OPT_USAGE));
    visible.add_options()
      ("output,o", po::value<std::string>(),
        Strings::tr(Strings::OPT_OUTPUT).c_str())
      ("create-keys,c", Strings::tr(Strings::OPT_CREATE).c_str())
      ("only,O", po::value<std::string>(),
        Strings::tr(Strings::OPT_ONLY).c_str())
      ("sort-bib,s", po::value<std::string>(),
        Strings::tr(Strings::OPT_SORT_BIB).c_str())
      ("sort-elements,S", Strings::tr(Strings::OPT_SORT_ELEMENTS).c_str())
      ("erase-field,e", po::value<std::string>(),
        Strings::tr(Strings::OPT_ERASE_FIELD).c_str())
      ("show-missing,m", po::value<char>()->implicit_value('R'),
        Strings::tr(Strings::OPT_SHOW_MISSING).c_str())
      ("missing-fields,M", po::value<std::string>(),
        Strings::tr(Strings::OPT_MISSING_FIELDS).c_str())
      ("change-case", po::value<std::string>()->default_value("L"),
        Strings::tr(Strings::OPT_CHANGE_CASE).c_str())
      ("linebreak", po::value<unsigned int>(),
        Strings::tr(Strings::OPT_LINEBREAK).c_str())
      ("intendation", po::value<std::string>(),
        Strings::tr(Strings::OPT_INTENDATION).c_str())
      ("delimiter", po::value<char>(),
        Strings::tr(Strings::OPT_DELIMITER).c_str())
      ("align-left", Strings::tr(Strings::OPT_ALIGN_LEFT).c_str())
      ("abbrev-month", Strings::tr(Strings::OPT_ABBREV_MONTH).c_str())
      ("new-entry,n", Strings::tr(Strings::OPT_NEW_ENTRY).c_str())
      ("help", Strings::tr(Strings::OPT_HELP).c_str())
      ("version", Strings::tr(Strings::OPT_VERSION).c_str())
    ;

    // hidden command line options
    po::options_description hidden;
    hidden.add_options()
      ("input-files", po::value< std::vector<std::string> >(),
        Strings::tr(Strings::OPT_INPUT).c_str())
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
      std::cout << visible << "\n";
      return 0;
    }

    // version message
    if (vm.count("version")) {
      std::cout << Strings::tr(Strings::OUT_VERSION);
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
    else if (vm.count("new-entry")) {
      bib.create_entry();
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
        std::cerr << Strings::tr(Strings::ERR_CHANGE_CASE);
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
        std::cerr << Strings::tr(Strings::ERR_DELIMITER) << delim << "\n";
    }

    // alignment
    if (vm.count("align-left")) {
      bool right_aligned = false;
      bib.set_alignment(right_aligned);
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

    // sort bibliography
    if (vm.count("sort-bib")) {
      std::vector<std::string> sort =
        separate_string( vm["sort-bib"].as<std::string>() );
      bib.sort_bib(sort);
    }

    // sort elements
    if (vm.count("sort-elements")) {
      bib.sort_elements();
    }

    // create keys
    if (vm.count("create-keys"))
      bib.create_keys();

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
    std::cerr << "error: " << e.what() << "\n";
    return 1;
  }
  catch(...) {
    std::cerr << "Unknown exception.\n";
    return 1;
  }

  return EXIT_SUCCESS;
}

