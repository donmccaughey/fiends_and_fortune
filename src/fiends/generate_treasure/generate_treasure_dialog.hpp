#ifndef FIENDS_GENERATE_TREASURE_GENERATE_TREASURE_DIALOG_HPP_INCLUDED
#define FIENDS_GENERATE_TREASURE_GENERATE_TREASURE_DIALOG_HPP_INCLUDED


class TDialog;


struct GenerateTreasureDialogData {
    char treasureType[2];
};


TDialog *
newGenerateTreasureDialog();


#endif
