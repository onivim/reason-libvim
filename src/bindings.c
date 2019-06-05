#include <caml/mlvalues.h>

#include "libvim.h"

CAMLprim value
libvim_vimInit(value unit) {
    char *args[0];
    vimInit(0, args);
    return Val_unit;
}

CAMLprim value
libvim_vimInput(value v) {
    char_u *s;
    s = (char_u *)String_val(v);
    vimInput(s);
    return Val_unit;
}

CAMLprim value
libvim_vimCommand(value v) {
    char_u *s;
    s = (char_u *)String_val(v);
    vimExecute(s);
    return Val_unit;
}
