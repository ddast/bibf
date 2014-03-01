#include "Bibliography.hpp"
#include <boost/program_options.hpp>
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
    po::options_description visible("Usage: reformbib [OPTION]... [FILE]...");
    visible.add_options()
      ("create-keys,c", "create keys using last name of first author plus last"
        " two digits of the year plus [a,b,c...]")
      ("only,O", po::value<std::string>(),
        "print only the given fields;"
        " different fields must be separated by commas")
      ("sort-bib,s", po::value<std::string>(), 
        "sorts the bibliography by the given citeria; valid values are 'type',"
        " 'key', 'firstauthor' and every string used as field identifier;"
        " different values must be separated by commas")
      ("erase-field,e", po::value<std::string>(),
        "erase the field in every entry;"
        " use comma to apply more than one value")
      ("show-missing,m", po::value<char>()->implicit_value('R'),
        "show missing required fields and also missing optional fields with"
        " option O")
      ("missing-fields,M", po::value<std::string>(),
        "show all entries that do not contain a specific field;"
        " use commas to search for more than one field")
      ("change-case", po::value<char>(), "change the case of all field"
        " identifiers to lower (L) or upper (U) case")
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
      cout << "reformbib 0.1\n";
      return 0;
    }

    // create empty Bibliography
    Bibliography bib;

    // input file
    if (vm.count("input-files")) {
      std::vector<std::string> filenames =
        vm["input-files"].as< std::vector<std::string> >();
      for (std::string &filename : filenames)
        bib.add_file(filename);
    }
    else {
      cerr << "No input files given\n";
      return 1;
    }

    // change case of field ids
    if (vm.count("change-case"))
      bib.change_field_case(vm["change-case"].as<char>());

    // linebreak
    if (vm.count("linebreak"))
      bib.set_linebreak(vm["linebreak"].as<int>());

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
    if (vm.count("only"))
      bib.print_bib( separate_string(vm["only"].as<std::string>()) );

    // standard action, print bib
    bib.print_bib();

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

