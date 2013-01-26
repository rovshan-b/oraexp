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
    row->percentCpu=result.colValue("PERCENT_CPU");
    row->bytes=result.colValue("BYTES");
    row->cardinality=result.colValue("CARDINALITY");
    row->otherTag=result.colValue("OTHER_TAG");
    row->other=result.colValue("OTHER");
    row->partitionStart=result.colValue("PARTITION_START");
    row->partitionStop=result.colValue("PARTITION_STOP");
    row->filterPredicates=result.colValue("ACCESS_PREDICATES");
    row->accessPredicates=result.colValue("FILTER_PREDICATES");
    row->time=result.colValue("TIME");

    return row;
}
