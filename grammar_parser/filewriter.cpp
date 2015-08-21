#include "filewriter.h"
#include <QFile>
#include <QDir>
#include <QTextStream>

QString FileWriter::keywordsCode;
QString FileWriter::reservedWordsCode;
QString FileWriter::ruleNamesCode;
QString FileWriter::actionDefinitionsCode;
QStringList FileWriter::actionDesclarationsCode;
QStringList FileWriter::actionInitializationsCode;
QStringList FileWriter::statesCode;
QString FileWriter::rulesCode;
QString FileWriter::ruleOptionsCode;

QStringList FileWriter::generatedHeaderFiles;
QStringList FileWriter::generatedSourceFiles;

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
    case ReservedWords:
        buffer = &FileWriter::reservedWordsCode;
        break;
    case RuleNames:
        buffer = &FileWriter::ruleNamesCode;
        break;
    case ActionDefinitions:
        buffer = &FileWriter::actionDefinitionsCode;
        break;
    //case ActionInitializations:
    //    buffer = &FileWriter::actionInitializationsCode;
    //    break;
    //case States:
    //    buffer = &FileWriter::statesCode;
    //    break;
    case Rules:
        buffer = &FileWriter::rulesCode;
        break;
    case RuleOptions:
        buffer = &FileWriter::ruleOptionsCode;
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

void FileWriter::writeActionDeclaration(const QString &line)
{
    FileWriter::actionDesclarationsCode.append(line);
}

void FileWriter::writeActionInitialization(const QString &line)
{
    FileWriter::actionInitializationsCode.append(line);
}

void FileWriter::writeState(const QString &line)
{
    FileWriter::statesCode.append(line);
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
    f1contents.replace("{reserved_word_initializations}", FileWriter::reservedWordsCode);
    f1contents.replace("{rule_name_initializations}", FileWriter::ruleNamesCode);
    f1contents.replace("{rule_option_initializations}", FileWriter::ruleOptionsCode);
    f1contents.replace("{action_definitions}", FileWriter::actionDefinitionsCode);
    //f1contents.replace("{action_initializations}", FileWriter::actionInitializationsCode);
    //f1contents.replace("{state_initializations}", FileWriter::statesCode);
    f2contents.replace("{rule_definitions}", FileWriter::rulesCode);

    QFile f11("/home/rovshan/Projects/Qt/OraExp/code_parser/plsql/plsqlparsingtable.cpp");
    QFile f22("/home/rovshan/Projects/Qt/OraExp/code_parser/plsql/plsqlrules.h");

    f11.open(QIODevice::WriteOnly);
    f22.open(QIODevice::WriteOnly);

    QTextStream s11(&f11);
    QTextStream s22(&f22);

    s11 << f1contents;
    s22 << f2contents;

    QString path = "/home/rovshan/Projects/Qt/OraExp/code_parser/plsql/generated/";
    QDir dir(path);
    dir.setNameFilters(QStringList() << "*.*");
    dir.setFilter(QDir::Files);
    foreach(QString dirFile, dir.entryList())
    {
        dir.remove(dirFile);
    }

    FileWriter::flushActionDeclarations();
    FileWriter::flushActionInitializations();
    FileWriter::flushStateInitializations();
    FileWriter::flushGeneratedFileList();
}


void FileWriter::flushActionDeclarations()
{
    QFile fAll("/home/rovshan/Projects/Qt/OraExp/code_parser/plsql/plsqlallparsingactiondecl.h.tpl");
    QFile fOne("/home/rovshan/Projects/Qt/OraExp/code_parser/plsql/plsqlparsingactiondecl.h.tpl");

    fAll.open(QIODevice::ReadOnly);
    fOne.open(QIODevice::ReadOnly);

    QTextStream sAll(&fAll);
    QTextStream sOne(&fOne);

    QString fAllContents = sAll.readAll();
    QString fOneContents = sOne.readAll();


    static int declCountPerFile = 1000;
    int currentFileId = 0;

    QString includeList;

    QFile fWrite;
    QTextStream sWrite;
    QString declarations;
    for(int i=0; i<FileWriter::actionDesclarationsCode.count(); ++i){
        if(i%declCountPerFile == 0){

            if(fWrite.isOpen()){
                QString contents = QString(fOneContents);
                contents.replace("{id}", QString::number(currentFileId));
                contents.replace("{declarations}", declarations);
                sWrite << contents;
                declarations.clear();
                fWrite.close();

                ++currentFileId;
            }

            QString fileName = QString("/home/rovshan/Projects/Qt/OraExp/code_parser/plsql/generated/plsqlparsingactiondecl_%1.h").arg(QString::number(currentFileId));
            includeList.append(QString("#include \"plsqlparsingactiondecl_%1.h\"\n").arg(QString::number(currentFileId)));
            FileWriter::generatedHeaderFiles.append(QString("code_parser/plsql/generated/plsqlparsingactiondecl_%1.h").arg(QString::number(currentFileId)));
            fWrite.setFileName(fileName);
            fWrite.open(QIODevice::WriteOnly);
            sWrite.setDevice(&fWrite);
        }

        declarations.append(FileWriter::actionDesclarationsCode.at(i)).append("\n");
    }

    if(fWrite.isOpen() && declarations.size()>0){
        QString contents = QString(fOneContents);
        contents.replace("{id}", QString::number(currentFileId));
        contents.replace("{declarations}", declarations);
        sWrite << contents;
        declarations.clear();
    }
    fWrite.close();

    fAllContents.replace("{declarations}", includeList);

    QFile fAllGenerated("/home/rovshan/Projects/Qt/OraExp/code_parser/plsql/generated/plsqlallparsingactiondecl.h");

    fAllGenerated.open(QIODevice::WriteOnly);

    QTextStream sAllGenerated(&fAllGenerated);

    sAllGenerated << fAllContents;
}

