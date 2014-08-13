# bibf

A simple bibtex pretty printer.

## Command line options

    Usage: bibf [OPTION]... [FILE]...:
      -o [ --output ] arg              print to file instead of cout
      -c [ --create-keys ]             create keys using last name of first author 
                                       plus last two digits of the year plus 
                                       [a,b,c...]
      -O [ --only ] arg                print only the given fields; different 
                                       fields must be separated by commas
      -s [ --sort-bib ] arg            sort the bibliography by the given citeria; 
                                       valid values are 'type', 'key', 
                                       'firstauthor' and every string used as field
                                       identifier; different values must be 
                                       separated by commas
      -S [ --sort-elements ]           sort the elements of each entry 
                                       alphabetically
      -e [ --erase-field ] arg         erase the field in every entry; use comma to
                                       apply more than one value
      -m [ --show-missing ] [=arg(=R)] show missing required fields (R) and also 
                                       missing optional fields (O); R is assumend 
                                       if invoked without an argument
      -M [ --missing-fields ] arg      show all entries that do not contain a 
                                       specific field; use commas to search for 
                                       more than one field
      --change-case arg (=L)           change the case of all types and keys to 
                                       lower (L), UPPER (U) or Start (S) case 
                                       (default L); for different cases of types 
                                       and fields use two characters (LU, UL, ...)
      --linebreak arg                  break lines after given amount of characters
                                       (default 79)
      --intendation arg                change intendation for pretty printing 
                                       (default two spaces)
      --delimiter arg                  set field delimiter, valid values are { or "
      --align-left                     use left instead of right alignment
      --abbrev-month                   try to find the correct abbreviation of the 
                                       month
      -n [ --new-entry ]               interactively create a new BibTeX entry
      --help                           display this help and exit
      --version                        output version information and exit
    
