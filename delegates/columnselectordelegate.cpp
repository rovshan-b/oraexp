#include "columnselectordelegate.h"
#include "widgets/lineeditwithbutton.h"
#include "dialogs/columnselectordialog.h"
#include "interfaces/istringlistretriever.h"
#include "util/widgethelper.h"
#include "util/strutil.h"
#include "widgets/lineeditwithautocompleter.h"
#include <QtGui>

ColumnSelectorDelegate::ColumnSelectorDelegate(IStringListRetriever *columnNameRetriever, const QString &dialogTitle, QObject *parent, bool convertToUpperCase) :
    LineEditWithButtonDelegate(convertToUpperCase, parent),
    columnNameRetriever(columnNameRetriever),
    dialogTitle(dialogTitle)
{
}

QWidget *ColumnSelectorDelegate::createEditor(QWidget *parent,
     const QStyleOptionViewItem &option,
     const QModelIndex &index) const
{
    QWidget *editor=LineEditWithButtonDelegate::createEditor(parent, option, index);

    ((LineEditWithButton*)editor)->lineEdit()->setItemListRetriever(columnNameRetriever);

    return editor;
}

void ColumnSelectorDelegate::buttonClicked(LineEditWithButton *senderWidget)
{
    QString currentText=senderWidget->lineEdit()->text();
    ColumnSelectorDialog dialog(columnNameRetriever->getStringList(),
                                splitTrimmed(currentText),
                                senderWidget->window());
    dialog.setWindowTitle(dialogTitle);

    if(dialog.exec()){
        QString selectedColumnNames=dialog.getSelectedColumnsCommaSeparated();
        senderWidget->lineEdit()->setText(selectedColumnNames);
        emit commitData(senderWidget);
    }
}
