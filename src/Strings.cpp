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

#include "Strings.hpp"

// program version
const std::string Strings::version = "0.1.1";

// Standard value of current_lang
Strings::LANG Strings::current_lang = Strings::LANG_EN;

const std::unordered_map<std::string, Strings::LANG> Strings::lang_map {
  {"en", Strings::LANG_EN},
  {"de", Strings::LANG_DE}
};

// English
const std::array<std::string, Strings::STR_CNT> Strings::en {{
  "Usage: bibf [OPTION]... [FILE]...",
  "print to file instead of cout",
  "create keys using last name of first author plus last"
    " two digits of the year plus [a,b,c...]",
  "print only the given fields;"
    " different fields must be separated by commas",
  "sort the bibliography by the given citeria; valid values are 'type',"
    " 'key', 'firstauthor' and every string used as field identifier;"
    " different values must be separated by commas",
  "sort the elements of each entry alphabetically",
  "erase the field in every entry; use comma to apply more than one value",
  "show missing required fields (R) and also missing optional fields (O);"
   " R is assumend if invoked without an argument",
  "show all entries that do not contain a specific field;"
    " use commas to search for more than one field",
  "change the case of all types and keys to lower (L), UPPER (U) or"
    " Start (S) case (default L); for different cases of types and fields"
    " use two characters (LU, UL, ...)",
  "break lines after given amount of characters (default 79)",
  "change intendation for pretty printing (default two spaces)",
  "set field delimiter, valid values are { or \"",
  "use left instead of right alignment",
  "try to find the correct abbreviation of the month",
  "interactively create a new bibtex entry",
  "display this help and exit",
  "output version information and exit",
  "bibtex files for input",
  "bibf " + version + " Copyright (C) 2014 Dennis Dast\n\n"
    "This program comes with ABSOLUTELY NO WARRANTY.\n"
    "This is free software, and you are welcome to redistribute it"
    " under certain conditions.\n",
  " misses required field \"",
  " misses optional field \"",
  " misses field \"",
  "Required fields:\n",
  "Add optional fields? [Y,n]\n",
  "Unknown type\n",
  "Add user-defined fields (use empty input to exit)\n",
  "Malformed option '--change-case', valid options"
    " are one or two characters.\n",
  "Illegal field delimiter: ",
  "Author field is empty",
  "Warning: Key \"",
  "\" defined more than once\n",
  "Bibliography::create_keys(): Author ",
  " has more than 26 entries in the year ",
  ". Ran out of identifiers for creating the keys.",
  "Bibliography::change_case : case_t must be 'U', 'L' or 'S' but is ",
  "Bibliography::change_case : case_f must be 'U', 'L' or 'S' but is ",
  "Warning: set_field_delimiter called with illegal "
    "character as begin field delimiter: ",
  "Giving a type is mandatory\n",
  "Warning: set_field_delimiter called with illegal "
    "character as end field delimiter: "
}};

// German
const std::array<std::string, Strings::STR_CNT> Strings::de {{
  "Verwendung: bibf [OPTION]... [DATEI]...",
  "schreibe in Datei anstatt auf cout",
  "erstelle Schlüssel aus dem Nachnamen des ersten Autors plus den letzten"
    " zwei Stellen des Jahres plus [a,b,c...]",
  "gebe nur die angegeben Felder aus;"
    " mehrere Felder werden durch Kommas getrennt",
  "sortiere die Bibliothek nach den angegebenen Kriterien; mögliche Werte sind"
    " 'type', 'key', 'firstauthor' und jeder Name eines Feldes;"
    " verschiedene Werte müssen durch Kommas getrennt werden",
  "sortiere die Elemente jedes Eintrags alphabetisch",
  "lösche das Feld in jedem Eintrag; mit Komma getrennt können mehrere Werte"
    " angegeben werden",
  "zeige die fehlenden Pflichtfelder an (R) und zusätzlich fehlende optionale"
   " Felder (O); falls die Option ohne Argument aufgerufen wird, wird R"
   " verwendet", 
  "zeige alle Einträge, die ein bestimmtes Feld nicht enthalten;"
    "mit Kommas kann mehr als ein Feld angegeben werden",
  "ändere Groß-/Kleinschreibung der Typen und Schlüssel zu"
    " kleinschreibung (L), GROSSSCHREIBUNG (U) oder Großen"
    " Anfangsbuchstaben (S) (Standard L); für verschiedene Optionen von"
    " Typ und Schlüssel können zwei Buchstaben übergeben werden"
    " (LU, UL, ...)",
  "breche nach dieser Zahl von Ziffern um (Standard 79)",
  "legt Einrückung fest (Standard zwei Leerzeichen)",
  "legt Feldtrenner fest, mögliche Werte sind { oder \"",
  "versucht die korrekte Abkürzung für den Monat zu finden",
  "verwende linke statt rechte Ausrichtung",
  "erzeuge interaktiv einen neuen bibtex Eintrag",
  "zeige diese Hilfe an",
  "zeige Versionsinformationen an",
  "bibtex Dateien zum Einlesen",
  "bibf " + version + " Copyright (C) 2014 Dennis Dast\n\n"
    "This program comes with ABSOLUTELY NO WARRANTY.\n"
    "This is free software, and you are welcome to redistribute it"
    " under certain conditions.\n",
  " fehlt das Pflichtfeld \"",
  " fehlt das optionale feld \"",
  " fehlt das Feld \"",
  "Pflichtfelder:\n",
  "Optionale Felder hinzufügen? [Y,n]\n",
  "Unbekannter Typ\n",
  "Füge benutzerdefinierte Felder hinzu (leere Eingabe bricht ab)\n",
  "Unbekannte Option für '--change-case', mögliche Werte sind ein oder zwei"
    " Zifferns.\n",
  "Nicht erlaubtes Zeichen für Feldtrennung: ",
  "Feld 'author' ist leer",
  "Warnung: Schlüssel \"",
  "\" mehr als einmal definiert\n",
  "Bibliography::create_keys(): Autor ",
  " hat mehr als 26 Einträge im Jahr ",
  ". Keine eindeutige Schlüsselerzeugung möglich.",
  "Bibliography::change_case : case_t muss 'U', 'L' oder 'S' sein, aber ist ",
  "Bibliography::change_case : case_f muss 'U', 'L' oder 'S' sein, aber ist ",
  "Warnung: set_field_delimiter mit unerlaubtem Zeichen als Feldbeginnzeichen"
    " aufgerufen: ",
  "Es muss ein bibtex Typ angegeben werden\n",
  "Warnung: set_field_delimiter mit unerlaubtem Zeichen als Feldendzeichen"
    " aufgerufen: "
}};

const std::array<std::array<std::string, Strings::STR_CNT>, Strings::LANG_CNT>
    Strings::langs {{
  Strings::en,
  Strings::de
}};

void Strings::set_locale(std::string lang)
{
  // if lang is unknown use english
  try {
    current_lang = lang_map.at(lang);
  }
  catch(std::out_of_range& e) {
    current_lang = LANG_EN;
  }
}
