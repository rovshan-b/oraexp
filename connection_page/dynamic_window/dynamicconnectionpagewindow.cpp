#include "dynamicconnectionpagewindow.h"
#include "codeeditor/codeeditor.h"
#include "widgets/subtabwidget.h"
#include "widgets/radiobuttongroup.h"
#include "util/dialoghelper.h"
#include "util/iconutil.h"
#include "util/filesystemutil.h"
#include "util/codeeditorutil.h"
#include "connectivity/statement.h"
#include <QtGui>

DynamicConnectionPageWindow::DynamicConnectionPageWindow(DbUiManager *uiManager, QWidget *parent) :
    ConnectionPageWindow(uiManager, parent), windowInfo(0)
{
    connect(&sequentialRunner, SIGNAL(beforeExecute(QString,int,int)), this, SLOT(beforeExecute(QString,int,int)));
    connect(&sequentialRunner, SIGNAL(queryResultAvailable(QueryResult)), this, SLOT(queryResultAvailable(QueryResult)));
    connect(&sequentialRunner, SIGNAL(completed(bool)), this, SLOT(completed(bool)));
}

DynamicConnectionPageWindow::~DynamicConnectionPageWindow()
{
    delete windowInfo;
}

void DynamicConnectionPageWindow::createUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QLabel *captionLabel = new QLabel(QString("<h3>%1</h3>").arg(windowInfo->caption));
    mainLayout->addWidget(captionLabel);

    QTabWidget *tab = new SubTabWidget();
    tab->setDocumentMode(false);

    formWidget = new QWidget();
    QFormLayout *form = new QFormLayout();
    createForm(form);
    formWidget->setLayout(form);

    tab->addTab(formWidget, IconUtil::getIcon("form"), tr("Options"));

    editor = new CodeEditor();
    editor->setWordWrapMode(QTextOption::WordWrap);
    editor->setReadOnly(true);
    tab->addTab(editor, IconUtil::getIcon("query"), tr("Query"));

    mainLayout->addWidget(tab);


    buttonBox=DialogHelper::createButtonBox(this);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    connect(tab, SIGNAL(currentChanged(int)), this, SLOT(tabIndexChanged(int)));

    QSize size = sizeHint();
    if(size.width() < 350){
        size.setWidth(350);
    }
    resize(size);
    DialogHelper::centerWindow(this);
}

void DynamicConnectionPageWindow::setConnection(DbConnection *db)
{
    ConnectionPageWindow::setConnection(db);

    QString prefix = ":/scripts/";
    QString script = FileSystemUtil::readTextResource(prefix, windowInfo->scriptFileName, "js", db);
    if(scriptRunner.checkSyntax(script)){
        scriptRunner.evaluate(script, windowInfo->scriptFileName);
        registerScriptVariables();
        updateQueryPane();
    }

    sequentialRunner.setQueryScheduler(this);

    emitInitCompletedSignal();
}

void DynamicConnectionPageWindow::createForm(QFormLayout *form)
{
    const QList<DynamicWidgetInfo*> &widgetInfos = windowInfo->widgetInfos;

    const QMetaObject &mo = DynamicConnectionPageWindow::staticMetaObject;
    int enumIx = mo.indexOfEnumerator("WidgetType");
    QMetaEnum metaEnum = mo.enumerator(enumIx);

    for(int i=0; i<widgetInfos.size(); ++i){
        DynamicWidgetInfo *widgetInfo = widgetInfos.at(i);
        QHash<QString,QString> attributes = widgetInfo->attributes;
        int widgetType = metaEnum.keyToValue(attributes["type"].toStdString().c_str());
        Q_ASSERT(widgetType != -1);
        QWidget *dynamicWidget = createDynamicWidget((WidgetType) widgetType, widgetInfo);
        QString caption = attributes.value("caption");
        if(caption.isEmpty()){
            form->addRow(dynamicWidget);
            form->setAlignment(dynamicWidget, Qt::AlignCenter);
        }else{
            form->addRow(caption, dynamicWidget);
        }
        scriptRunner.setProperty(dynamicWidget->objectName(), dynamicWidget);
    }
}

void DynamicConnectionPageWindow::setWindowInfo(DynamicWindowInfo *windowInfo)
{
    this->windowInfo = windowInfo;
}

DynamicWindowInfo *DynamicConnectionPageWindow::getWindowInfo()
{
    return this->windowInfo;
}

void DynamicConnectionPageWindow::setResultListener(QObject *resultListener)
{
    this->resultListener = resultListener;
}

void DynamicConnectionPageWindow::setActionProperties(const QHash<QString, QString> &properties)
{
    this->actionProperties = properties;
}

void DynamicConnectionPageWindow::tabIndexChanged(int index)
{
    if(index == 1){ //Query tab
        updateQueryPane();
    }
}

