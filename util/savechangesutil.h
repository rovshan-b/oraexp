#ifndef SAVEFILESUTIL_H
#define SAVEFILESUTIL_H

class ConnectionsPane;
class ConnectionPage;
class ConnectionPageTab;
class SaveChangesDialog;

class SaveChangesUtil
{
public:
    static bool saveAll(ConnectionsPane *cnPane, bool showDiscardButton, bool promptForAll);
    static bool saveConnectionPage(ConnectionsPane *cnPane, int cnPageIx);

private:
    SaveChangesUtil();

    static void addConnectionPageToSaveDialog(ConnectionsPane *cnPane, int cnPageIx, SaveChangesDialog *dialog, bool promptForAll);
    static void addConnectionPageTabToSaveDialog(ConnectionsPane *cnPane, int cnPageIx, int cnPageTabIx, SaveChangesDialog *dialog, bool promptForAll);
    static bool execSaveDialog(SaveChangesDialog *dialog);
};

#endif // SAVEFILESUTIL_H
