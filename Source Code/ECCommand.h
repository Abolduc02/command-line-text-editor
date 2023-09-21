#ifndef ECCOMMAND_H
#define ECCOMMAND_H

#include <vector>

class ECTextDocument;
class ECTextDocumentCtrl;

// ******************************************************
// Implement command design pattern

class ECCommand {
    public:
        virtual ~ECCommand() {}
        virtual void Execute() = 0;
        virtual void UnExecute() = 0;
};

class ECTypeTextCmd : public ECCommand {
    public:
        ECTypeTextCmd(ECTextDocument &docIn, int x, int y, char c);
        virtual ~ECTypeTextCmd();

        void Execute();
        void UnExecute();
    
    private:
        ECTextDocument& doc;
        int cursorX;
        int cursorY;
        char text;
};

class ECDeleteTextCmd : public ECCommand {
    public:
        ECDeleteTextCmd(ECTextDocument &docIn, int x, int y);
        virtual ~ECDeleteTextCmd();

        void Execute();
        void UnExecute();

    private:
        ECTextDocument& doc;
        int cursorX;
        int cursorY;
        char text;
};

class ECEnterCmd : public ECCommand {
    public:
        ECEnterCmd(ECTextDocument &docIn, int x, int y);
        virtual ~ECEnterCmd();

        void Execute();
        void UnExecute();

    private:
        ECTextDocument& doc;
        int cursorX;
        int cursorY;
};

// ******************************************************
// Implement command history

class ECCommandHistory {
    public:
        ECCommandHistory();
        virtual ~ECCommandHistory();

        bool Undo();
        bool Redo();
        void ExecuteCmd( ECCommand *pCmd );
        
    private:
        std::vector<ECCommand *> listCommands;
        int posCurrCmd;
};

#endif