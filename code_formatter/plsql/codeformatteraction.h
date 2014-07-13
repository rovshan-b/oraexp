#ifndef CODEFORMATTERACTION_H
#define CODEFORMATTERACTION_H

#include <QMultiHash>

class CodeFormatterAction
{
public:
    enum ActionSequence
    {
        Before,
        After,
        BeforeAndAfter,
        Any
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

    ActionSequence getSequence() const {return this->sequence;}
    ActionType getType() const {return this->type;}

    bool boolValue(const QString &name) const;
    QList<QChar> charList(const QString &name) const;

private:
    ActionSequence sequence;
    ActionType type;
    QMultiHash<QString,QString> attributes;

    void init(ActionSequence sequence,
              ActionType type,
              const QMultiHash<QString,QString> &attributes);
};

#endif // CODEFORMATTERACTION_H
