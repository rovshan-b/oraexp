#include "codestructurepane.h"
#include "codestructurepage.h"
#include <QtGui>

CodeStructurePane::CodeStructurePane(QWidget *parent) :
    QWidget(parent),
    isActive(true)
{
    QVBoxLayout *mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(2,0,0,0);
    mainLayout->setSpacing(5);

    mainTab = new QStackedWidget();
    pagesTab = new QStackedWidget();

    mainTab->setContentsMargins(0,0,0,0);
    pagesTab->setContentsMargins(0,0,0,0);

    createLabelPane();

    mainTab->addWidget(pagesTab);

    mainLayout->addWidget(mainTab);

    setLayout(mainLayout);

    showPane(LabelPane);
}

void CodeStructurePane::createLabelPane()
{
    QLabel *label = new QLabel(tr("No active PL/SQL editor"));

    label->setWordWrap(true);
    label->setAlignment(Qt::AlignCenter);

    mainTab->addWidget(label);
}

QSize CodeStructurePane::sizeHint() const
{
    return QSize(200, 200);
}

void CodeStructurePane::registerWidget(MultiEditorWidget *editor)
{
    Q_ASSERT(editors.value(editor) == 0);

    CodeStructurePage *page = new CodeStructurePage(editor);

    editors[editor] = page;
    pagesTab->addWidget(page);

    showPane(PagesPane);
}

void CodeStructurePane::unregisterWidget(MultiEditorWidget *editor)
{
    Q_ASSERT(editors.value(editor) != 0);

    editors[editor]->deleteLater();
    editors.remove(editor);

    showPane(LabelPane);
}

void CodeStructurePane::setCurrentEditor(MultiEditorWidget *editor)
{
    Q_ASSERT(editors.value(editor) != 0);

    pagesTab->setCurrentWidget(editors[editor]);

    showPane(PagesPane);
}

void CodeStructurePane::setActive(bool active)
{
    this->isActive = active;
}

void CodeStructurePane::showPane(CodeStructurePane::Pane pane)
{
    mainTab->setCurrentIndex((int)pane);
}

void CodeStructurePane::setNewModel(MultiEditorWidget *editor, CodeStructureModel *newModel, const QModelIndex &index)
{
    CodeStructurePage *page = editors[editor];
    Q_ASSERT(page);
    page->setNewModel(newModel);
    page->setCursorPosition(index);
}
