#ifndef PROGRAMUNITEDITOR_H
#define PROGRAMUNITEDITOR_H

#include "../connectionpagetab.h"

class ProgramCodeEditor;

class ProgramUnitEditor : public ConnectionPageTab
{
    Q_OBJECT
public:
    explicit ProgramUnitEditor(DbUiManager *uiManager, QWidget *parent = 0);

    virtual void setConnection(DbConnection *db);

    virtual bool disableWhileConnecting(){return false;}

private:
    ProgramCodeEditor *editor;

};

#endif // PROGRAMUNITEDITOR_H
