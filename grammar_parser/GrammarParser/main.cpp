#include <QtCore/QCoreApplication>
#include "../ebnfparser.h"
#include "../dfa.h"

int main(int argc, char *argv[])
{
    EBNFParser parser;
    parser.parse();
    if(parser.getHasMissingRules()){
        qDebug("grammar file has missing rule definitions. exiting.");
    }else{
        DFA(parser.getBNFRules());
    }
    return 0;

    //QCoreApplication a(argc, argv);
    
    //return a.exec();
}
