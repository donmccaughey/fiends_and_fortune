#ifndef FIENDS_GENERATE_MAGIC_ITEMS_GENERATE_MAGIC_ITEMS_DIALOG_HPP_INCLUDED
#define FIENDS_GENERATE_MAGIC_ITEMS_GENERATE_MAGIC_ITEMS_DIALOG_HPP_INCLUDED


class TDialog;


struct GenerateMagicItemsDialogData {
    char numberOfItems[3];
};


TDialog *
newGenerateMagicItemsDialog();


#endif
