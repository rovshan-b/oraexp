#include <QtCore/QCoreApplication>
#include "../ebnfparser.h"
#include "../dfa.h"
#include "../parsingtablebuilder.h"

int main(int argc, char *argv[])
{
    EBNFParser parser;
    parser.parse();
    if(parser.getHasMissingRules()){
        qDebug("grammar file has missing rule definitions. exiting.");
    }else{
        DFA dfa(parser.getBNFRules(), parser.eofTokenId);
        if(!dfa.hasConflicts){
            ParsingTableBuilder(&dfa,
                                parser.getTargetScannerKeywords(),
                                parser.getTargetScannerTokens(),
                                parser.eofTokenId);
        }
    }
    return 0;

    //QCoreApplication a(argc, argv);
    
    //return a.exec();
}
