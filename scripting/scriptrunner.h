#ifndef SCRIPTRUNNER_H
#define SCRIPTRUNNER_H

#include <QScriptEngine>

class ScriptRunner : public QObject
{
    Q_OBJECT
public:
    ScriptRunner();

    bool checkSyntax(const QString &program);

    void setProperty(const QString &name, QObject *value);

    QScriptValue evaluate(const QString &program, const QString &fileName);
    QScriptValue callFunction(const QString &functioName, const QScriptValueList &args = QScriptValueList());

private:
    QScriptEngine engine;

    bool checkForError();
};

#endif // SCRIPTRUNNER_H
