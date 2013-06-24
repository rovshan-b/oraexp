#ifndef SAVEPREVIEWWIDGET_H
#define SAVEPREVIEWWIDGET_H

#include <QWidget>

class QTextDocument;
class SubTabWidget;

class SavePreviewWidget : public QWidget
{
    Q_OBJECT
public:
    explicit SavePreviewWidget(QWidget *parent = 0);

    void setPreviewDocuments(QList<QTextDocument*> documents);
    void clearPreviewDocuments();
    
private:
    SubTabWidget *tab;
    QWidget *noPreviewWidget;

    void ensureTabCount(int count);
    void showNoPreviewMessage(bool show);
    
};

#endif // SAVEPREVIEWWIDGET_H
