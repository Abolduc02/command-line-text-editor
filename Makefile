all:
	c++ ECEditorTest.cpp ECInputObservers.cpp ECTextDocument.cpp ECTextViewImp.cpp ECCommand.cpp -o myeditor -std=c++14
