#include "filewriter.h"
#include <QFile>
#include <QTextStream>

QString FileWriter::keywordsCode;
QString FileWriter::ruleNamesCode;
QString FileWriter::actionDesclarationsCode;
QString FileWriter::actionInitializationsCode;
QString FileWriter::statesCode;
QString FileWriter::rulesCode;

FileWriter::FileWriter()
{
}

QString *FileWriter::getBuffer(FileWriter::Destination dest)
{
    QString *buffer;

    switch(dest){
    case Keywords:
        buffer = &FileWriter::keywordsCode;
        break;
    case RuleNames:
        buffer = &FileWriter::ruleNamesCode;
        break;
    case ActionDeclarations:
        buffer = &FileWriter::actionDesclarationsCode;
        break;
    case ActionInitializations:
        buffer = &FileWriter::actionInitializationsCode;
        break;
    case States:
        buffer = &FileWriter::statesCode;
        break;
    case Rules:
        buffer = &FileWriter::rulesCode;
        break;
    default:
        buffer = 0;
        Q_ASSERT(false);
        break;
    }

    return buffer;
}

void FileWriter::write(const QString &str, Destination dest)
{
    QString *buffer = FileWriter::getBuffer(dest);
    (*buffer).append(str);
}

void FileWriter::writeLine(const QString &line, Destination dest)
{
    QString *buffer = FileWriter::getBuffer(dest);
    (*buffer).append(line).append("\n");
}

void FileWriter::flushToFiles()
{
    QFile f1("/home/rovshan/Projects/Qt/OraExp/code_parser/plsql/plsqlparsingtable.cpp.tpl");
    QFile f2("/home/rovshan/Projects/Qt/OraExp/code_parser/plsql/plsqlrules.h.tpl");

    f1.open(QIODevice::ReadOnly);
    f2.open(QIODevice::ReadOnly);

    QTextStream s1(&f1);
    QTextStream s2(&f2);

    QString f1contents = s1.readAll();
    QString f2contents = s2.readAll();

    f1contents.replace("{keyword_initializations}", FileWriter::keywordsCode);
    f1contents.replace("{rule_name_initializations}", FileWriter::ruleNamesCode);
    f1contents.replace("{action_declarations}", FileWriter::actionDesclarationsCode);
    f1contents.replace("{action_initializations}", FileWriter::actionInitializationsCode);
    f1contents.replace("{state_initializations}", FileWriter::statesCode);
    f2contents.replace("{rule_definitions}", FileWriter::rulesCode);

    QFile f11("/home/rovshan/Projects/Qt/OraExp/code_parser/plsql/plsqlparsingtable.cpp");
    QFile f22("/home/rovshan/Projects/Qt/OraExp/code_parser/plsql/plsqlrules.h");

    f11.open(QIODevice::WriteOnly);
    f22.open(QIODevice::WriteOnly);

    QTextStream s11(&f11);
    QTextStream s22(&f22);

    s11 << f1contents;
    s22 << f2contents;
}
