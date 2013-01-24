#include "explainplanrow.h"

ExplainPlanRow::ExplainPlanRow()
{
}

ExplainPlanRow *ExplainPlanRow::createFromFetchResult(const FetchResult &result)
{
    ExplainPlanRow *row=new ExplainPlanRow();
    bool conversionOk;
    row->level=result.colValue("LEVEL").toInt(&conversionOk);
    if(!conversionOk){
        row->level=-1;
    }
    row->operation=result.colValue("OPERATION");
    row->objectName=result.colValue("OBJECT_NAME");
    row->optimizer=result.colValue("OPTIMIZER");
    row->options=result.colValue("OPTIONS");
    row->cost=result.colValue("COST");
    row->bytes=result.colValue("BYTES");
    row->cardinality=result.colValue("CARDINALITY");
    row->other=result.colValue("OTHER");
    row->partitionId=result.colValue("PARTITION_ID");
    row->filterPredicates=result.colValue("ACCESS_PREDICATES");
    row->accessPredicates=result.colValue("FILTER_PREDICATES");

    return row;
}
