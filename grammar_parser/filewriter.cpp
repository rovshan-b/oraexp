#include "filewriter.h"
#include <QFile>
#include <QTextStream>

QString FileWriter::parsingTableCode;
QString FileWriter::rulesCode;

FileWriter::FileWriter()
{
}

QString *FileWriter::getBuffer(FileWriter::Destination dest)
{
    if(dest == ParsingTable){
        return &FileWriter::parsingTableCode;
    }else{
        return &FileWriter::rulesCode;
    }
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

    f1contents.replace("{populate_table_statements}", FileWriter::parsingTableCode);
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
