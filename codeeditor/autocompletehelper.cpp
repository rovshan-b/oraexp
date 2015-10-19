#include "autocompletehelper.h"
#include <QStringListModel>
#include "codeeditor.h"
#include "util/codeeditorutil.h"
#include "util/queryscheduler.h"
#include "connectivity/dbconnection.h"
#include "connectivity/statement.h"
#include "widgets/multieditorwidget.h"
#include "models/resultsettablemodel.h"

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

void AutocompleteHelper::prepareCompletionList(MultiEditorWidget *multiEditor, bool topLevel)
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
        getChildList(currentObjectNameInfo, topLevel);
    }
}

void AutocompleteHelper::getChildList(const TokenNameInfo &tokenNameInfo, bool topLevel)
{
    if(this->busy){
        return;
    }

    //qDebug() << "get child list for" << tokenNameInfo.toString();
    //QStringListModel *model = new QStringListModel(QStringList() << "item_1" << "item_2" << "item_3", this);

    queryScheduler->enqueueQuery("get_object_list_for_autocomplete", QList<Param*>() <<
                                 new Param(":name_parts", tokenNameInfo.parts) <<
                                 new Param(":name_parts_count", tokenNameInfo.parts.count()) <<
                                 new Param(":default_schema", queryScheduler->getDb()->getUsername()) <<
                                 new Param(":top_level", topLevel) <<
                                 new Param(":rs_out"),
                                 this,
                                 "get_object_list_for_autocomplete",
                                 "childListReady");
}

void AutocompleteHelper::childListReady(const QueryResult &result)
{
    if(result.hasError || result.statement->rsCount() != 1){
        return;
    }

    QHash<QString, QString> iconColumns;
    iconColumns["OBJECT_NAME"] = "ICON_NAME";
    ResultsetTableModel *model = new ResultsetTableModel(this->queryScheduler,
                                                         result.statement->rsAt(0),
                                                         this, QHash<int,StatementDesc*>(), iconColumns);

    connect(model, SIGNAL(firstFetchCompleted()), this, SLOT(firstFetchCompleted()));

    if(model->canFetchMore(QModelIndex())){
        model->fetchMore(QModelIndex());
    }

}

void AutocompleteHelper::firstFetchCompleted()
{
    emit modelReady(static_cast<QAbstractItemModel*>(sender()), 0);
}