void FileWriter::flushActionInitializations()
{
    QFile fOneH("/home/rovshan/Projects/Qt/OraExp/code_parser/plsql/plsqlparsingtableactionsutil.h.tpl");
    QFile fOneC("/home/rovshan/Projects/Qt/OraExp/code_parser/plsql/plsqlparsingtableactionsutil.cpp.tpl");

    fOneH.open(QIODevice::ReadOnly);
    fOneC.open(QIODevice::ReadOnly);

    QTextStream sOneH(&fOneH);
    QTextStream sOneC(&fOneC);

    QString fOneContentsH = sOneH.readAll();
    QString fOneContentsC = sOneC.readAll();


    static int declCountPerFile = 1000;
    int currentFileId = 0;

    QFile fWriteH, fWriteC;
    QTextStream sWriteH, sWriteC;
    QString initializations;

    for(int i=0; i<FileWriter::actionInitializationsCode.count(); ++i){
        if(i%declCountPerFile == 0){

            if(fWriteH.isOpen()){
                QString contentsH = QString(fOneContentsH);
                QString contentsC = QString(fOneContentsC);
                contentsH.replace("{id}", QString::number(currentFileId));
                contentsC.replace("{id}", QString::number(currentFileId));
                contentsC.replace("{action_initializations}", initializations);

                if(i < FileWriter::actionInitializationsCode.count()-1){
                    contentsC.replace("{include_next_file}", QString("#include \"plsqlparsingtableactionsutil_%1.h\"").arg(currentFileId+1));
                    contentsC.replace("{call_next_file}", QString("PlSqlParsingTableActionsUtil%1::initializeActions();").arg(currentFileId+1));
                }else{
                    contentsC.replace("{include_next_file}", "");
                    contentsC.replace("{call_next_file}", "");
                }

                sWriteH << contentsH;
                sWriteC << contentsC;
                initializations.clear();
                fWriteH.close();
                fWriteC.close();

                ++currentFileId;
            }

            QString fileNameH = QString("/home/rovshan/Projects/Qt/OraExp/code_parser/plsql/generated/plsqlparsingtableactionsutil_%1.h").arg(QString::number(currentFileId));
            QString fileNameC = QString("/home/rovshan/Projects/Qt/OraExp/code_parser/plsql/generated/plsqlparsingtableactionsutil_%1.cpp").arg(QString::number(currentFileId));
            fWriteH.setFileName(fileNameH);
            fWriteC.setFileName(fileNameC);
            fWriteH.open(QIODevice::WriteOnly);
            fWriteC.open(QIODevice::WriteOnly);
            sWriteH.setDevice(&fWriteH);
            sWriteC.setDevice(&fWriteC);

            FileWriter::generatedHeaderFiles.append(QString("code_parser/plsql/generated/plsqlparsingtableactionsutil_%1.h").arg(QString::number(currentFileId)));
            FileWriter::generatedSourceFiles.append(QString("code_parser/plsql/generated/plsqlparsingtableactionsutil_%1.cpp").arg(QString::number(currentFileId)));

        }

        initializations.append(FileWriter::actionInitializationsCode.at(i)).append("\n");
    }

    if(fWriteH.isOpen() && initializations.size()>0){
        QString contentsH = QString(fOneContentsH);
        QString contentsC = QString(fOneContentsC);
        contentsH.replace("{id}", QString::number(currentFileId));
        contentsC.replace("{id}", QString::number(currentFileId));
        contentsC.replace("{action_initializations}", initializations);
        contentsC.replace("{include_next_file}", "");
        contentsC.replace("{call_next_file}", "");
        sWriteH << contentsH;
        sWriteC << contentsC;
        initializations.clear();

    }
    fWriteH.close();
    fWriteC.close();

}

