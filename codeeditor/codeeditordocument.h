#ifndef CODEEDITORDOCUMENT_H
#define CODEEDITORDOCUMENT_H

#include <QTextDocument>

class CodeEditorDocument : public QTextDocument
{
    Q_OBJECT
public:
    explicit CodeEditorDocument(QObject *parent);
       
private slots:
    void handleContentChanged(int position, int charsRemoved, int charsAdded);
    void handleBlockCountChanged(int newBlockCount);
    
};

#endif // CODEEDITORDOCUMENT_H
