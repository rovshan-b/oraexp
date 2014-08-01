#ifndef CODESTRUCTURECOMBOBOX_H
#define CODESTRUCTURECOMBOBOX_H

#include <QComboBox>

class CodeStructureModel;

class CodeStructureComboBox : public QComboBox
{
    Q_OBJECT
public:
    explicit CodeStructureComboBox(QWidget *parent = 0);
       
public slots:
    void codeStructureModelAvailable(CodeStructureModel *model);
    void cursorPositionChanged(const QModelIndex &index);

signals:
    void selectionChanged(int startPos, int endPos);

private slots:
    void indexChanged(int index);

protected:
    //void focusInEvent (QFocusEvent *event);
    //void focusOutEvent (QFocusEvent *event);
    
};

#endif // CODESTRUCTURECOMBOBOX_H
