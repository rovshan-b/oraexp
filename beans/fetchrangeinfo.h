#ifndef FETCHRANGEINFO_H
#define FETCHRANGEINFO_H

//used by ScrollableResultsetTableModel
class FetchRangeInfo
{
public:
    enum FetchMode
    {
        AppendToCurrentData,
        PrependToCurrentData,
        ReplaceCurrentData
    };

    FetchRangeInfo();

    int rangeStart;
    int rangeEnd;
    FetchMode fetchMode;
};

#endif // FETCHRANGEINFO_H
