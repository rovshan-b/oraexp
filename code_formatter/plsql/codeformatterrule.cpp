#include "codeformatterrule.h"
#include "codeformatteraction.h"

CodeFormatterRule::CodeFormatterRule()
{
}

CodeFormatterRule::~CodeFormatterRule()
{
    qDeleteAll(actions);
}

void CodeFormatterRule::addAction(CodeFormatterAction *action)
{
    actions.append(action);
}
