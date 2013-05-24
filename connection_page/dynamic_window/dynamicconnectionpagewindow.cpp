#include "dynamicconnectionpagewindow.h"
#include "codeeditor/codeeditor.h"
#include "widgets/subtabwidget.h"
#include "util/dialoghelper.h"
#include "util/iconutil.h"
#include "util/filesystemutil.h"
#include <QtGui>

DynamicConnectionPageWindow::DynamicConnectionPageWindow(QWidget *parent) :
    ConnectionPageWindow(parent)
{
}

void DynamicConnectionPageWindow::createUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QLabel *captionLabel = new QLabel(QString("<h3>%1</h3>").arg(windowInfo.caption));
    mainLayout->addWidget(captionLabel);

    QTabWidget *tab = new SubTabWidget();
    tab->setDocumentMode(false);

    QWidget *formWidget = new QWidget();
    QFormLayout *form = new QFormLayout();
    createForm(form);
    formWidget->setLayout(form);

    tab->addTab(formWidget, IconUtil::getIcon("form"), tr("Options"));

    editor = new CodeEditor();
    editor->setWordWrapMode(QTextOption::WordWrap);
    tab->addTab(editor, IconUtil::getIcon("query"), tr("Query"));

    mainLayout->addWidget(tab);


    QDialogButtonBox *buttonBox=DialogHelper::createButtonBox(this);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

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
    QString script = FileSystemUtil::readTextResource(prefix, windowInfo.scriptFileName, "js", db);
    if(scriptRunner.checkSyntax(script)){
        scriptRunner.evaluate(script, windowInfo.scriptFileName);
        registerScriptVariables();
        updateQueryPane();
    }
}

void DynamicConnectionPageWindow::createForm(QFormLayout *form)
{
    const QList< QHash<QString,QString> > &widgetInfos = windowInfo.widgetInfos;

    const QMetaObject &mo = DynamicConnectionPageWindow::staticMetaObject;
    int enumIx = mo.indexOfEnumerator("WidgetType");
    QMetaEnum metaEnum = mo.enumerator(enumIx);

    for(int i=0; i<widgetInfos.size(); ++i){
        QHash<QString,QString> attributes = widgetInfos.at(i);
        int widgetType = metaEnum.keyToValue(attributes["type"].toStdString().c_str());
        Q_ASSERT(widgetType != -1);
        form->addRow(attributes.value("caption"),
                     createDynamicWidget((WidgetType) widgetType, attributes));
    }
}

void DynamicConnectionPageWindow::setWindowInfo(const DynamicWindowInfo &windowInfo)
{
    this->windowInfo = windowInfo;
}

DynamicWindowInfo *DynamicConnectionPageWindow::getWindowInfo()
{
    return &this->windowInfo;
}

void DynamicConnectionPageWindow::setActionProperties(const QHash<QString, QString> &properties)
{
    this->actionProperties = properties;
}

QWidget *DynamicConnectionPageWindow::createDynamicWidget(WidgetType widgetType, const QHash<QString, QString> &attributes) const
{
    QWidget *widget = 0;
    QString value = getValue(attributes.value("value"));

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
    default:
        Q_ASSERT(false);
        return 0;
        break;
    }

    widget->setObjectName(attributes.value("name"));

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
}

void DynamicConnectionPageWindow::updateQueryPane()
{
    editor->setPlainText(scriptRunner.callFunction("getScript").toString());
}
