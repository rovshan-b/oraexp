#include "explainplanrow.h"

ExplainPlanRow::ExplainPlanRow()
{
}

ExplainPlanRow *ExplainPlanRow::createFromStringList(const QStringList &list)
{
    const int LEVEL_IX=1;
    const int OPERATION_IX=2;
    const int OBJECT_NAME_IX=3;
    const int OPTIMIZER_IX=4;
    const int OPTIONS_IX=5;
    const int COST_IX=6;
    const int BYTES_IX=9;
    const int CARDINALITY_IX=10;
    const int OTHER_IX=11;
    const int PART_ID_IX=12;
    const int FILTER_PRED_IX=13;
    const int ACCESS_PRED_IX=14;

    ExplainPlanRow *row=new ExplainPlanRow();
    bool conversionOk;
    row->level=list.at(LEVEL_IX).toInt(&conversionOk);
    if(!conversionOk){
        row->level=-1;
    }
    row->operation=list.at(OPERATION_IX);
    row->objectName=list.at(OBJECT_NAME_IX);
    row->optimizer=list.at(OPTIMIZER_IX);
    row->options=list.at(OPTIONS_IX);
    row->cost=list.at(COST_IX);
    row->bytes=list.at(BYTES_IX);
    row->cardinality=list.at(CARDINALITY_IX);
    row->other=list.at(OTHER_IX);
    row->partitionId=list.at(PART_ID_IX);
    if(list.size()>=FILTER_PRED_IX+1){
        row->filterPredicates=list.at(FILTER_PRED_IX);
        row->accessPredicates=list.at(ACCESS_PRED_IX);
    }

    return row;
}
