#ifndef DIALOG2D_H
#define DIALOG2D_H

class Dialog2D
{
public:
    Dialog2D();
    //-1 - not selected; 0 - ok; 1 - cancel
    int dialogResult;

    virtual void resetDialogState() = 0;
};

#endif //DIALOG2D_H
