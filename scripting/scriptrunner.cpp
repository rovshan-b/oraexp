#include "scriptrunner.h"
#include <QMessageBox>

ScriptRunner::ScriptRunner()
{
}

bool ScriptRunner::checkSyntax(const QString &program)
{
    QScriptSyntaxCheckResult syntaxCheckResult = engine.checkSyntax(program);

    if(syntaxCheckResult.state() == QScriptSyntaxCheckResult::Valid){
        return true;
    }

    QMessageBox::critical(0, tr("Script error occured"),
                          syntaxCheckResult.errorMessage());

    return false;
}

void ScriptRunner::setProperty(const QString &name, QObject *value)
{
    QScriptValue obj = engine.newQObject(value);
    engine.globalObject().setProperty(name, obj);
}

void ScriptRunner::setVariable(const QString &name, const QScriptValue &value)
{
    engine.globalObject().setProperty(name, value);
}

QScriptValue ScriptRunner::evaluate(const QString &program, const QString &fileName)
{
    QScriptValue result = engine.evaluate(program, fileName);
    checkForError();
    return result;
}

QScriptValue ScriptRunner::callFunction(const QString &functionName, const QScriptValueList &args)
{
    QScriptValue function = engine.globalObject().property(functionName);

    if(function.isError()){
        checkForError();
        return function;
    }

    QScriptValue result = function.call(QScriptValue(), args);
    checkForError();
    return result;
}

bool ScriptRunner::checkForError()
{
    if(engine.hasUncaughtException()){
        QMessageBox::critical(0, tr("Script error occured"),
                              engine.uncaughtException().toString());
        return true;
    }else{
        return false;
    }
}
