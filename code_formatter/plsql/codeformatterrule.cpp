#include "codeformatterrule.h"
#include "codeformatteraction.h"
#include "util/strutil.h"
#include "code_parser/codescanner.h"

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

QList<CodeFormatterAction *> CodeFormatterRule::getActions() const
{
    return this->actions;
}

void CodeFormatterRule::setLexemes(const QStringList &lexemes)
{
    this->lexemes = lexemes;
}

void CodeFormatterRule::addToken(int token)
{
    tokens.append(token);
}

void CodeFormatterRule::ready()
{
    qSort(lexemes.begin(), lexemes.end(), caseInsensitiveLessThan);
    qSort(tokens);
}

bool CodeFormatterRule::containsToken(int token, const QString &lexeme) const
{
    bool found = false;

    if(token > NON_LITERAL_START_IX){
        found = (qBinaryFind(tokens, token) != tokens.end());
    }else{
        found = (qBinaryFind(lexemes.begin(), lexemes.end(), lexeme, caseInsensitiveLessThan) != lexemes.end());
    }

    return found;
}
