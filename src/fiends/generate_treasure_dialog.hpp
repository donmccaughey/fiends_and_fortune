#ifndef FIENDS_GENERATE_TREASURE_DIALOG_HPP_INCLUDED
#define FIENDS_GENERATE_TREASURE_DIALOG_HPP_INCLUDED


class TDialog;


struct GenerateTreasureDialogData {
    char letter[2];
};


TDialog *
newGenerateTreasureDialog();


#endif
