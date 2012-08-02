#ifndef PROXYSTYLE_H
#define PROXYSTYLE_H

#include <QProxyStyle>
#include <QFormLayout>

class ProxyStyle : public QProxyStyle
{
public:
    int styleHint(StyleHint hint, const QStyleOption *option = 0,
                  const QWidget *widget = 0, QStyleHintReturn *returnData = 0) const
    {
        if (hint == QStyle::SH_FormLayoutFieldGrowthPolicy){
            return QFormLayout::AllNonFixedFieldsGrow;
        }

        return QProxyStyle::styleHint(hint, option, widget, returnData);
    }
};

#endif // PROXYSTYLE_H
