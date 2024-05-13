#include <iostream>
#include <vector>
#include <string>
#include "BST.h"
#include "HashTable.h"
#include <fstream>
#include <chrono>
#include <unordered_map>
#include <cctype>

//CS300-HW3 CAGRI CAN SARACAYDIN 30984

using namespace std;

class DocumentItem {
public:
    string documentName;
    int count;
    DocumentItem(string docName) : documentName(docName), count(1) {}
};

class WordItem {
public:
    string word;
    vector<DocumentItem> documents; // list of document items
    WordItem(string w) : word(w) {}
    bool operator < (const WordItem& other) const {
        return word < other.word; // Sort alphabetically by word
    }
    bool operator > (const WordItem& other) const {
        return word > other.word; // Sort alphabetically by word
    }
    void addDocument(const string& documentName) {
        for (auto& document : documents) {
            if (document.documentName == documentName) {
                document.count++;
                return;
            }
        }
        documents.emplace_back(documentName);
    }
};

void lowerCase(string& str) {
    for (char &c : str) {
        c = tolower(c);
    }
}

vector<string> extractWords(const string& line) {

    vector<string> words; // to store words
    string word;
    for (char ch : line) {
        if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) { // if it's a letter, add to the word
            word += ch;
        }
        else if (!word.empty()) { // if it's not a letter, the word ended and a new one started
            words.push_back(word);
            word.clear();
        }
    }
    if (!word.empty()) { // last word in the line
        words.push_back(word);
    }
    return words; // return modified word vector
}

void updateBST(BST<string, WordItem*>& tree, const string& word, const string& documentName) {

    WordItem* existingWordPtr = tree.search(word);
    if (existingWordPtr != nullptr) { // if word already exists just update its documentitem
        existingWordPtr->addDocument(documentName); // if not returnd then documentitem does not exist, add
    }
    else { // word does not exist create its worditem documentitem then insert
        WordItem* newWordItem = new WordItem(word);
        newWordItem->addDocument(documentName);
        tree.insert(word, newWordItem);
    }
}
void updateHashTable(HashTable<string, WordItem*>& table, const string& word, const string& documentName) {

    WordItem* existingWordPtr = table.search(word);
    if (existingWordPtr != nullptr) { // if word already exists just update its documentitem
        existingWordPtr->addDocument(documentName);
    }
    else { // word does not exist, create its worditem and documentitem then insert
        WordItem* newWordItem = new WordItem(word);
        newWordItem->addDocument(documentName);
        table.insert(word, newWordItem);
    }
}
void insertSorted(vector<WordItem>& items, const string& word, const string& documentName) {
    // Check if word already exists
    for (auto& item : items) {
        if (item.word == word) {
            item.addDocument(documentName);
            return;
        }
    }
    // If word does not exist, create and insert it
    WordItem newItem(word);
    newItem.addDocument(documentName);
    items.push_back(newItem);

    // Perform insertion sort
    for (int i = items.size() - 1; i > 0; i--) {
        if (items[i] < items[i - 1]) {
            swap(items[i], items[i - 1]);
        } else {
            break; // Already sorted
        }
    }
}

void processDocument(BST<string, WordItem*>& tree,HashTable<string, WordItem*>& table, vector<WordItem>& items, const string& documentName, vector<string>& allDocuments) { //BST

    allDocuments.push_back(documentName); // store the name of txt
    ifstream file(documentName); // open
    string line;

    while (getline(file, line)) {
        vector<string> words = extractWords(line); // get words from txt
        for (const string& word : words) {
            updateBST(tree, word, documentName); // add or increment the tree
            updateHashTable(table, word, documentName); // update table
            insertSorted(items, word, documentName);
        }
    }

    file.close();
}

int binarySearch(const vector<WordItem>& items, const string& word) {
    int low = 0;
    int high = items.size() - 1;

    while (low <= high) {
        int mid = low + (high - low) / 2;
        if (items[mid].word == word) {
            return mid;             //  found
        } else if (items[mid].word < word) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }
    return -1; //  not found
}

