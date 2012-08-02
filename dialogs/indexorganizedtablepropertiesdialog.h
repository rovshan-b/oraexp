#ifndef INDEXORGANIZEDTABLEPROPERTIESDIALOG_H
#define INDEXORGANIZEDTABLEPROPERTIESDIALOG_H

#include <QDialog>
#include "beans/indexorganizedtableproperties.h"

class IQueryScheduler;
class IndexOrganizedTablePropertiesWidget;
class IStringListRetriever;

class IndexOrganizedTablePropertiesDialog : public QDialog
{
    Q_OBJECT
public:
    explicit IndexOrganizedTablePropertiesDialog(IQueryScheduler *queryScheduler, IStringListRetriever *columnListRetriever, bool isEditMode, QWidget *parent = 0);

    IndexOrganizedTableProperties getAttributes() const;
    void setAttributes(const IndexOrganizedTableProperties &attributes,
                       const IndexOrganizedTableProperties &originalAttributes) const;

private:
    IndexOrganizedTablePropertiesWidget *propertiesWidget;

};

#endif // INDEXORGANIZEDTABLEPROPERTIESDIALOG_H
