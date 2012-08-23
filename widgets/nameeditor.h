#ifndef NAMEEDITOR_H
#define NAMEEDITOR_H

#include <QLineEdit>

class DbConnection;

class NameEditor : public QLineEdit
{
    Q_OBJECT
public:
    explicit NameEditor(QWidget *parent = 0);

signals:

public slots:

};

#endif // NAMEEDITOR_H