/*
void DynamicConnectionPageWindow::queryCompleted(const QueryResult &result)
{
    delete result.statement;

    setInProgress(false);

    if(result.hasError){
        QMessageBox::critical(this, tr("Error executing query"),
                              result.exception.getErrorMessage());
    }else{
        QDialog::accept();
    }
}*/

void DynamicConnectionPageWindow::setInProgress(bool inProgress)
{
    ConnectionPageWindow::setInProgress(inProgress);

    formWidget->setEnabled(!inProgress);
    buttonBox->setEnabled(false);
}

void DynamicConnectionPageWindow::accept()
{
    setInProgress(true);

    updateQueryPane();

    sequentialRunner.execute(editor->toPlainText(), this->window());

    /*this->enqueueQuery(QString("$%1").arg(editor->toPlainText()),
                       QList<Param*>(),
                       this,
                       "execute_dynamic_query",
                       "queryCompleted");*/
}

void DynamicConnectionPageWindow::beforeExecute(const QString &query, int startPos, int endPos)
{
    Q_UNUSED(query);
    Q_UNUSED(endPos);

    CodeEditorUtil::markPosition(editor, startPos);
}

void DynamicConnectionPageWindow::queryResultAvailable(const QueryResult &result)
{
    delete result.statement;
}

void DynamicConnectionPageWindow::completed(bool success)
{
    setInProgress(false);

    if(success){

        if(resultListener){
            QMetaObject::invokeMethod(resultListener, "actionSucceeded");
        }

        QDialog::accept();
    }
}

QWidget *DynamicConnectionPageWindow::createDynamicWidget(WidgetType widgetType, DynamicWidgetInfo *widgetInfo) const
{
    QWidget *widget = 0;
    QString value = getValue(widgetInfo->attributes.value("value"));

    switch(widgetType){
    case Label:
    {
        QLabel *label = new QLabel(value);
        label->setFrameShape(QFrame::StyledPanel);
        label->setFrameShadow(QFrame::Sunken);
        label->setTextInteractionFlags(Qt::TextSelectableByMouse|Qt::TextSelectableByKeyboard);
        widget = label;
        break;
    }
    case CheckBox:
    {
        QCheckBox *checkBox = new QCheckBox();
        checkBox->setChecked(value == "true");
        widget = checkBox;
        break;
    }
    case ComboBox:
    {
        QComboBox *comboBox = new QComboBox();
        QDomNodeList childNodes = widgetInfo->childNodes;
        for(int i=0; i<childNodes.size(); ++i){
            QDomNode e = childNodes.at(i);
            Q_ASSERT(e.nodeName() == "option");
            comboBox->addItem(e.attributes().namedItem("text").toAttr().value());
        }
        if(!value.isEmpty()){
            comboBox->setCurrentIndex(value.toInt());
        }

        widget = comboBox;
        break;
    }
    case RadioButton:
    {
        RadioButtonGroup *radioButtonGroup = new RadioButtonGroup(widgetInfo->attributes.value("caption").isEmpty() ? Qt::Horizontal : Qt::Vertical);
        QDomNodeList childNodes = widgetInfo->childNodes;
        for(int i=0; i<childNodes.size(); ++i){
            QDomNode e = childNodes.at(i);
            Q_ASSERT(e.nodeName() == "option");
            QString text = e.attributes().namedItem("text").toAttr().value();
            QRadioButton *radio = new QRadioButton(text);
            radioButtonGroup->addRadioButton(radio);
        }

        if(!value.isEmpty()){
            radioButtonGroup->checkRadio(value.toInt());
        }

        widget = radioButtonGroup;
        break;
    }
    default:
        Q_ASSERT(false);
        return 0;
        break;
    }

    QString widgetName = widgetInfo->attributes.value("name");
    widget->setObjectName(widgetName);

    return widget;
}

QString DynamicConnectionPageWindow::getValue(const QString &attrValue) const
{
    if(attrValue.startsWith("prop:")){
        return actionProperties.value(attrValue.mid(5));
    }else{
        return attrValue;
    }
}

void DynamicConnectionPageWindow::registerScriptVariables()
{
    QHashIterator<QString,QString> i(actionProperties);
     while (i.hasNext()) {
         i.next();

         if(i.key().startsWith("var:g_")){
            scriptRunner.setVariable(i.key().mid(4), i.value());
         }
     }
}

void DynamicConnectionPageWindow::updateQueryPane()
{
    QScriptValue val = scriptRunner.callFunction("getScript");

    QString script;

    if(val.isArray()){
        script = val.toVariant().toStringList().join("\n");
    }else{
        script = val.toString();
    }

    editor->setPlainText(script);
}
