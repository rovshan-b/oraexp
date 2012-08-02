#include "syntaxhighligher.h"

SyntaxHighligher::SyntaxHighligher(QTextDocument * parent) :
    QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::blue);
    //keywordFormat.setFontWeight(QFont::Bold);
    keywordFormat.setFontCapitalization(QFont::AllUppercase);

    QStringList keywordPatterns;
    keywordPatterns << "\\bALL\\b" << "\\bALTER\\b" << "\\bAND\\b" << "\\bANY\\b" << "\\bAS\\b" << "\\bASC\\b" << "\\bAT\\b"
                    << "\\bBEGIN\\b" << "\\bBETWEEN\\b" << "\\bBY\\b"
                    << "\\bCASE\\b" << "\\bCHECK\\b" << "\\bCLUSTER\\b" << "\\bCLUSTERS\\b" << "\\bCOLAUTH\\b" << "\\bCOLUMNS\\b" << "\\bCOMPRESS\\b" << "\\bCONNECT\\b" << "\\bCRASH\\b" << "\\bCREATE\\b" << "\\bCURRENT\\b"
                    << "\\bDECLARE\\b" << "\\bDEFAULT\\b" << "\\bDELETE\\b" << "\\bDESC\\b" << "\\bDISTINCT\\b" << "\\bDROP\\b"
                    << "\\bELSE\\b" << "\\bEND\\b" << "\\bEXCEPTION\\b" << "\\bEXCLUSIVE\\b" << "\\bEXISTS\\b"
                    << "\\bFETCH\\b" << "\\bFOR\\b" << "\\bFROM\\b"
                    << "\\bGOTO\\b" << "\\bGRANT\\b" << "\\bGROUP\\b"
                    << "\\bHAVING\\b"
                    << "\\bIDENTIFIED\\b" << "\\bIF\\b" << "\\bIN\\b" << "\\bINDEX\\b" << "\\bINDEXES\\b" << "\\bINSERT\\b" << "\\bINTERSECT\\b" << "\\bINTO\\b" << "\\bIS\\b"
                    << "\\bLIKE\\b" << "\\bLOCK\\b"
                    << "\\bMINUS\\b" << "\\bMODE\\b"
                    << "\\bNOCOMPRESS\\b" << "\\bNOT\\b" << "\\bNOWAIT\\b" << "\\bNULL\\b"
                    << "\\bOF\\b" << "\\bON\\b" << "\\bOPTION\\b" << "\\bOR\\b" << "\\bORDER\\b" << "\\bOVERLAPS\\b"
                    << "\\bPRIOR\\b" << "\\bPROCEDURE\\b" << "\\bPUBLIC\\b"
                    << "\\bRESOURCE\\b" << "\\bREVOKE\\b"
                    << "\\bSELECT\\b" << "\\bSHARE\\b" << "\\bSIZE\\b" << "\\bSQL\\b" << "\\bSTART\\b"
                    << "\\bTABAUTH\\b" << "\\bTABLE\\b" << "\\bTHEN\\b" << "\\bTO\\b"
                    << "\\bUNION\\b" << "\\bUNIQUE\\b" << "\\bUPDATE\\b"
                    << "\\bVALUES\\b" << "\\bVIEW\\b" << "\\bVIEWS\\b"
                    << "\\bWHEN\\b" << "\\bWHERE\\b" << "\\bWITH\\b"
                    << "\\bA\\b" << "\\bADD\\b" << "\\bAGENT\\b" << "\\bAGGREGATE\\b" << "\\bARRAY\\b" << "\\bATTRIBUTE\\b" << "\\bAUTHID\\b" << "\\bAVG\\b"
                    << "\\bBFILE_BASE\\b" << "\\bBINARY\\b" << "\\bBLOB_BASE\\b" << "\\bBLOCK\\b" << "\\bBODY\\b" << "\\bBOTH\\b" << "\\bBOUND\\b" << "\\bBULK\\b" << "\\bBYTE\\b"
                    << "\\bC\\b" << "\\bCALL\\b" << "\\bCALLING\\b" << "\\bCASCADE\\b" << "\\bCHAR\\b" << "\\bCHAR_BASE\\b" << "\\bCHARACTER\\b" << "\\bCHARSETFORM\\b" << "\\bCHARSETID\\b" << "\\bCHARSET\\b" << "\\bCLOB_BASE\\b" << "\\bCLOSE\\b" << "\\bCOLLECT\\b" << "\\bCOMMENT\\b"
                    << "\\bCOMMIT\\b" << "\\bCOMMITTED\\b" << "\\bCOMPILED\\b" << "\\bCONSTANT\\b" << "\\bCONSTRUCTOR\\b" << "\\bCONTEXT\\b" << "\\bCONTINUE\\b" << "\\bCONVERT\\b" << "\\bCOUNT\\b" << "\\bCURSOR\\b" << "\\bCUSTOMDATUM\\b"
                    << "\\bDANGLING\\b" << "\\bDATA\\b" << "\\bDATE\\b" << "\\bDATE_BASE\\b" << "\\bDAY\\b" << "\\bDEFINE\\b" << "\\bDETERMINISTIC\\b" << "\\bDOUBLE\\b" << "\\bDURATION\\b"
                    << "\\bELEMENT\\b" << "\\bELSIF\\b" << "\\bEMPTY\\b" << "\\bESCAPE\\b" << "\\bEXCEPT\\b" << "\\bEXCEPTIONS\\b" << "\\bEXECUTE\\b" << "\\bEXIT\\b" << "\\bEXTERNAL\\b"
                    << "\\bFINAL\\b" << "\\bFIXED\\b" << "\\bFLOAT\\b" << "\\bFORALL\\b" << "\\bFORCE\\b" << "\\bFUNCTION\\b"
                    << "\\bGENERAL\\b"
                    << "\\bHASH\\b" << "\\bHEAP\\b" << "\\bHIDDEN\\b" << "\\bHOUR\\b"
                    << "\\bIMMEDIATE\\b" << "\\bINCLUDING\\b" << "\\bINDICATOR\\b" << "\\bINDICES\\b" << "\\bINFINITE\\b" << "\\bINSTANTIABLE\\b" << "\\bINT\\b" << "\\bINTERFACE\\b" << "\\bINTERVAL\\b" << "\\bINVALIDATE\\b" << "\\bISOLATION\\b"
                    << "\\bJAVA\\b"
                    << "\\bLANGUAGE\\b" << "\\bLARGE\\b" << "\\bLEADING\\b" << "\\bLENGTH\\b" << "\\bLEVEL\\b" << "\\bLIBRARY\\b" << "\\bLIKE2\\b" << "\\bLIKE4\\b" << "\\bLIKEC\\b" << "\\bLIMIT\\b" << "\\bLIMITED\\b" << "\\bLOCAL\\b" << "\\bLONG\\b" << "\\bLOOP\\b"
                    << "\\bMAP\\b" << "\\bMAX\\b" << "\\bMAXLEN\\b" << "\\bMEMBER\\b" << "\\bMERGE\\b" << "\\bMIN\\b" << "\\bMINUTE\\b" << "\\bMOD\\b" << "\\bMODIFY\\b" << "\\bMONTH\\b" << "\\bMULTISET\\b"
                    << "\\bNAME\\b" << "\\bNAN\\b" << "\\bNATIONAL\\b" << "\\bNATIVE\\b" << "\\bNCHAR\\b" << "\\bNEW\\b" << "\\bNOCOPY\\b" << "\\bNUMBER_BASE\\b"
                    << "\\bOBJECT\\b" << "\\bOCICOLL\\b" << "\\bOCIDATETIME\\b" << "\\bOCIDATE\\b" << "\\bOCIDURATION\\b" << "\\bOCIINTERVAL\\b" << "\\bOCILOBLOCATOR\\b" << "\\bOCINUMBER\\b"
                    << "\\bOCIRAW\\b" << "\\bOCIREFCURSOR\\b" << "\\bOCIREF\\b" << "\\bOCIROWID\\b" << "\\bOCISTRING\\b" << "\\bOCITYPE\\b" << "\\bONLY\\b" << "\\bOPAQUE\\b" << "\\bOPEN\\b" << "\\bOPERATOR\\b" << "\\bORACLE\\b" << "\\bORADATA\\b" << "\\bORGANIZATION\\b" << "\\bORLANY\\b" << "\\bORLVARY\\b"
                    << "\\bOTHERS\\b" << "\\bOUT\\b" << "\\bOVERRIDING\\b"
                    << "\\bPACKAGE\\b" << "\\bPARALLEL_ENABLE\\b" << "\\bPARAMETER\\b" << "\\bPARAMETERS\\b" << "\\bPARTITION\\b" << "\\bPASCAL\\b" << "\\bPIPE\\b" << "\\bPIPELINED\\b" << "\\bPRAGMA\\b" << "\\bPRECISION\\b" << "\\bPRIVATE\\b"
                    << "\\bRAISE\\b" << "\\bRANGE\\b" << "\\bRAW\\b" << "\\bREAD\\b" << "\\bRECORD\\b" << "\\bREF\\b" << "\\bREFERENCE\\b" << "\\bRELIES_ON\\b" << "\\bREM\\b" << "\\bREMAINDER\\b" << "\\bRENAME\\b" << "\\bRESULT\\b" << "\\bRESULT_CACHE\\b" << "\\bRETURN\\b" << "\\bRETURNING\\b"
                    << "\\bREVERSE\\b" << "\\bROLLBACK\\b" << "\\bROW\\b"
                    << "\\bSAMPLE\\b" << "\\bSAVE\\b" << "\\bSAVEPOINT\\b" << "\\bSB1\\b" << "\\bSB2\\b" << "\\bSB4\\b" << "\\bSECOND\\b" << "\\bSEGMENT\\b" << "\\bSELF\\b" << "\\bSEPARATE\\b" << "\\bSEQUENCE\\b" << "\\bSERIALIZABLE\\b"
                    << "\\bSET\\b" << "\\bSHORT\\b" << "\\bSIZE_T\\b" << "\\bSOME\\b" << "\\bSPARSE\\b" << "\\bSQLCODE\\b" << "\\bSQLDATA\\b" << "\\bSQLNAME\\b" << "\\bSQLSTATE\\b" << "\\bSTANDARD\\b" << "\\bSTATIC\\b"
                    << "\\bSTDDEV\\b" << "\\bSTORED\\b" << "\\bSTRING\\b" << "\\bSTRUCT\\b" << "\\bSTYLE\\b" << "\\bSUBMULTISET\\b" << "\\bSUBPARTITION\\b" << "\\bSUBSTITUTABLE\\b" << "\\bSUBTYPE\\b" << "\\bSUM\\b" << "\\bSYNONYM\\b"
                    << "\\bTDO\\b" << "\\bTHE\\b" << "\\bTIME\\b" << "\\bTIMESTAMP\\b" << "\\bTIMEZONE_ABBR\\b" << "\\bTIMEZONE_HOUR\\b" << "\\bTIMEZONE_MINUTE\\b" << "\\bTIMEZONE_REGION\\b" << "\\bTRAILING\\b" << "\\bTRANSACTION\\b" << "\\bTRANSACTIONAL\\b" << "\\bTRUSTED\\b" << "\\bTYPE\\b"
                    << "\\bUB1\\b" << "\\bUB2\\b" << "\\bUB4\\b" << "\\bUNDER\\b" << "\\bUNSIGNED\\b" << "\\bUNTRUSTED\\b" << "\\bUSE\\b" << "\\bUSING\\b"
                    << "\\bVALIST\\b" << "\\bVALUE\\b" << "\\bVARIABLE\\b" << "\\bVARIANCE\\b" << "\\bVARRAY\\b" << "\\bVARYING\\b" << "\\bVOID\\b"
                    << "\\bWHILE\\b" << "\\bWORK\\b" << "\\bWRAPPED\\b" << "\\bWRITE\\b"
                    << "\\bYEAR\\b"
                    << "\\bZONE\\b" << "\\bNUMBER\\b" << "\\bVARCHAR\\b" << "\\bVARCHAR2\\b"
                    << "\\bNVARCHAR\\b" << "\\bNVARCHAR2\\b"
                    << "\\bCONSTRAINT\\b" << "\\bPRIMARY\\b" << "\\bKEY\\b"
                    << "\\bFOREIGN\\b" << "\\bTABLESPACE\\b"
                    << "\\bPCTFREE\\b" << "\\bPCTUSED\\b" << "\\bINITRANS\\b" << "\\bMAXTRANS\\b"
                    << "\\bSTORAGE\\b" << "\\bINITIAL\\b" << "\\bNEXT\\b" << "\\bPCTINCREASE\\b"
                    << "\\bMINEXTENTS\\b" << "\\bMAXEXTENTS\\b" << "\\bREPLACE\\b"
                    << "\\bREFERENCES\\b" << "\\bPARALLEL\\b" << "\\bFREELISTS\\b" << "\\bFREELIST\\b"  << "\\bGROUPS\\b"
                    << "\\bBUFFER_POOL\\b" << "\\bKEEP\\b" << "\\bRECYCLE\\b" << "\\bLOGGING\\b" << "\\bNOLOGGING\\b"
                    << "\\bCACHE\\b" << "\\bNOCACHE\\b" << "\\bMONITORING\\b" << "\\bNOMONITORING\\b"
                    << "\\bROWDEPENDENCIES\\b" << "\\bNOROWDEPENDENCIES\\b" << "\\bENABLE\\b" << "\\bDISABLE\\b"
                    << "\\bMOVEMENT\\b" << "\\bJOIN\\b" << "\\bINNER\\b" << "\\bLEFT\\b" << "\\bRIGHT\\b"
                    << "\\bLOB\\b" << "\\bCHUNK\\b" << "\\bFREEPOOLS\\b" << "\\bRETENTION\\b" << "\\bUNLIMITED\\b"
                    << "\\bTRIGGER\\b";

    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegExp(pattern, Qt::CaseInsensitive);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    numberFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("\\b\\d+\\.?\\d*\\b");
    rule.format=numberFormat;
    highlightingRules.append(rule);

    singleQuotationFormat.setForeground(Qt::red);
    rule.pattern = QRegExp("'.*'");
    rule.pattern.setMinimal(true);
    rule.format = singleQuotationFormat;
    highlightingRules.append(rule);

    doubleQuotationFormat.setForeground(Qt::darkRed);
    rule.pattern = QRegExp("\".*\"");
    rule.pattern.setMinimal(true);
    rule.format = doubleQuotationFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::gray);
    rule.pattern = QRegExp("--[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::gray);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

void SyntaxHighligher::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0) {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0) {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                    + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}
