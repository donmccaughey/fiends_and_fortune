#ifndef FNF_TUI_RECT_H_INCLUDED
#define FNF_TUI_RECT_H_INCLUDED


struct rect {
    int x, y, width, height;
};


inline struct rect
rect_make(int x, int y, int width, int height)
{
    return (struct rect){
        .x=x, .y=y, .width=width, .height=height
    };
}


inline struct rect
rect_make_zero(void)
{
    return (struct rect){
        .x=0, .y=0, .width=0, .height=0
    };
}

    
#endif
