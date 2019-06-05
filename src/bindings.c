#include <caml/mlvalues.h>

#include "libvim.h"

CAMLprim value
libvim_vimInit(value unit) {
    char *args[0];
    vimInit(0, args);
    return Val_unit;
}
