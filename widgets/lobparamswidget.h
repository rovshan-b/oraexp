#ifndef LOBPARAMSWIDGET_H
#define LOBPARAMSWIDGET_H

#include <QWidget>
#include "beans/lobparams.h"

class StorageParamsWidget;
class IQueryScheduler;
class QLineEdit;
class TablespaceListComboBox;
class QComboBox;
class QCheckBox;

class LobParamsWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LobParamsWidget(IQueryScheduler *queryScheduler, bool isEditMode, QWidget *parent = 0);

    LobParams getLobParams() const;
    void setLobParams(const LobParams &params);

private:
    QLineEdit *segmentEditor;
    TablespaceListComboBox *tablespaceCombo;
    QComboBox *cacheCombo;
    QComboBox *loggingCombo;
    QLineEdit *chunkEditor;
    QLineEdit *pctVersionEditor;
    QLineEdit *freePoolsEditor;
    QCheckBox *storeInRowCheckBox;
    QCheckBox *retentionCheckBox;

    StorageParamsWidget *storageParamsWidget;

    IQueryScheduler *queryScheduler;
    bool isEditMode;

private slots:
    void cacheComboChanged(int index);
};

#endif // LOBPARAMSWIDGET_H