void FileWriter::flushStateInitializations()
{
    QFile fOneH("/home/rovshan/Projects/Qt/OraExp/code_parser/plsql/plsqlparsingtablestatesutil.h.tpl");
    QFile fOneC("/home/rovshan/Projects/Qt/OraExp/code_parser/plsql/plsqlparsingtablestatesutil.cpp.tpl");

    fOneH.open(QIODevice::ReadOnly);
    fOneC.open(QIODevice::ReadOnly);

    QTextStream sOneH(&fOneH);
    QTextStream sOneC(&fOneC);

    QString fOneContentsH = sOneH.readAll();
    QString fOneContentsC = sOneC.readAll();


    static int declCountPerFile = 100;
    int currentFileId = 0;

    QFile fWriteH, fWriteC;
    QTextStream sWriteH, sWriteC;
    QString initializations;

    for(int i=0; i<FileWriter::statesCode.count(); ++i){
        if(i%declCountPerFile == 0){

            if(fWriteH.isOpen()){
                QString contentsH = QString(fOneContentsH);
                QString contentsC = QString(fOneContentsC);
                contentsH.replace("{id}", QString::number(currentFileId));
                contentsC.replace("{id}", QString::number(currentFileId));
                contentsC.replace("{state_initializations}", initializations);

                if(i < FileWriter::statesCode.count()-1){
                    contentsC.replace("{include_next_file}", QString("#include \"plsqlparsingtablestatesutil_%1.h\"").arg(currentFileId+1));
                    contentsC.replace("{call_next_file}", QString("PlSqlParsingTableStatesUtil%1::initializeStates(rows);").arg(currentFileId+1));
                }else{
                    contentsC.replace("{include_next_file}", "");
                    contentsC.replace("{call_next_file}", "");
                }

                sWriteH << contentsH;
                sWriteC << contentsC;
                initializations.clear();
                fWriteH.close();
                fWriteC.close();

                ++currentFileId;
            }

            QString fileNameH = QString("/home/rovshan/Projects/Qt/OraExp/code_parser/plsql/generated/plsqlparsingtablestatesutil_%1.h").arg(QString::number(currentFileId));
            QString fileNameC = QString("/home/rovshan/Projects/Qt/OraExp/code_parser/plsql/generated/plsqlparsingtablestatesutil_%1.cpp").arg(QString::number(currentFileId));
            fWriteH.setFileName(fileNameH);
            fWriteC.setFileName(fileNameC);
            fWriteH.open(QIODevice::WriteOnly);
            fWriteC.open(QIODevice::WriteOnly);
            sWriteH.setDevice(&fWriteH);
            sWriteC.setDevice(&fWriteC);

            FileWriter::generatedHeaderFiles.append(QString("code_parser/plsql/generated/plsqlparsingtablestatesutil_%1.h").arg(QString::number(currentFileId)));
            FileWriter::generatedSourceFiles.append(QString("code_parser/plsql/generated/plsqlparsingtablestatesutil_%1.cpp").arg(QString::number(currentFileId)));
        }

        initializations.append(FileWriter::statesCode.at(i)).append("\n");
    }

    if(fWriteH.isOpen() && initializations.size()>0){
        QString contentsH = QString(fOneContentsH);
        QString contentsC = QString(fOneContentsC);
        contentsH.replace("{id}", QString::number(currentFileId));
        contentsC.replace("{id}", QString::number(currentFileId));
        contentsC.replace("{state_initializations}", initializations);
        contentsC.replace("{include_next_file}", "");
        contentsC.replace("{call_next_file}", "");
        sWriteH << contentsH;
        sWriteC << contentsC;
        initializations.clear();

    }
    fWriteH.close();
    fWriteC.close();
}

void FileWriter::flushGeneratedFileList()
{
    QFile fAll("/home/rovshan/Projects/Qt/OraExp/GeneratedFiles.pri.tpl");
    fAll.open(QIODevice::ReadOnly);
    QTextStream sAll(&fAll);
    QString fAllContents = sAll.readAll();
    fAll.close();

    fAllContents.replace("{sources}", FileWriter::generatedSourceFiles.join(" \\ \n"));
    fAllContents.replace("{headers}", FileWriter::generatedHeaderFiles.join(" \\ \n"));

    QFile fAllGenerated("/home/rovshan/Projects/Qt/OraExp/GeneratedFiles.pri");
    QFile fAllGenerated2("/home/rovshan/Projects/Qt/OraExp/code_parser/CodeParser/GeneratedFiles.pri");

    fAllGenerated.open(QIODevice::WriteOnly);
    fAllGenerated2.open(QIODevice::WriteOnly);

    QTextStream sAllGenerated(&fAllGenerated);
    QTextStream sAllGenerated2(&fAllGenerated2);

    sAllGenerated << fAllContents;

    fAllContents.replace("code_parser/", "../");
    sAllGenerated2 << fAllContents;
}

