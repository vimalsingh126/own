#include "/home/vimalsingh/.local/share/Trash/files/tpm2-tss/include/tss2/tss2_fapi.h"
#include <stdio.h>

int main() {
    TSS2_RC r;
    FAPI_CONTEXT *context;

    // Initialize FAPI context
    r = Fapi_Initialize(&context, NULL);
    if (r != TSS2_RC_SUCCESS) {
        printf("Error: Fapi_Initialize failed with 0x%x\n", r);
        return 1;
    }

    // Create NV index
    r = Fapi_CreateNv(context, "/nv/my_index", NULL, 10,NULL,NULL);
    if (r != TSS2_RC_SUCCESS) {
        printf("Error: Fapi_CreateNv failed with 0x%x\n", r);
        Fapi_Finalize(context);
        return 1;
    }

    // Finalize FAPI context
    Fapi_Finalize(context);
    return 0;
}
