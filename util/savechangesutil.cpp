#include "savechangesutil.h"
#include "connectionspane.h"
#include "connection_page/connectionpage.h"
#include "dialogs/savechangesdialog.h"

SaveChangesUtil::SaveChangesUtil()
{
}

bool SaveChangesUtil::saveAll(ConnectionsPane *cnPane, bool showDiscardButton, bool promptForAll)
{
    SaveChangesDialog dialog(showDiscardButton, cnPane->window());

    for(int i=0; i<cnPane->count(); ++i){
        SaveChangesUtil::addConnectionPageToSaveDialog(cnPane, i, &dialog, promptForAll);
    }

    return SaveChangesUtil::execSaveDialog(&dialog);
}

bool SaveChangesUtil::saveConnectionPage(ConnectionsPane *cnPane,
                                         int cnPageIx)
{
    SaveChangesDialog dialog(true, cnPane->window());
    SaveChangesUtil::addConnectionPageToSaveDialog(cnPane, cnPageIx, &dialog, true);
    return SaveChangesUtil::execSaveDialog(&dialog);
}

void SaveChangesUtil::addConnectionPageToSaveDialog(ConnectionsPane *cnPane,
                                                    int cnPageIx,
                                                    SaveChangesDialog *dialog,
                                                    bool promptForAll)
{
    ConnectionPage *cnPage = static_cast<ConnectionPage*>(cnPane->widget(cnPageIx));

    for(int k=0; k<cnPage->tabCount(); ++k){
        SaveChangesUtil::addConnectionPageTabToSaveDialog(cnPane, cnPageIx, k, dialog, promptForAll);
    }
}

void SaveChangesUtil::addConnectionPageTabToSaveDialog(ConnectionsPane *cnPane,
                                                       int cnPageIx,
                                                       int cnPageTabIx,
                                                       SaveChangesDialog *dialog,
                                                       bool promptForAll)
{
    ConnectionPage *cnPage = static_cast<ConnectionPage*>(cnPane->widget(cnPageIx));
    ConnectionPageTab *cnPageTab = cnPage->tabAt(cnPageTabIx);

    if(cnPageTab->isModified()){

        if(!promptForAll && cnPageTab->isSaved()){
            cnPageTab->saveAll();
        }else{
            dialog->addConnection(cnPage, cnPane->tabIcon(cnPageIx), cnPane->tabText(cnPageIx));
            dialog->addTab(cnPageTab, cnPage->tabIcon(cnPageTabIx), cnPageTab->getTitle());
        }

    }
}

bool SaveChangesUtil::execSaveDialog(SaveChangesDialog *dialog)
{
    if(dialog->isEmpty()){
        return true; //nothing to save
    }else{
        dialog->setReady();
        if(dialog->exec()){
            return true; //saved
        }else{
            return false; //cancelled
        }
    }
}
