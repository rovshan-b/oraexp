#include "bnfruleitem.h"

BNFRuleItem::BNFRuleItem() : isTerminal(false), isNegated(false)
{
}

bool BNFRuleItem::isEpsilon() const
{
    return (isTerminal && token.lexeme.isEmpty());
}

bool BNFRuleItem::operator==(const BNFRuleItem &other) const
{
    if(this->isTerminal != other.isTerminal){
        return false;
    }
    if(!this->isTerminal && (this->pointsTo!=other.pointsTo)){
        return false;
    }
    if(this->isTerminal && (this->token.lexeme!=other.token.lexeme ||
                            this->rangeEndToken.lexeme!=other.rangeEndToken.lexeme ||
                            this->token.tokenType!=other.token.tokenType)){
        return false;
    }

    return true;
}
