#include "autocompletehelper.h"
#include <QStringListModel>
#include "codeeditor.h"
#include "util/codeeditorutil.h"
#include "widgets/multieditorwidget.h"

#include <QDebug>

AutocompleteHelper::AutocompleteHelper(QObject *parent) :
    QObject(parent),
    queryScheduler(0),
    busy(false)
{

}

void AutocompleteHelper::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    this->queryScheduler=queryScheduler;
}

void AutocompleteHelper::prepareCompletionList(MultiEditorWidget *multiEditor)
{
    if(!this->queryScheduler || this->busy){
        return;
    }

    /*
        resolve steps (name can consist of several parts)
        check first token for following
          1. is it local variable name
          2. try to find it in all_objects (if it is a snynonym, find out target object)
          3. is it schema name

        if first token is resolved go on to resolve next parts
    */

    CodeEditor *editor = multiEditor->currentTextEditor();

    //for now assume we stopped on dot
    QTextCursor cur = editor->textCursor();
    cur.movePosition(QTextCursor::PreviousCharacter, QTextCursor::MoveAnchor, 2);

    TokenNameInfo currentObjectNameInfo = CodeEditorUtil::getObjectNameInfo(cur);
    if(!currentObjectNameInfo.isEmpty()){
        getChildList(currentObjectNameInfo);
    }
}

void AutocompleteHelper::getChildList(const TokenNameInfo &tokenNameInfo)
{
    if(this->busy){
        return;
    }

    //qDebug() << "get child list for" << tokenNameInfo.toString();
    QStringListModel *model = new QStringListModel(QStringList() << "item_1" << "item_2" << "item_3", this);
    emit modelReady(model, 0);
}
