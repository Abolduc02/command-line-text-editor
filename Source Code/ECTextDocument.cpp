#include <fstream>
#include <algorithm>
#include <iostream>

#include "ECTextDocument.h"
#include "ECInputObservers.h"

// ***** ECTextDocumentCtrl class definitions ***** //

ECTextDocumentCtrl :: ECTextDocumentCtrl(ECTextDocument &d) : doc(d), isCommandMode(true) {
    std::ifstream keywordsFile("keywords.txt");

    if (keywordsFile.is_open()) {
        std::string word;

        while (getline(keywordsFile, word)) {
            this->keywords.push_back(word);
        }

        keywordsFile.close();
    }
}

ECTextDocumentCtrl :: ~ECTextDocumentCtrl() {}

ECTextViewImp& ECTextDocumentCtrl :: GetView() {
    return docView;
}

bool ECTextDocumentCtrl :: GetCommandMode() {
    return isCommandMode;
}

void ECTextDocumentCtrl :: SetCommandMode(bool s) {
    this->isCommandMode = s;
}

void ECTextDocumentCtrl :: Undo() {
    this->histCmds.Undo();
}

void ECTextDocumentCtrl :: Redo() {
    this->histCmds.Redo();
}

void ECTextDocumentCtrl :: HighlightKeywords(int row) {
    // Define delimiters as whitespace characters
    std::string delimiters = " \t\n\r\f\v";

    // Find the position of the first non-delimiter character
    size_t start = doc.GetRows()[row].find_first_not_of(delimiters);

    // Iterate over the string, searching for delimiters and extracting words
    while (start != std::string::npos) {
        // Find the position of the next delimiter
        size_t end = doc.GetRows()[row].find_first_of(delimiters, start);

        // Extract the word
        std::string word = doc.GetRows()[row].substr(start, end - start);

        // Check if the word is a keyword
        if (std::find(keywords.begin(), keywords.end(), word) != keywords.end()) {
            docView.SetColor(row, start, end, TEXT_COLOR :: TEXT_COLOR_BLUE);
        }

        // Find the position of the next non-delimiter character
        start = doc.GetRows()[row].find_first_not_of(delimiters, end);
    }
}

void ECTextDocumentCtrl :: UpdateView() {
    docView.InitRows();
    docView.ClearColor();

    for (int b = 0; b < doc.GetRows().size(); b++) {
        if (doc.GetRows()[b].length() < docView.GetColNumInView() || docView.GetColNumInView() == 0) {
            HighlightKeywords(b);
            docView.AddRow(doc.GetRows()[b]);
        }
        
        else {
            int startPos = 0;
            while (startPos < doc.GetRows()[b].length()) {
                std::string subLine = doc.GetRows()[b].substr(startPos, docView.GetColNumInView() -  1);
                docView.AddRow(subLine);
                startPos += docView.GetColNumInView() - 1;
            }
            
            HighlightKeywords(b);
        }
    }
}

void ECTextDocumentCtrl :: MoveCursor(const int &pressedKey) {
    int cursorX = docView.GetCursorX();
    int cursorY = docView.GetCursorY();

    // Move cursor up, protect bounds, move to EOL if prev line was longer
    if (pressedKey == KEY_ACTION :: ARROW_UP) {
        if (cursorY > 0) {
            docView.SetCursorY(cursorY - 1);

            if (cursorX > doc.GetRows()[cursorY - 1].length()) { // EDIT000
                docView.SetCursorX(doc.GetRows()[cursorY - 1].length()); // EDIT000
            }
        }
    }

    // Move cursor down, protect bounds, move to EOL if prev line was longer
    else if (pressedKey == KEY_ACTION :: ARROW_DOWN) {
        if (cursorY < doc.GetRows().size() - 1) { // EDIT000
            docView.SetCursorY(cursorY + 1);

            if (cursorX > doc.GetRows()[cursorY + 1].length()) { // EDIT000
                docView.SetCursorX(doc.GetRows()[cursorY + 1].length()); // EDIT000
            }
        }
    }

    // Move cursor left, protect bounds
    else if (pressedKey == KEY_ACTION :: ARROW_LEFT) {
        if (cursorX > 0) {
            docView.SetCursorX(cursorX - 1);
        }
    }

    // Move cursor right, protect bounds
    else if (pressedKey == KEY_ACTION :: ARROW_RIGHT) {
        if (cursorX < doc.GetRows()[cursorY].length()) { // EDIT000
            docView.SetCursorX(cursorX + 1);
        }
    }
}

void ECTextDocumentCtrl :: TypeText(const char &pressedKey) {
    ECTypeTextCmd *pCmdIns = new ECTypeTextCmd(this->doc, docView.GetCursorX(), docView.GetCursorY(), pressedKey);
    this->histCmds.ExecuteCmd(pCmdIns);
}

void ECTextDocumentCtrl :: DeleteText() {
    ECDeleteTextCmd *pCmdIns = new ECDeleteTextCmd(this->doc, docView.GetCursorX(), docView.GetCursorY());
    this->histCmds.ExecuteCmd(pCmdIns);
}

void ECTextDocumentCtrl :: Enter() {
    ECEnterCmd *pCmdIns = new ECEnterCmd(this->doc, docView.GetCursorX(), docView.GetCursorY());
    this->histCmds.ExecuteCmd(pCmdIns);
}

// ***** ECTextDocument class definitions ***** //

ECTextDocument :: ECTextDocument(std::string name) : docName(name), docCtrl(*this) {}

ECTextDocument :: ~ECTextDocument() {
    std::ofstream save;
    save.open(this->docName);
        
    for (auto line : this->GetRows()) {
        save << line << std::endl;
    }

    save.close();
}

ECTextDocumentCtrl& ECTextDocument :: GetCtrl() {
    return docCtrl;
}

std::vector<std::string>& ECTextDocument :: GetRows() {
    return rows;
}

void ECTextDocument :: WriteChar(int row, int column, const char &ch) {
    // Add a line when first char is entered into document
    if (rows.size() <= row) {
        rows.push_back(std::string(""));
    }

    // Right char into position where the cursor is
    rows[row].insert(rows[row].begin() + column, ch);
}

void ECTextDocument :: DeleteChar(int row, int column) {
    // If beginning of row, move line contents up to previous row and delete row
    if (column == 0 && row > 0) {
        rows[row - 1] += rows[row];
        rows.erase(rows.begin() + row);

        return;
    }

    // Else erase character
    if (column > 0) {
        rows[row].erase(rows[row].begin() + column - 1);
    }
}

void ECTextDocument :: Newline(int row, int column) {
    // Insert newline
    rows.insert(rows.begin() + row + 1, std::string(""));

    // Move characters after the cursor down to newline
    rows[row + 1] += rows[row].substr(column);
    rows[row].erase(column);
}

void ECTextDocument :: Mergeline(int row) {
    // Merge row with one before it
    rows[row] += rows[row + 1];
    rows.erase(rows.begin() + row + 1);
}