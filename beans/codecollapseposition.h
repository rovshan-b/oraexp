#ifndef CODECOLLAPSEPOSITION_H
#define CODECOLLAPSEPOSITION_H

class CodeCollapsePosition
{
public:
    CodeCollapsePosition();
    CodeCollapsePosition(int startLine, int endLine);

    int startLine;
    int endLine;

    bool isValid() {return startLine!=-1 && endLine!=-1;}
};

#endif // CODECOLLAPSEPOSITION_H
