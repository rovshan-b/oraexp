#include "usercreatorgrants.h"
#include "util/widgethelper.h"
#include "usercreatorgrantssimplelayout.h"
#include "usercreatorgrantsadvancedlayout.h"
#include "widgets/datatableandtoolbarwidget.h"
#include "models/genericeditabletablemodel.h"
#include <QtGui>

UserCreatorGrants::UserCreatorGrants(UserCreatorTabs *userCreator, bool editMode, QWidget *parent) :
    UserCreatorTab(userCreator, editMode, parent),
    simpleModeRadio(0),
    advancedModeRadio(0),
    simpleLayout(0),
    doDync(true)
{
    QVBoxLayout *mainLayout=new QVBoxLayout();

    if(!editMode){
        QHBoxLayout *modeSelectionLayout=new QHBoxLayout();
        simpleModeRadio=new QRadioButton(tr("&Simple mode"));
        simpleModeRadio->setChecked(true);
        modeSelectionLayout->addWidget(simpleModeRadio);
        advancedModeRadio=new QRadioButton(tr("&Advanced mode"));
        modeSelectionLayout->addWidget(advancedModeRadio);

        mainLayout->addLayout(modeSelectionLayout);
        mainLayout->setAlignment(modeSelectionLayout, Qt::AlignTop|Qt::AlignLeft);
    }

    tab = new QStackedWidget();

    if(!editMode){
        createSimpleLayout();
    }
    createAdvancedLayout();

    mainLayout->addWidget(tab);
    setLayout(mainLayout);

    if(!editMode){
        connect(simpleModeRadio, SIGNAL(toggled(bool)), this, SLOT(switchMode(bool)));

        foreach(QCheckBox *chk, simpleLayout->getRoleCheckBoxes()){
            connect(chk, SIGNAL(stateChanged(int)), this, SLOT(syncAdvancedLayout()));
        }

        foreach(QCheckBox *chk, simpleLayout->getPrivCheckBoxes()){
            connect(chk, SIGNAL(stateChanged(int)), this, SLOT(syncAdvancedLayout()));
        }
    }
}

void UserCreatorGrants::setQueryScheduler(IQueryScheduler *queryScheduler)
{
    UserCreatorTab::setQueryScheduler(queryScheduler);

    advancedLayout->setQueryScheduler(queryScheduler);

    if(!isEditMode()){
        syncAdvancedLayout();
    }
}

void UserCreatorGrants::setUserInfo(UserInfo *userInfo)
{
    advancedLayout->setUserInfo(userInfo);
    //syncSimpleLayout();
}

QList<PrivGrantInfo> UserCreatorGrants::getUserRoles() const
{
    return advancedLayout->getUserRoles();
}

QList<PrivGrantInfo> UserCreatorGrants::getUserSysPrivs() const
{
    return advancedLayout->getUserSysPrivs();
}

QList<TablespaceQuotaInfo> UserCreatorGrants::getUserQuotas() const
{
    return advancedLayout->getUserQuotas();
}

QList<ObjectGrantInfo> UserCreatorGrants::getUserObjectPrivs() const
{
    return advancedLayout->getUserObjectPrivs(objectCreator->getUserName());
}

void UserCreatorGrants::removeIncorrectRows()
{
    advancedLayout->removeIncorrectRows();
}

void UserCreatorGrants::switchMode(bool simpleMode)
{
    tab->setCurrentIndex(simpleMode ? 0 : 1);
    if(simpleMode){
        syncSimpleLayout();
    }else{
        syncAdvancedLayout();
    }
}

void UserCreatorGrants::createSimpleLayout()
{
    simpleLayout = new UserCreatorGrantsSimpleLayout();
    connect(simpleLayout, SIGNAL(beforeToggleAll()), this, SLOT(disableSync()));
    connect(simpleLayout, SIGNAL(afterToggleAll()), this, SLOT(enableSync()));
    tab->addWidget(simpleLayout);
}

void UserCreatorGrants::createAdvancedLayout()
{
    advancedLayout = new UserCreatorGrantsAdvancedLayout(isEditMode());
    connect(advancedLayout, SIGNAL(ddlChanged()), this, SIGNAL(ddlChanged()));
    tab->addWidget(advancedLayout);
}

void UserCreatorGrants::syncSimpleLayout()
{
    syncCheckBoxes(simpleLayout->getRoleCheckBoxes(), advancedLayout->getRolesTable());
    syncCheckBoxes(simpleLayout->getPrivCheckBoxes(), advancedLayout->getSysPrivsTable());
}

void UserCreatorGrants::syncAdvancedLayout()
{
    if(!doDync){
        return;
    }

    syncTable(simpleLayout->getRoleCheckBoxes(), advancedLayout->getRolesTable());
    syncTable(simpleLayout->getPrivCheckBoxes(), advancedLayout->getSysPrivsTable());
}

void UserCreatorGrants::disableSync()
{
    doDync=false;
}

void UserCreatorGrants::enableSync()
{
    doDync=true;
    syncAdvancedLayout();
}

void UserCreatorGrants::syncTable(QList<QCheckBox *> checkBoxes, DataTableAndToolBarWidget *table)
{
    GenericEditableTableModel *model=static_cast<GenericEditableTableModel*>(table->table()->model());
    model->removeIncorrectRows();

    bool removed=false;

    foreach(const QCheckBox *chk, checkBoxes){
        QModelIndexList foundItems=model->match(model->index(0,0), Qt::DisplayRole, chk->text(), 1, Qt::MatchFixedString);
        Q_ASSERT(foundItems.size()<=1);
        if(chk->isChecked()){
            if(foundItems.size()==0){
                model->insertRow(model->rowCount());
                model->setData(model->index(model->rowCount()-1, 0), chk->text());
            }
        }else{
            if(foundItems.size()==1){
                WidgetHelper::deleteTableRow(table->table(), foundItems.at(0).row(), false, false);
                removed=true;
            }
        }
    }

    if(model->rowCount()==0){
        model->insertRow(0);
    }

    if(removed){
        emit ddlChanged();
    }
}

void UserCreatorGrants::syncCheckBoxes(QList<QCheckBox *> checkBoxes, DataTableAndToolBarWidget *table)
{
    GenericEditableTableModel *model=static_cast<GenericEditableTableModel*>(table->table()->model());
    foreach(QCheckBox *chk, checkBoxes){
        QModelIndexList foundItems=model->match(model->index(0,0), Qt::DisplayRole, chk->text(), 1, Qt::MatchFixedString);
        Q_ASSERT(foundItems.size()<=1);
        chk->setChecked(foundItems.size()>0);
    }
}
