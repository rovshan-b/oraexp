#include "bindparameditorwidget.h"
#include "util/iconutil.h"
#include "util/param.h"
#include "util/widgethelper.h"
#include "connectivity/datetime.h"
#include "defines.h"
#include <QtGui>

BindParamEditorWidget::BindParamEditorWidget(QWidget *parent) :
    QWidget(parent)
{
    QHBoxLayout *mainLayout=new QHBoxLayout();

    //dateValidator = new QRegExpValidator(WidgetHelper::createDateTimeRegExp(), this);

    paramTypeCombo=new QComboBox();
    paramTypeCombo->addItem(tr("String/Number"));
    paramTypeCombo->addItem(tr("Date"));
    paramTypeCombo->addItem(tr("Cursor"));
    mainLayout->addWidget(paramTypeCombo);

    valueEditor=new QComboBox();
    valueEditor->setEditable(true);
    valueEditor->lineEdit()->setPlaceholderText(tr("NULL"));
    valueEditor->setMinimumWidth(200);
    mainLayout->addWidget(valueEditor);

    paramDirectionCombo=new QComboBox();
    paramDirectionCombo->addItem(IconUtil::getIcon("right_arrow"), tr("In"));
    paramDirectionCombo->addItem(IconUtil::getIcon("left_arrow"), tr("Out"));
    paramDirectionCombo->addItem(IconUtil::getIcon("left_right_arrows"), tr("InOut"));
    mainLayout->addWidget(paramDirectionCombo);

    mainLayout->setStretchFactor(valueEditor, 1);
    mainLayout->setContentsMargins(0,0,0,0);
    setLayout(mainLayout);

    connect(paramTypeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(paramTypeChanged(int)));
}

bool BindParamEditorWidget::isEditorEnabled() const
{
    return valueEditor->isEnabled();
}

void BindParamEditorWidget::setFocusToEditor()
{
    valueEditor->setFocus();
}

void BindParamEditorWidget::setBindParamType(BindParamInfo::BindParamType type)
{
    Q_ASSERT(type>=0 && type<paramTypeCombo->count());

    paramTypeCombo->setCurrentIndex((int)type);
}

BindParamInfo::BindParamType BindParamEditorWidget::getBindParamType() const
{
    return (BindParamInfo::BindParamType)paramTypeCombo->currentIndex();
}

QString BindParamEditorWidget::getBindParamValue() const
{
    return valueEditor->currentText();
}

void BindParamEditorWidget::setBindParamInfo(BindParamInfo *paramInfo)
{
    paramTypeCombo->setCurrentIndex((int)paramInfo->paramType);
    paramDirectionCombo->setCurrentIndex((int)paramInfo->paramDirection);

    for(int i=paramInfo->paramValueHistory.size()-1; i>=0; --i){
        valueEditor->addItem(paramInfo->paramValueHistory.at(i));
    }

    if(valueEditor->count()>0){
        valueEditor->setCurrentIndex(0);
    }
}

Param *BindParamEditorWidget::createParam(const QString &paramName)
{
    Param *result;
    BindParamInfo::BindParamType paramType=(BindParamInfo::BindParamType)paramTypeCombo->currentIndex();
    QString value=valueEditor->lineEdit()->text();
    Param::ParamDirection paramDirection = (Param::ParamDirection)paramDirectionCombo->currentIndex();

    qDebug() << value;
    switch(paramType){
    case BindParamInfo::Cursor:
        result = new Param(paramName);
        break;
    case BindParamInfo::StringOrNumber:
        result = new Param(paramName, value, paramDirection);
        break;
    case BindParamInfo::Date:
        result = new Param(paramName, DateTime(value), paramDirection);
        if(value.trimmed().isEmpty()){
            result->setNull();
        }
        break;
    default:
        result = 0;
        Q_ASSERT(false);
        break;
    }

    return result;
}

void BindParamEditorWidget::paramTypeChanged(int newType)
{
    if(newType==BindParamInfo::Cursor){
        valueEditor->setEnabled(false);
        paramDirectionCombo->setEnabled(false);
        paramDirectionCombo->setCurrentIndex(Param::Out);
    }else{
        if(!valueEditor->isEnabled()){
            valueEditor->setEnabled(true);
        }
        if(!paramDirectionCombo->isEnabled()){
            paramDirectionCombo->setEnabled(true);
        }
    }


    if(newType==BindParamInfo::Date){
        valueEditor->lineEdit()->setPlaceholderText(DB_DATE_FORMAT);
        valueEditor->lineEdit()->setInputMask("9999-99-99 00:00:00");
        //valueEditor->setValidator(dateValidator);

    }else{
        valueEditor->lineEdit()->setPlaceholderText("NULL");
        valueEditor->lineEdit()->setInputMask("");
        //valueEditor->setValidator(0);
    }
}
