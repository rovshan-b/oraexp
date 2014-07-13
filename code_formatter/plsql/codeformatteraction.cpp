#include "codeformatteraction.h"

CodeFormatterAction::CodeFormatterAction(ActionSequence sequence,
                                         ActionType type,
                                         const QMultiHash<QString, QString> &attributes)
{
    init(sequence, type, attributes);
}

CodeFormatterAction::CodeFormatterAction(const QMultiHash<QString, QString> &attributes)
{
    QString seq = attributes.value("sequence");
    QString typ = attributes.value("type");

    ActionSequence sequence;
    ActionType type;

    sequence = (seq=="before" ? Before : After);

    if(typ == "newline"){
        type = Newline;
    }else if(typ == "space"){
        type = Space;
    }else if(typ == "no_space"){
        type = NoSpace;
    }else if(typ == "indent"){
        type = Indent;
    }else if(typ == "unindent"){
        type = Unindent;
    }else{
        Q_ASSERT(false);
    }

    init(sequence, type, attributes);
}

bool CodeFormatterAction::boolValue(const QString &name) const
{
    QString value = attributes.value(name, "false");
    return value == "true" ? true : false;
}

QList<QChar> CodeFormatterAction::charList(const QString &name) const
{
    QList<QChar> result;

    QList<QString> values = attributes.values(name);
    foreach(const QString &val, values){
        Q_ASSERT(val.length() == 1 || val.startsWith('\\'));

        result.append(val.length()==1 ? val[0] : '\n');
    }

    return result;
}

void CodeFormatterAction::init(CodeFormatterAction::ActionSequence sequence, CodeFormatterAction::ActionType type, const QMultiHash<QString, QString> &attributes)
{
    this->sequence = sequence;
    this->type = type;
    this->attributes = attributes;
}
