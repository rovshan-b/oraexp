#ifndef ENUMS_H
#define ENUMS_H

namespace OraExp
{
    enum ConstraintType {
        PrimaryKey,
        ForeignKey,
        UniqueKey,
        CheckConstraint,
        UnknownConstraintType //if not entered in GUI
    };

    enum ExtentUnit {
        Bytes,
        KB,
        MB,
        GB
    };

    enum IndexType {
        NormalIndex,
        BitmapIndex,
        UniqueIndex
        //FunctionBasedIndex
    };

    enum QueryType {
        QueryTypeSelect,
        QueryTypeUpdate,
        QueryTypeDelete,
        QueryTypeInsert,
        QueryTypeCreate,
        QueryTypeDrop,
        QueryTypeAlter,
        QueryTypeBegin,
        QueryTypeDeclare,
        QueryTypeUnknown,
        QueryTypeExplainPlan,
        QueryTypeMerge
    };

    enum TableType {
        TableTypeHeap,
        TableTypeTemporaryTransaction,
        TableTypeTemporarySession,
        TableTypeIndexOrganized,
        TableTypeExternal,
        TableTypeIndexOrganizedMapping,
        TableTypeIndexOrganizedOverflow
    };

    enum ColumnType {
        ColumnTypeNormal,
        ColumnTypeVirtual
    };

    enum PartitionType {
        PartitionTypeRange,
        PartitionTypeList,
        PartitionTypeHash,
        PartitionTypeReference,
        PartitionTypeInterval,
        PartitionTypeUnknown
    };

    enum PartitionPart {
        PartitionPartPartition,
        PartitionPartSubpartition,
        PartitionPartSubpartitionTemplate
    };

    enum GrantsEditorPerspective
    {
        ObjectGrants,
        UserGrants
    };

    enum ColumnDataType {
        CDTNumeric,
        CDTDateTime,
        CDTText,
        CDTLong,
        CDTCursor,
        CDTLob,
        CDTFile,
        CDTTimestamp,
        CDTInterval,
        CDTRaw,
        CDTObject,
        CDTCollection,
        CDTRef,
        CDTUnknown
    };

    enum ColumnSubType {
        CSTBLong,
        CSTCLong,
        CSTBlob,
        CSTClob,
        CSTNClob,
        CSTBFile,
        CSTCFile,
        CSTTimestamp,
        CSTTimestampTz,
        CSTTimestampLtz,
        CSTIntervalYm,
        CSTIntervalDs,
        CSTUnknown
    };

    enum DmlType{
        DmlTypeSelect,
        DmlTypeInsert,
        DmlTypeUpdate,
        DmlTypeDelete
    };

    enum BindVarStyle{
        BindVarStyleNamed,
        BindVarStylePositioned,
        BindVarStyleQuestionMark
    };
}

#endif // ENUMS_H
