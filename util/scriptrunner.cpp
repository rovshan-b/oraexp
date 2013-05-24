#include "scriptrunner.h"
#include <QMessageBox>

ScriptRunner::ScriptRunner() : isNull(true)
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
    Q_ASSERT(!isNull);

    QScriptValue obj = engine.newQObject(value);
    engine.globalObject().setProperty(name, obj);
}

QScriptValue ScriptRunner::evaluate(const QString &program, const QString &fileName)
{
    this->isNull = false;

    QScriptValue result = engine.evaluate(program, fileName);
    if(engine.hasUncaughtException()){
        QMessageBox::critical(0, tr("Script error occured"),
                              engine.uncaughtException().toString());
    }
    return result;
}

QScriptValue ScriptRunner::callFunction(const QString &functionName, const QScriptValueList &args)
{
    Q_ASSERT(!isNull);

    QScriptValue function = engine.globalObject().property(functionName);

    if(function.isError()){
        QMessageBox::critical(0, tr("Script error occured"),
                              engine.uncaughtException().toString());
        return function;
    }

    QScriptValue result = function.call(QScriptValue(), args);
    if(engine.hasUncaughtException()){
        QMessageBox::critical(0, tr("Script error occured"),
                              engine.uncaughtException().toString());
    }
    return result;
}
