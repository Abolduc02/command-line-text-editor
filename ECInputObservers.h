#ifndef ECINPUTOBSERVERS_H
#define ECINPUTOBSERVERS_H

#include "ECObserver.h"
#include "ECTextViewImp.h"

class ECTextViewImp;
class ECTextDocumentCtrl;

class ECKeyObserver : public ECObserver {
    public:
        ECKeyObserver(ECTextViewImp* s, ECTextDocumentCtrl* c);
        virtual ~ECKeyObserver();

        virtual void Update() = 0;

    protected:
        ECTextViewImp* subject;
        ECTextDocumentCtrl* docCtrl;
};

class ECTextObserver : public ECKeyObserver {
    public:
        ECTextObserver(ECTextViewImp* s, ECTextDocumentCtrl* c);
        ~ECTextObserver();

        void Update();
};

class ECBackspaceObserver : public ECKeyObserver {
    public:
        ECBackspaceObserver(ECTextViewImp* s, ECTextDocumentCtrl* c);
        ~ECBackspaceObserver();

        void Update();
};

class ECEnterObserver : public ECKeyObserver {
    public:
        ECEnterObserver(ECTextViewImp* s, ECTextDocumentCtrl* c);
        ~ECEnterObserver();

        void Update();
};

class ECEscapeObserver : public ECKeyObserver {
    public:
        ECEscapeObserver(ECTextViewImp* s, ECTextDocumentCtrl* c);
        ~ECEscapeObserver();

        void Update();
};

class ECCursorObserver : public ECKeyObserver {
    public:
        ECCursorObserver(ECTextViewImp* s, ECTextDocumentCtrl* c);
        ~ECCursorObserver();

        void Update();
};

class ECUndoRedoObserver : public ECKeyObserver {
    public:
        ECUndoRedoObserver(ECTextViewImp* s, ECTextDocumentCtrl* c);
        ~ECUndoRedoObserver();

        void Update();
};

#endif