#include "ECInputObservers.h"
#include "ECTextDocument.h"

// ***** ECTextObserver class definitions ***** //

ECKeyObserver :: ECKeyObserver(ECTextViewImp* s, ECTextDocumentCtrl* c) : subject(s), docCtrl(c) {}

ECKeyObserver :: ~ECKeyObserver() {}

// ***** ECTextObserver class definitions ***** //

ECTextObserver :: ECTextObserver(ECTextViewImp* s, ECTextDocumentCtrl* c) : ECKeyObserver(s, c) {}

ECTextObserver :: ~ECTextObserver() {}

void ECTextObserver :: Update() {
    if (!this->docCtrl->GetCommandMode()) {
        if (subject->GetPressedKey() >= 32 && subject->GetPressedKey() <= 126) {
            this->docCtrl->TypeText(subject->GetPressedKey());
        }
    }

    else {
        if (subject->GetPressedKey() == 105) {
            this->docCtrl->SetCommandMode(false);
        }
    }
}

// ***** ECBackspaceObserver class definitions ***** //

ECBackspaceObserver :: ECBackspaceObserver(ECTextViewImp* s, ECTextDocumentCtrl* c) : ECKeyObserver(s, c) {}

ECBackspaceObserver :: ~ECBackspaceObserver() {}

void ECBackspaceObserver :: Update() {
    if (!this->docCtrl->GetCommandMode()) {
        if (subject->GetPressedKey() == KEY_ACTION :: BACKSPACE) {
            this->docCtrl->DeleteText();
        }
    }
}

// ***** ECEnterObserver class definitions ***** //

ECEnterObserver :: ECEnterObserver(ECTextViewImp* s, ECTextDocumentCtrl* c) : ECKeyObserver(s, c) {}

ECEnterObserver :: ~ECEnterObserver() {}

void ECEnterObserver :: Update() {
    if (!this->docCtrl->GetCommandMode()) {
        if (subject->GetPressedKey() == KEY_ACTION :: ENTER) {
            this->docCtrl->Enter();
        }
    }
}

// ***** ECEscapeObserver class definitions ***** //

ECEscapeObserver :: ECEscapeObserver(ECTextViewImp* s, ECTextDocumentCtrl* c) : ECKeyObserver(s, c) {}

ECEscapeObserver :: ~ECEscapeObserver() {}

void ECEscapeObserver :: Update() {
    if (!this->docCtrl->GetCommandMode()) {
        if (subject->GetPressedKey() == 27 || subject->GetPressedKey() == KEY_ACTION :: CTRL_A) {
            this->docCtrl->SetCommandMode(true);
        }
    }
}

// ***** ECCursorObserver class definitions ***** //

ECCursorObserver :: ECCursorObserver(ECTextViewImp* s, ECTextDocumentCtrl* c) : ECKeyObserver(s, c) {}

ECCursorObserver :: ~ECCursorObserver() {}

void ECCursorObserver :: Update() {
    if (subject->GetPressedKey() == KEY_ACTION :: ARROW_UP ||
        subject->GetPressedKey() == KEY_ACTION :: ARROW_DOWN ||
        subject->GetPressedKey() == KEY_ACTION :: ARROW_LEFT ||
        subject->GetPressedKey() == KEY_ACTION :: ARROW_RIGHT) {
        this->docCtrl->MoveCursor(subject->GetPressedKey());
    }
}

// ***** ECUndoRedoObserver class definitions ***** //

ECUndoRedoObserver :: ECUndoRedoObserver(ECTextViewImp* s, ECTextDocumentCtrl* c) : ECKeyObserver(s, c) {}

ECUndoRedoObserver :: ~ECUndoRedoObserver() {}

void ECUndoRedoObserver :: Update() {
    if (this->docCtrl->GetCommandMode()) {
        if (subject->GetPressedKey() == KEY_ACTION :: CTRL_Z) {
            docCtrl->Undo();
        }

        else if (subject->GetPressedKey() == KEY_ACTION :: CTRL_Y) {
            docCtrl->Redo();
        }
    }
}