#ifndef ONDEMANDINFOVIEWERWIDGET_H
#define ONDEMANDINFOVIEWERWIDGET_H

#include <QWidget>

class OnDemandInfoViewerWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OnDemandInfoViewerWidget(QWidget *parent = 0);

public slots:
    void loadInfo(); //
    bool isInfoLoaded() const;
    void refreshInfo();
    bool isLoading() const;

signals:
    void beforeLoadInfo();
    void afterLoadInfo();

protected:
    //these methods should be implemented by inherited classes
    virtual void doLoadInfo();
    virtual void clearInfo();
    void setLoadingComplete();

private:
    volatile bool infoLoaded;
    volatile bool infoLoading;
};

#endif // ONDEMANDINFOVIEWERWIDGET_H
