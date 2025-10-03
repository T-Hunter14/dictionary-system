#include <iostream>
#include <limits>
#include "enhanced_dictionary.h"

#ifdef _WIN32
#include <windows.h>
#endif

static void setupConsole() {
#ifdef _WIN32
    // Simple UTF-8 setup (no _setmode to avoid console issues)
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
#endif
    // Force auto-flush so prompts appear immediately in CLion Run
    std::cout.setf(std::ios::unitbuf);
}


static void printMenu() {
    std::cout << "\n========================================\n";
    std::cout << "     ENGLISH-ARABIC DICTIONARY\n";
    std::cout << "========================================\n";
    std::cout << " 1. Search word\n";
    std::cout << " 2. Arabic to English\n";
    std::cout << " 3. Autocomplete (prefix)\n";
    std::cout << " 4. Add new word\n";
    std::cout << " 5. Add meaning to word\n";
    std::cout << " 6. Delete meaning/word\n";
    std::cout << " 7. Statistics\n";
    std::cout << " 0. Exit\n";
    std::cout << "========================================\n";
    std::cout << ">> " << std::flush; // critical for CLion console
}

static std::string getLine(const std::string& prompt) {
    std::cout << prompt << std::flush;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

static void doSearch(Dictionary& d) {
    std::cout << "\n--- SEARCH WORD ---\n";
    std::string w = getLine("Word: ");
    Word* p = d.findWord(w);
    if (!p) { std::cout << "Not found.\n"; return; }
    std::cout << "\nWord: " << p->word << "\nMeanings:\n";
    for (size_t i = 0; i < p->meanings.size(); ++i) {
        std::cout << " [" << i << "] " << p->meanings[i].definition << "\n";
        if (!p->meanings[i].arabic.empty()) std::cout << "     Arabic: " << p->meanings[i].arabic << "\n";
    }
}

static void doArabic(Dictionary& d) {
    std::cout << "\n--- ARABIC TO ENGLISH ---\n";
    std::string a = getLine("Arabic: ");
    auto r = d.findEnglish(a);
    if (r.empty()) { std::cout << "No translation.\n"; return; }
    std::cout << "\nEnglish words:\n";
    for (size_t i = 0; i < r.size(); ++i) std::cout << " " << (i+1) << ". " << r[i] << "\n";
}

static void doAutocomplete(Dictionary& d) {
    std::cout << "\n--- AUTOCOMPLETE ---\n";
    std::string p = getLine("Prefix: ");
    auto r = d.autocomplete(p);
    if (r.empty()) { std::cout << "Nothing found.\n"; return; }
    for (size_t i = 0; i < r.size(); ++i) std::cout << " " << (i+1) << ". " << r[i] << "\n";
}

static void doAddWord(Dictionary& d) {
    std::cout << "\n--- ADD NEW WORD ---\n";
    std::string w = getLine("English word: ");
    if (d.findWord(w)) { std::cout << "Exists! Use option 5.\n"; return; }
    std::string m = getLine("Meaning: ");
    if (m.empty()) { std::cout << "Meaning required.\n"; return; }
    std::string a = getLine("Arabic (optional): ");
    std::cout << (d.addWord(w, m, a) ? "Added!\n" : "Failed.\n");
}

static void doAddMeaning(Dictionary& d) {
    std::cout << "\n--- ADD MEANING ---\n";
    std::string w = getLine("Word: ");
    if (!d.findWord(w)) { std::cout << "Not found. Use option 4.\n"; return; }
    std::string m = getLine("New meaning: ");
    if (m.empty()) { std::cout << "Meaning required.\n"; return; }
    std::string a = getLine("Arabic (optional): ");
    std::cout << (d.addMeaning(w, m, a) ? "Added!\n" : "Failed.\n");
}

static void doDelete(Dictionary& d) {
    std::cout << "\n--- DELETE ---\n";
    std::string w = getLine("Word: ");
    Word* p = d.findWord(w);
    if (!p) { std::cout << "Not found.\n"; return; }
    for (size_t i = 0; i < p->meanings.size(); ++i) std::cout << " [" << i << "] " << p->meanings[i].definition << "\n";
    std::cout << "\n-1 = delete entire word\n0-" << (p->meanings.size()-1) << " = delete meaning\n";
    std::cout << "Choice: " << std::flush;
    int c;
    if (!(std::cin >> c)) { std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); std::cout << "Invalid.\n"; return; }
    std::cin.ignore();
    std::cout << (d.deleteMeaning(w, c) ? "Deleted!\n" : "Failed.\n");
}

static void doStats(Dictionary& d) {
    std::cout << "\n--- STATS ---\n";
    std::cout << "Words: " << d.wordCount() << "\n";
    std::cout << "Meanings: " << d.meaningCount() << "\n";
}

int main() {
    setupConsole();

    std::cout << "Loading...\n";
    Dictionary dict;
    std::cout << "Ready! " << dict.wordCount() << " words.\n";

    int choice = -1;
    while (choice != 0) {
        printMenu();
        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        std::cin.ignore(); // eat newline

        switch (choice) {
            case 0: std::cout << "\nBye!\n"; break;
            case 1: doSearch(dict); break;
            case 2: doArabic(dict); break;
            case 3: doAutocomplete(dict); break;
            case 4: doAddWord(dict); break;
            case 5: doAddMeaning(dict); break;
            case 6: doDelete(dict); break;
            case 7: doStats(dict); break;
            default: std::cout << "Invalid.\n";
        }

        if (choice != 0) {
            std::cout << "\nPress Enter..." << std::flush;
            std::cin.get();
        }
    }
    return 0;
}
