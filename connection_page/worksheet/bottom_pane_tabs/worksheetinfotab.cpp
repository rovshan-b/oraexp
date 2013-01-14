#include "worksheetinfotab.h"
#include "util/widgethelper.h"
#include "connectivity/statement.h"
#include <QtGui>

WorksheetInfoTab::WorksheetInfoTab(QWidget *parent) :
    WorksheetBottomPaneTab(parent), blueBrush(Qt::blue), redBrush(Qt::red)
{
    QHBoxLayout *layout=new QHBoxLayout();
    infoBox=new QPlainTextEdit();
    infoBox->setReadOnly(true);
    layout->addWidget(infoBox);

    setLayout(layout);
}

WorksheetResultPane::WorksheetBottomPaneTabType WorksheetInfoTab::getTabType() const
{
    return WorksheetResultPane::InfoTab;
}

void WorksheetInfoTab::showQueryResults(IQueryScheduler*, const QueryResult &result)
{
    //WidgetHelper::appendToPlainTextEdit(infoBox, "\n");

    if(result.hasError){
        WidgetHelper::appendToPlainTextEdit(infoBox, QString("%1Line: %2, Position: %3").
                                            arg(result.exception.getErrorMessage()).
                                            arg(result.exception.getErrorRow()).
                                            arg(result.exception.getErrorPos()),
                                            false, redBrush);
    }else{
        QString textToAppend;
        textToAppend.append(tr("%1 succeeded").arg(result.statement->getStatementVerb()));

        OraExp::QueryType statementType=result.statement->getStatementType();
        int affectedRecordCount=result.statement->getAffectedRecordCount();

        if(statementType==OraExp::QueryTypeInsert ||
                statementType==OraExp::QueryTypeUpdate ||
                statementType==OraExp::QueryTypeDelete){
            if(affectedRecordCount==1){
                textToAppend.append(tr(". 1 row affected"));
            }else{
                textToAppend.append(tr(". %1 rows affected").arg(affectedRecordCount));
            }
        }

        WidgetHelper::appendToPlainTextEdit(infoBox, textToAppend, false, defaultBrush);
    }

    if(!result.dbmsOutput.isEmpty()){
        WidgetHelper::appendToPlainTextEdit(infoBox, QString("\nDBMS_OUTPUT:\n%1").arg(result.dbmsOutput), false, blueBrush);
    }

    if(result.statement!=0){
        printParamValues(result.statement);
    }

    if(result.statement!=0 && result.statement->rsCount()==0){
        delete result.statement;
    }
}

void WorksheetInfoTab::showMessage(const QString &msg)
{
    WidgetHelper::appendToPlainTextEdit(infoBox, msg, false, defaultBrush);
}

void WorksheetInfoTab::printParamValues(Statement *stmt)
{
    QString output;

    for(int i=0; i<stmt->paramCount(); ++i){
        Param *param=stmt->param(i);
        if(param->getParamDirection()==Param::In || param->getParamType()==Param::Stmt){
            continue;
        }
        output.append(QString("%1 : %2\n").arg(param->getParamName(), param->toString()));
    }
    if(!output.isEmpty()){
        output.prepend("Bind parameters:\n");
    }

    if(!output.isEmpty()){
        WidgetHelper::appendToPlainTextEdit(infoBox, output, false, blueBrush);
    }
}
