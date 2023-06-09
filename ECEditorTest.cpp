#include <iostream>
#include <fstream>

#include "ECTextViewImp.h"
#include "ECTextDocument.h"
#include "ECInputObservers.h"

using namespace std;

int main(int argc, char *argv[]) {

    // If file name not provided, exit program
    if (argc != 2) {
        cout << "Usage: ./editor <filename>" << endl;
        exit(-1);
    }

    // Create document
    ECTextDocument doc(argv[1]);
    ECTextDocumentCtrl &docCtrl = doc.GetCtrl();
    ECTextViewImp &docView = docCtrl.GetView();

    // load doc if it exists in directory, if not create new doc
    ifstream textfile(argv[1]);

    if (textfile.is_open()) {
        string line;

        while (getline(textfile, line)) {
            doc.GetRows().push_back(line);
        }

        textfile.close();

        // initialize view with loaded textfile
        docCtrl.UpdateView();
    }
    
    // create observers (I wanted to produce and save these in my controller but I kept getting bus errors)
    ECTextObserver text(&docView, &docCtrl);
    docView.Attach(&text);

    ECBackspaceObserver backspace(&docView, &docCtrl);
    docView.Attach(&backspace);

    ECEnterObserver enter(&docView, &docCtrl);
    docView.Attach(&enter);

    ECCursorObserver cursor(&docView, &docCtrl);
    docView.Attach(&cursor);

    ECEscapeObserver escape(&docView, &docCtrl);
    docView.Attach(&escape);

    ECUndoRedoObserver undoredo(&docView, &docCtrl);
    docView.Attach(&undoredo);

    // Show the window
    docView.AddStatusRow("Editor", "For demo only", true);
    docView.Show();
    
    return 0;
}
