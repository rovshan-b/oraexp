#include "dynamicconnectionpagewindow.h"
#include "util/dialoghelper.h"
#include <QtGui>

DynamicConnectionPageWindow::DynamicConnectionPageWindow(QWidget *parent) :
    ConnectionPageWindow(parent)
{
}

void DynamicConnectionPageWindow::createUi()
{
    QVBoxLayout *mainLayout = new QVBoxLayout();

    QFormLayout *form = new QFormLayout();
    createForm(form);
    mainLayout->addLayout(form);
    //mainLayout->setAlignment(form, Qt::AlignHCenter);

    QDialogButtonBox *buttonBox=DialogHelper::createButtonBox(this);
    mainLayout->addWidget(buttonBox);

    setLayout(mainLayout);

    resize(sizeHint());
    DialogHelper::centerWindow(this);
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
    default:
        Q_ASSERT(false);
        break;
    }

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
