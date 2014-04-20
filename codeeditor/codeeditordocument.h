#ifndef CODEEDITORDOCUMENT_H
#define CODEEDITORDOCUMENT_H

#include <QTextDocument>

class CodeEditorDocument : public QTextDocument
{
    Q_OBJECT
public:
    explicit CodeEditorDocument(QObject *parent);
    
};

#endif // CODEEDITORDOCUMENT_H
