#include <iostream>
#include <string>
#include <limits>
#include <iomanip>

#ifdef _WIN32
#include <windows.h>
#include <fcntl.h>
#include <io.h>
#endif

#include "enhanced_dictionary.h"

void setupConsole() {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    system("chcp 65001 > nul");
#endif
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void printLine() {
    std::cout << std::string(60, '=') << std::endl;
}

void printMenu() {
    std::cout << "\n";
    printLine();
    std::cout << "          ENGLISH-ARABIC DICTIONARY SYSTEM" << std::endl;
    printLine();
    std::cout << std::endl;
    std::cout << "  1. Search for word" << std::endl;
    std::cout << "  2. Autocomplete (prefix search)" << std::endl;
    std::cout << "  3. Get Arabic translation" << std::endl;
    std::cout << "  4. Search by meaning" << std::endl;
    std::cout << std::endl;
    std::cout << "  5. Add new word" << std::endl;
    std::cout << "  6. Add meaning to existing word" << std::endl;
    std::cout << "  7. Delete word/meaning" << std::endl;
    std::cout << std::endl;
    std::cout << "  8. Show statistics" << std::endl;
    std::cout << "  0. Exit" << std::endl;
    std::cout << std::endl;
    printLine();
    std::cout << "Choose option: ";
}

void pause() {
    std::cout << "\n\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

std::string getInput(const std::string& prompt) {
    std::cout << prompt;
    std::string input;
    std::getline(std::cin, input);
    return input;
}

void displayWord(const MultiWordEntry& entry) {
    if (entry.word.empty()) {
        std::cout << "\nWord not found." << std::endl;
        return;
    }

    std::cout << "\n";
    printLine();
    std::cout << "Word: " << entry.word << std::endl;
    printLine();

    for (size_t i = 0; i < entry.meanings.size(); ++i) {
        std::cout << "\n[" << (i + 1) << "] " << entry.meanings[i];

        if (i < entry.arabicTranslations.size() && !entry.arabicTranslations[i].empty()) {
            std::cout << "\n    Arabic: " << entry.arabicTranslations[i];
        }

        if (i < entry.partOfSpeeches.size() && !entry.partOfSpeeches[i].empty()) {
            std::cout << "\n    Type: " << entry.partOfSpeeches[i];
        }
        std::cout << std::endl;
    }
    printLine();
}

// Option 1: Search for word
void searchWord(EnhancedDictionary& dict) {
    std::cout << "\n--- SEARCH WORD ---" << std::endl;
    std::string word = getInput("Enter word: ");

    if (word.empty()) {
        std::cout << "Invalid input." << std::endl;
        return;
    }

    MultiWordEntry entry = dict.getWordEntry(word);
    displayWord(entry);
}

// Option 2: Autocomplete
void autocomplete(EnhancedDictionary& dict) {
    std::cout << "\n--- AUTOCOMPLETE ---" << std::endl;
    std::string prefix = getInput("Enter prefix: ");

    if (prefix.empty()) {
        std::cout << "Invalid input." << std::endl;
        return;
    }

    auto results = dict.searchByPrefix(prefix, 20);

    if (results.empty()) {
        std::cout << "\nNo words found." << std::endl;
        return;
    }

    std::cout << "\nFound " << results.size() << " words:\n" << std::endl;
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << results[i] << std::endl;
    }
}

// Option 3: Arabic translation
void getArabic(EnhancedDictionary& dict) {
    std::cout << "\n--- ARABIC TRANSLATION ---" << std::endl;
    std::string word = getInput("Enter word: ");

    if (word.empty()) {
        std::cout << "Invalid input." << std::endl;
        return;
    }

    std::string arabic = dict.getArabicTranslation(word);

    std::cout << "\nWord:   " << word << std::endl;
    std::cout << "Arabic: " << arabic << std::endl;
}

// Option 4: Search by meaning
void searchMeaning(EnhancedDictionary& dict) {
    std::cout << "\n--- SEARCH BY MEANING ---" << std::endl;
    std::string query = getInput("Enter search term: ");

    if (query.empty()) {
        std::cout << "Invalid input." << std::endl;
        return;
    }

    auto results = dict.searchByMeaning(query, 15);

    if (results.empty()) {
        std::cout << "\nNo matches found." << std::endl;
        return;
    }

    std::cout << "\nFound " << results.size() << " matches:\n" << std::endl;
    for (size_t i = 0; i < results.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << results[i].word;
        if (!results[i].meanings.empty()) {
            std::cout << " - " << results[i].meanings[0];
        }
        std::cout << std::endl;
    }
}

// Option 5: Add new word
void addWord(EnhancedDictionary& dict) {
    std::cout << "\n--- ADD NEW WORD ---" << std::endl;
    std::string word = getInput("Enter word: ");

    if (word.empty()) {
        std::cout << "Invalid input." << std::endl;
        return;
    }

    // Check if exists
    MultiWordEntry existing = dict.getWordEntry(word);
    if (!existing.word.empty()) {
        std::cout << "\nWord already exists:" << std::endl;
        displayWord(existing);
        std::cout << "\nUse option 6 to add another meaning." << std::endl;
        return;
    }

    std::string meaning = getInput("Enter meaning: ");
    if (meaning.empty()) {
        std::cout << "Meaning cannot be empty." << std::endl;
        return;
    }

    std::string pos = getInput("Enter type (noun/verb/adj): ");
    if (pos.empty()) pos = "noun";

    std::string arabic = getInput("Enter Arabic (or press Enter to skip): ");

    if (dict.addWord(word, meaning, arabic, pos)) {
        std::cout << "\nSUCCESS! Word added and saved." << std::endl;
        MultiWordEntry entry = dict.getWordEntry(word);
        displayWord(entry);
    } else {
        std::cout << "\nFailed to add word." << std::endl;
    }
}

// Option 6: Add meaning to existing word
void addMeaning(EnhancedDictionary& dict) {
    std::cout << "\n--- ADD MEANING TO WORD ---" << std::endl;
    std::string word = getInput("Enter word: ");

    if (word.empty()) {
        std::cout << "Invalid input." << std::endl;
        return;
    }

    // Check if exists
    MultiWordEntry existing = dict.getWordEntry(word);
    if (existing.word.empty()) {
        std::cout << "\nWord not found. Use option 5 to add it." << std::endl;
        return;
    }

    std::cout << "\nCurrent meanings:" << std::endl;
    displayWord(existing);

    std::string meaning = getInput("\nEnter new meaning: ");
    if (meaning.empty()) {
        std::cout << "Meaning cannot be empty." << std::endl;
        return;
    }

    std::string pos = getInput("Enter type (noun/verb/adj): ");
    if (pos.empty()) pos = "noun";

    std::string arabic = getInput("Enter Arabic (or press Enter to skip): ");

    if (dict.addMeaningToWord(word, meaning, arabic, pos)) {
        std::cout << "\nSUCCESS! Meaning added." << std::endl;
        MultiWordEntry updated = dict.getWordEntry(word);
        displayWord(updated);
    } else {
        std::cout << "\nFailed to add meaning." << std::endl;
    }
}

// Option 7: Delete word or meaning
void deleteWord(EnhancedDictionary& dict) {
    std::cout << "\n--- DELETE WORD/MEANING ---" << std::endl;
    std::string word = getInput("Enter word: ");

    if (word.empty()) {
        std::cout << "Invalid input." << std::endl;
        return;
    }

    MultiWordEntry entry = dict.getWordEntry(word);
    if (entry.word.empty()) {
        std::cout << "\nWord not found." << std::endl;
        return;
    }

    displayWord(entry);

    if (entry.meanings.size() == 1) {
        std::cout << "\nDelete entire word? (y/n): ";
        char confirm;
        std::cin >> confirm;
        std::cin.ignore();

        if (confirm == 'y' || confirm == 'Y') {
            if (dict.deleteWordMeaning(word, -1)) {
                std::cout << "\nWord deleted." << std::endl;
            } else {
                std::cout << "\nFailed to delete." << std::endl;
            }
        }
    } else {
        std::cout << "\nOptions:" << std::endl;
        std::cout << "  0 = Cancel" << std::endl;
        for (size_t i = 0; i < entry.meanings.size(); ++i) {
            std::cout << "  " << (i + 1) << " = Delete meaning " << (i + 1) << std::endl;
        }
        std::cout << "  " << (entry.meanings.size() + 1) << " = Delete entire word" << std::endl;

        std::cout << "\nChoice: ";
        int choice;
        std::cin >> choice;
        std::cin.ignore();

        if (choice == 0) {
            std::cout << "Cancelled." << std::endl;
        } else if (choice > 0 && choice <= static_cast<int>(entry.meanings.size())) {
            if (dict.deleteWordMeaning(word, choice - 1)) {
                std::cout << "\nMeaning deleted." << std::endl;
            } else {
                std::cout << "\nFailed to delete." << std::endl;
            }
        } else if (choice == static_cast<int>(entry.meanings.size()) + 1) {
            std::cout << "Confirm delete entire word? (y/n): ";
            char confirm;
            std::cin >> confirm;
            std::cin.ignore();

            if (confirm == 'y' || confirm == 'Y') {
                if (dict.deleteWordMeaning(word, -1)) {
                    std::cout << "\nWord deleted." << std::endl;
                } else {
                    std::cout << "\nFailed to delete." << std::endl;
                }
            }
        }
    }
}

// Option 8: Statistics
void showStats(EnhancedDictionary& dict) {
    std::cout << "\n--- STATISTICS ---\n" << std::endl;
    dict.printDetailedStatistics();
}

int main() {
    setupConsole();

    std::cout << "\nLoading dictionary...\n" << std::endl;

    EnhancedDictionary dictionary;
    dictionary.loadLargeDictionary();

    std::cout << "Dictionary loaded!" << std::endl;
    std::cout << "Total words: " << dictionary.getWordCount() << std::endl;

    int choice = -1;

    while (choice != 0) {
        printMenu();

        if (!(std::cin >> choice)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nInvalid input." << std::endl;
            continue;
        }
        std::cin.ignore();

        switch (choice) {
            case 0:
                std::cout << "\nGoodbye!" << std::endl;
                break;
            case 1:
                searchWord(dictionary);
                break;
            case 2:
                autocomplete(dictionary);
                break;
            case 3:
                getArabic(dictionary);
                break;
            case 4:
                searchMeaning(dictionary);
                break;
            case 5:
                addWord(dictionary);
                break;
            case 6:
                addMeaning(dictionary);
                break;
            case 7:
                deleteWord(dictionary);
                break;
            case 8:
                showStats(dictionary);
                break;
            default:
                std::cout << "\nInvalid option." << std::endl;
                break;
        }

        if (choice != 0) {
            pause();
        }
    }

    return 0;
}
