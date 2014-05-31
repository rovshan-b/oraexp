#ifndef BLOCKDATA_H
#define BLOCKDATA_H

#include <QTextBlockUserData>

class TokenInfo;

class BlockData : public QTextBlockUserData
{
public:
    BlockData();
    virtual ~BlockData();

    void addToken(TokenInfo *tokenInfo);
    void clear();

    //works in block positions (starts from 0 for every block)
    TokenInfo *tokenAtPosition(int position) const;
    TokenInfo *firstTokenFor(int position) const;

    //inline bool isSectionSeparator() const {return this->sectionSeparator;}

    void setCollapsePosition(int endLine, int versionId);
    inline bool isCollapsePosition(int versionId) const {return (this->versionId==versionId && this->collapseEndLine!=-1) || (this->collapsedUntilLineNumber!=-1);}
    inline int getCollapseEnd(int versionId) const {return (this->versionId==versionId && this->collapseEndLine!=-1) ? this->collapseEndLine : (this->collapsedUntilLineNumber!=-1 ? this->collapsedUntilLineNumber : -1);}

    void setCollapsedUntil(int blockNumber);
    inline int getCollapsedUntil() const {return this->collapsedUntilLineNumber;}
    inline int isCollapsedRangeStart() const {return (this->collapsedUntilLineNumber!=-1);}

private:
    QList<TokenInfo*> tokens;

    //bool sectionSeparator;

    int collapseEndLine;
    int versionId;

    int collapsedUntilLineNumber;
};

#endif // BLOCKDATA_H