int main() {

    vector<string> allDocuments1; // FOR BST
    vector<string> fileNames; // to store the input txt's name

    int noFiles;
    cout << "Enter number of input files: ";
    cin >> noFiles;
    cin.ignore();

    BST<string, WordItem*> myTree;
    HashTable<string, WordItem*> myHashTable;
    vector<WordItem> wordItInsertSort; // insert sorted vector

    for (unsigned int i = 0; i < noFiles; ++i) {
        string fileName;
        cout << "Enter " << i + 1 << ". file name: ";
        getline(cin, fileName);
        fileNames.push_back(fileName);
    }

    for (const string& fileName : fileNames) { // create the BST and HASHTABLE for each txt
        processDocument(myTree, myHashTable,wordItInsertSort, fileName, allDocuments1);
    }

    string line;
    cout << "Enter queried words in one line: ";
    getline(cin, line);
    lowerCase(line); // case insensitive

    vector<string> queryWords = extractWords(line);

    bool sameDoc = false;

    for (unsigned int docIndex = 0; docIndex < allDocuments1.size(); ++docIndex) {
        vector<string> successOutput;
        bool allWordsFound = true;

        for (const string& word : queryWords) {
            int index = binarySearch(wordItInsertSort, word);
            bool wordFound = false;

            if (index != -1) {
                for (const DocumentItem& doc : wordItInsertSort[index].documents) {
                    if (doc.documentName == allDocuments1[docIndex]) {
                        string output = word + " found " + to_string(doc.count) + " times";
                        successOutput.push_back(output);
                        wordFound = true;
                        break;
                    }
                }
            }

            if (!wordFound) {
                allWordsFound = false;
                break;
            }
        }

        if (allWordsFound && !successOutput.empty()) {
            sameDoc = true;
            cout << "In Document " << allDocuments1[docIndex] << ", ";
            for (unsigned int i = 0; i < successOutput.size(); ++i) {
                cout << successOutput[i];
                if (i < successOutput.size() - 1) {
                    cout << ", ";
                }
            }
            cout << "." << endl;
        }
    }

    if (!sameDoc) {
        cout << "No document contains the given query" << endl;
    }


    sameDoc = false;

    // GIVE THE OUTPUT USING BST
    for (unsigned int docIndex = 0; docIndex < allDocuments1.size(); ++docIndex) { // for each doc check if query words exists together

        vector<string> successOutput; // to store output
        bool allWordsFound = true;

        for (unsigned int wordIndex = 0; wordIndex < queryWords.size(); ++wordIndex) { // iterate for each query words

            WordItem* searchedWordItem = myTree.search(queryWords[wordIndex]);
            bool wordFound = false;

            if (searchedWordItem != nullptr) { // found

                for (unsigned int itemIndex = 0; itemIndex < searchedWordItem->documents.size(); ++itemIndex) { // check each docs where it exists

                    if (searchedWordItem->documents[itemIndex].documentName == allDocuments1[docIndex]) { // if exist in current doc

                        string output = queryWords[wordIndex] + " found " + to_string(searchedWordItem->documents[itemIndex].count) + " times";
                        successOutput.push_back(output);
                        wordFound = true;
                        break; // move to next word
                    }
                }
            }

            if (!wordFound) { // if anyone of the word not exist skip

                allWordsFound = false;
                break;
            }
        }

        if (allWordsFound && !successOutput.empty()) { // success now print

            sameDoc = true;
            cout << "in Document " << allDocuments1[docIndex] << ", ";
            for (unsigned int i = 0; i < successOutput.size(); ++i) {

                cout << successOutput[i];

                if (i < successOutput.size() - 1) {
                    cout << ", ";
                }
            }
            cout << "." << endl;
        }
    }

    if (!sameDoc) {
        cout << "No document contains the given query" << endl;
    }

    sameDoc = false;

    // GIVE OUTPUT USING HASHTABLE, SAME LOGIC WITH BST
    for (unsigned int docIndex = 0; docIndex < allDocuments1.size(); ++docIndex) {

        string currentDoc = allDocuments1[docIndex];
        vector<string> successOutput;
        bool allWordsFound = true;

        for (unsigned int wordIndex = 0; wordIndex < queryWords.size(); ++wordIndex) {

            WordItem* wordItem = myHashTable.search(queryWords[wordIndex]);
            bool wordFound = false;

            if (wordItem != nullptr) {

                for (unsigned int docItemIndex = 0; docItemIndex < wordItem->documents.size(); ++docItemIndex) {

                    DocumentItem& docItem = wordItem->documents[docItemIndex];
                    if (docItem.documentName == currentDoc) {

                        string output = queryWords[wordIndex] + " found " + to_string(docItem.count) + " times";
                        successOutput.push_back(output);
                        wordFound = true;
                        break;
                    }
                }
            }

            if (!wordFound) {

                allWordsFound = false;
                break;
            }
        }

        if (allWordsFound && !successOutput.empty()) {

            sameDoc = true;
            cout << "in Document " << currentDoc << ", ";
            for (unsigned int i = 0; i < successOutput.size(); ++i) {
                cout << successOutput[i];
                if (i < successOutput.size() - 1) {
                    cout << ", ";
                }
            }
            cout << "." << endl;
        }
    }

    if (!sameDoc) {
        cout << "No document contains the given query" << endl;
    }


    // TIME BENCHMARK: DOES NOT PRODUCE OUTPUT
    int k = 100;
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
    // QueryDocuments(with BST);

        // SEARCH QUERY checking doc by doc

        bool sameDoc = false;

        for (unsigned int docIndex = 0; docIndex < allDocuments1.size(); ++docIndex) { // for each doc check if query words exists together

            vector<string> successOutput; // to store output
            bool allWordsFound = true;

            for (unsigned int wordIndex = 0; wordIndex < queryWords.size(); ++wordIndex) { // iterate for each query words

                WordItem* searchedWordItem = myTree.search(queryWords[wordIndex]);
                bool wordFound = false;

                if (searchedWordItem != nullptr) { // found

                    for (unsigned int itemIndex = 0; itemIndex < searchedWordItem->documents.size(); ++itemIndex) { // check each docs where it exists

                        if (searchedWordItem->documents[itemIndex].documentName == allDocuments1[docIndex]) { // if exist in current doc

                          //  string output = queryWords[wordIndex] + " found " + to_string(searchedWordItem->documents[itemIndex].count) + " times";
                          //  successOutput.push_back(output);
                            wordFound = true;
                            break; // move to next word
                        }
                    }
                }

                if (!wordFound) { // if anyone of the word not exist skip

                    allWordsFound = false;
                    break;
                }
            }

            if (allWordsFound && !successOutput.empty()) { // success now print

                sameDoc = true;
                //cout << "in Document " << allDocuments[docIndex] << ", ";
                for (unsigned int i = 0; i < successOutput.size(); ++i) {

                   // cout << successOutput[i];

                    if (i < successOutput.size() - 1) {
                      //  cout << ", ";
                    }
                }
                //cout << "." << endl;
            }
        }

        if (!sameDoc) {

        }
        
    }


    auto BSTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);
    cout << "\nTime: " << BSTTime.count() / k << "\n";
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; i++)
    {
    // QueryDocuments (with hashtable);

       // same logic with BST query

            for (unsigned int docIndex = 0; docIndex < allDocuments1.size(); ++docIndex) {

                string currentDoc = allDocuments1[docIndex];
                vector<string> successOutput;
                bool allWordsFound = true;

                for (unsigned int wordIndex = 0; wordIndex < queryWords.size(); ++wordIndex) {

                    string currentWord = queryWords[wordIndex];
                    WordItem* wordItem = myHashTable.search(currentWord);
                    bool wordFound = false;

                    if (wordItem != nullptr) {

                        for (unsigned int docItemIndex = 0; docItemIndex < wordItem->documents.size(); ++docItemIndex) {

                            DocumentItem& docItem = wordItem->documents[docItemIndex];
                            if (docItem.documentName == currentDoc) {

                                wordFound = true;
                              //  string output = currentWord + " found " + to_string(docItem.count) + " times";
                              //  successOutput.push_back(output);
                                break;
                            }
                        }
                    }

                    if (!wordFound) {

                        allWordsFound = false;
                        break;
                    }
                }

                if (allWordsFound && !successOutput.empty()) {

                    //cout << "in Document " << currentDoc << ", ";
                    for (unsigned int i = 0; i < successOutput.size(); ++i) {
                      //  cout << successOutput[i];
                        if (i < successOutput.size() - 1) {
                          //  cout << ", ";
                        }
                    }
                    //cout << "." << endl;
                }
            }


    }
    auto HTTime = std::chrono::duration_cast<std::chrono::nanoseconds>
            (std::chrono::high_resolution_clock::now() - start);
    cout << "\nTime: " << HTTime.count() / k << "\n";


    double ratio = BSTTime.count() / (HTTime.count() * 1.0); //converting HTTime to have floating int
    cout << "Speed Up: ";
    printf("%.5f\n", ratio);



    return 0;
}