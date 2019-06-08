#include "libvim.h"

#include <caml/mlvalues.h>
#include <caml/bigarray.h>
#include <caml/memory.h>
#include <caml/alloc.h>
#include <caml/callback.h>

#define Val_none Val_int(0)

static value
Val_some( value v )
{   
    CAMLparam1( v );
    CAMLlocal1( some );
    some = caml_alloc(1, 0);
    Store_field( some, 0, v );
    CAMLreturn( some );
}
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

CAMLprim value
libvim_vimGetMode(value unit) {
    int mode = vimGetMode();

    printf("RAW MODE: %d\n", mode);
    printf("get_real_state: %d\n", get_real_state());

    int val = 0;

    if ((mode & INSERT) == INSERT) {
        val = 1;
    } else if ((mode & CMDLINE) == CMDLINE) {
        val = 2;
    } else if((mode & REPLACE_FLAG) == REPLACE_FLAG) {
        val = 3;
    } else if((mode & VISUAL) == VISUAL) {
        val = 4;
    } else if((mode & OP_PENDING) == OP_PENDING) {
        val = 5;
    }

    return Val_int(val);
}

CAMLprim value
libvim_vimBufferOpen(value v) {
    CAMLparam1(v);
    char_u *s;
    s = (char_u *)String_val(v);
    buf_T *buf = vimBufferOpen(s, 0, 1);
    value vbuf = (value)buf;
    CAMLreturn(vbuf);
}

CAMLprim value
libvim_vimBufferGetById(value v) {
    CAMLparam1(v);
    buf_T *buf = vimBufferGetById(Int_val(v));

    if (!buf) {
        CAMLreturn(Val_none);
    } else {
        value b = (value)buf;
        CAMLreturn(Val_some(b));
    }
}

CAMLprim value
libvim_vimBufferSetCurrent(value v) {
    CAMLparam1(v);
    buf_T *buf = (buf_T *)v;

    vimBufferSetCurrent(buf);
}
