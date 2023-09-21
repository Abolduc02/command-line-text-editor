#include "ECCommand.h"
#include "ECTextDocument.h"

// ***** ECCommandHistory class definitions ***** //

ECCommandHistory :: ECCommandHistory() : posCurrCmd(-1) {}

ECCommandHistory :: ~ECCommandHistory() {
    for (unsigned int i = 0; i < listCommands.size(); ++i) {
        delete listCommands[i];
    }

    listCommands.clear();
    posCurrCmd = -1;
}

bool ECCommandHistory :: Undo() {
    if (posCurrCmd < 0) {
        return false;
    }

    listCommands[posCurrCmd]->UnExecute();
    --posCurrCmd;

    return true;
}

bool ECCommandHistory :: Redo() {
    if (posCurrCmd >= (int)listCommands.size() - 1) {
        return false;
    }

    ++posCurrCmd;
    listCommands[posCurrCmd]->Execute();

    return true;
}

void ECCommandHistory :: ExecuteCmd(ECCommand *pCmd) {
    pCmd->Execute();

    // clear up all cmds from current pos to end
    if (posCurrCmd >= -1) {
        int szList = listCommands.size();

        for (unsigned int i = posCurrCmd + 1; i < szList; ++i) {
            delete listCommands.back();
            listCommands.pop_back();
        }
    }
    
    listCommands.push_back(pCmd);
    ++posCurrCmd;
}

// ***** ECTypeTextCmd class definitions ***** //

ECTypeTextCmd :: ECTypeTextCmd(ECTextDocument &docIn, int x, int y, char c) : doc(docIn), cursorX(x), cursorY(y), text(c) {}

ECTypeTextCmd :: ~ECTypeTextCmd() {}

void ECTypeTextCmd :: Execute() {
    // Update the document
    this->doc.WriteChar(this->cursorY, this->cursorX, this->text);

    // Update the cursor
    this->doc.GetCtrl().GetView().SetCursorX(this->cursorX + 1);

    // Update the view
    this->doc.GetCtrl().UpdateView();
}

void ECTypeTextCmd :: UnExecute() {
    int currRowLength = doc.GetRows()[this->cursorY].length(); // EDIT000

    // Update the document
    this->doc.DeleteChar(this->cursorY, this->cursorX + 1);

    // Update the view
    doc.GetCtrl().UpdateView();

    // Update the cursor
    if (cursorX == 0 && cursorY > 0) {
        doc.GetCtrl().GetView().SetCursorX(doc.GetRows()[this->cursorY - 1].length() - currRowLength); // EDIT000
        doc.GetCtrl().GetView().SetCursorY(cursorY - 1);
    }
    
    else if (cursorX > 0) {
        doc.GetCtrl().GetView().SetCursorX(cursorX);
    }
}

// ***** ECDeleteTextCmd class definitions ***** //

ECDeleteTextCmd :: ECDeleteTextCmd(ECTextDocument &docIn, int x, int y) : doc(docIn), cursorX(x), cursorY(y) {}

ECDeleteTextCmd :: ~ECDeleteTextCmd() {}

void ECDeleteTextCmd :: Execute() {
    int currRowLength = doc.GetRows()[this->cursorY].length(); // EDIT000

    // Save char to be restored if undo
    this->text = doc.GetRows()[cursorY][cursorX];

    // Update document
    doc.DeleteChar(this->cursorY, this->cursorX);

    // Update view
    doc.GetCtrl().UpdateView();

    // Move cursor one to the left if char removed, move to prev line if line deleted, dont move if at beginning of doc
    if (cursorX == 0 && cursorY > 0) {
        doc.GetCtrl().GetView().SetCursorX(doc.GetRows()[this->cursorY - 1].length() - currRowLength); // EDIT000
        doc.GetCtrl().GetView().SetCursorY(cursorY - 1);
    }
    
    else if (cursorX > 0) {
        doc.GetCtrl().GetView().SetCursorX(cursorX - 1);
    }
}

void ECDeleteTextCmd :: UnExecute() {
    // Update the document
    this->doc.WriteChar(this->cursorY, this->cursorX, this->text);

    // Update the cursor
    this->doc.GetCtrl().GetView().SetCursorX(this->cursorX + 1);

    // Update the view
    this->doc.GetCtrl().UpdateView();
}

// ***** ECEnterCmd class definitions ***** //

ECEnterCmd :: ECEnterCmd(ECTextDocument &docIn, int x, int y) : doc(docIn), cursorX(x), cursorY(y) {}

ECEnterCmd :: ~ECEnterCmd() {}

void ECEnterCmd :: Execute() {
    // Update document
    doc.Newline(this->cursorY, this->cursorX);

    // Update view
    doc.GetCtrl().UpdateView();

    // Move cursor to beginning of new row
    doc.GetCtrl().GetView().SetCursorY(cursorY + 1);
    doc.GetCtrl().GetView().SetCursorX(0);
}

void ECEnterCmd :: UnExecute() {
    int currRowLength = doc.GetRows()[this->cursorY].length(); // EDIT000

    // Update the document
    doc.Mergeline(cursorY);

    // Update the view
    doc.GetCtrl().UpdateView();

    // Move cursor up one row
    doc.GetCtrl().GetView().SetCursorY(cursorY);
    doc.GetCtrl().GetView().SetCursorX(cursorX);
}