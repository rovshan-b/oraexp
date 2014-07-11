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

void CodeFormatterAction::init(CodeFormatterAction::ActionSequence sequence, CodeFormatterAction::ActionType type, const QMultiHash<QString, QString> &attributes)
{
    this->sequence = sequence;
    this->type = type;
    this->attributes = attributes;
}
