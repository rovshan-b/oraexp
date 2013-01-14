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
        QueryTypeExplainPlan
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
}

#endif // ENUMS_H
