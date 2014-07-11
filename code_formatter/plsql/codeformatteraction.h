#ifndef CODEFORMATTERACTION_H
#define CODEFORMATTERACTION_H

#include <QMultiHash>

class CodeFormatterAction
{
public:
    enum ActionSequence
    {
        Before,
        After
    };

    enum ActionType
    {
        Newline,
        Space,
        NoSpace,
        Indent,
        Unindent
    };

    CodeFormatterAction(ActionSequence sequence,
                        ActionType type,
                        const QMultiHash<QString,QString> &attributes);

    CodeFormatterAction(const QMultiHash<QString,QString> &attributes);

private:
    ActionSequence sequence;
    ActionType type;
    QMultiHash<QString,QString> attributes;

    void init(ActionSequence sequence,
              ActionType type,
              const QMultiHash<QString,QString> &attributes);
};

#endif // CODEFORMATTERACTION_H
