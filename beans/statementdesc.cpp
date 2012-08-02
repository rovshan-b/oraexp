#include "statementdesc.h"
#include "util/param.h"
#include "connectivity/statement.h"
#include "connectivity/dbconnection.h"

StatementDesc::StatementDesc() : stmt(0)
{
}

StatementDesc::~StatementDesc()
{
    qDeleteAll(params);
    delete stmt;
}

void StatementDesc::addReplacement(int paramIndex, int parentRowColIndex /*this index starts at 1*/)
{
    Q_ASSERT(paramIndex>=0 && paramIndex<params.size());

    paramReplacements[paramIndex]=parentRowColIndex;
}

Statement *StatementDesc::getStatement(DbConnection *db)
{
    if(stmt==0){
        stmt=db->createStatement();
        stmt->prepare(this->query);
        stmt->bindParams(this->params, false);
    }

    return stmt;
}

void StatementDesc::makeReplacements(const QStringList &parentRow)
{
    Q_ASSERT(stmt);

    QHashIterator<int,int> replacementIterator(this->paramReplacements);
    while(replacementIterator.hasNext()){
        replacementIterator.next();
        int paramIx=replacementIterator.key();
        int parentColIx=replacementIterator.value();

        Q_ASSERT(parentColIx>0 && parentColIx<=parentRow.size());

        Param* param=stmt->param(paramIx);
        param->setStrValue(parentRow.at(parentColIx-1));
        stmt->bindParam(param);
    }
}
