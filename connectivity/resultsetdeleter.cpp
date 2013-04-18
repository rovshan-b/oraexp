#include "resultsetdeleter.h"
#include "statement.h"
#include "resultset.h"

ResultsetDeleter::ResultsetDeleter(Resultset *rs) : rs(rs)
{
}

void ResultsetDeleter::run()
{
    Q_ASSERT(rs!=0);
    Statement *parentStmt=rs->getStatement();

    delete rs;
    rs=0;

    if(parentStmt->rsCount()==0){
        delete parentStmt;
    }
}
