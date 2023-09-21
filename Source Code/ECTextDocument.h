#ifndef ECTEXTDOCUMENT_H
#define ECTEXTDOCUMENT_H

#include "ECTextViewImp.h"
#include "ECCommand.h"

class ECTextDocument;
class ECTextDocumentCtrl;

class ECTextDocumentCtrl {
    public:
        ECTextDocumentCtrl(ECTextDocument &d);
        virtual ~ECTextDocumentCtrl();

        // Get and set methods for the controller
        ECTextViewImp& GetView();
        bool GetCommandMode();
        void SetCommandMode(bool s);

        // Calls for modification on command history
        void Undo();
        void Redo();

        // Calls for modifications on the view
        void HighlightKeywords(int row);
        void UpdateView();
        void MoveCursor(const int &pressedKey);

        // Calls for modifications on the document
        void TypeText(const char &pressedKey);
        void DeleteText();
        void Enter();

    private:
        ECTextDocument& doc;
        ECTextViewImp docView;
        std::vector<std::string> keywords;
        ECCommandHistory histCmds;
        bool isCommandMode;
};

class ECTextDocument {
    public:
        ECTextDocument(std::string name);
        virtual ~ECTextDocument();

        // Get and set methods for the document
        ECTextDocumentCtrl& GetCtrl();
        std::vector<std::string>& GetRows();

        // Text modification methods for the document
        void WriteChar(int row, int column, const char &ch);
        void DeleteChar(int row, int column);
        void Newline(int row, int column);
        void Mergeline(int row);

    private:
        std::string docName;
        ECTextDocumentCtrl docCtrl;
        std::vector<std::string> rows;
};

#endif