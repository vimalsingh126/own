#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tss2/tss2_sys.h>

#define NV_INDEX 0x1500016 // Replace with your own NV index value
#define AUTH_VALUE "password" // Replace with your own authorization value

int main() {
    TSS2_RC rc;
    TSS2_SYS_CONTEXT *sapi_context;
    TPM2_HANDLE nv_handle;
    UINT16 data_size;
    BYTE *nv_data;
    TPMS_AUTH_COMMAND auth_command = {
        .sessionHandle = TPM2_RS_PW,
        .nonce = {0},
        .hmac = {0},
        .sessionAttributes = 0
    };

    // Initialize the TPM2 context
    rc = Tss2_Sys_Initialize(&sapi_context,sizeof(sapi_context), NULL,NULL);
    if (rc != TPM2_RC_SUCCESS) {
        printf("Error: Failed to initialize SAPI context.\n");
        exit(1);
    }

    // Get the NV index handle
    rc = Tss2_Sys_NV_ReadPublic(sapi_context, NV_INDEX, NULL, &nv_handle, NULL, NULL);
    if (rc != TPM2_RC_SUCCESS) {
        printf("Error: Failed to get NV index handle.\n");
        exit(1);
    }

    // Read the NV index data size
    rc = Tss2_Sys_NV_Read(sapi_context, TPM2_RH_OWNER, nv_handle, &auth_command, 0, sizeof(UINT16), &data_size, NULL);
    if (rc != TPM2_RC_SUCCESS) {
        printf("Error: Failed to read NV index data size.\n");
        exit(1);
    }

    // Read the NV index data
    nv_data = (BYTE*)malloc(data_size);
    if (nv_data == NULL) {
        printf("Error: Failed to allocate memory for NV index data.\n");
        exit(1);
    }
    rc = Tss2_Sys_NV_Read(sapi_context, TPM2_RH_OWNER, nv_handle, &auth_command, 0, data_size, &data_size, nv_data);
    if (rc != TPM2_RC_SUCCESS) {
        printf("Error: Failed to read NV index data.\n");
        free(nv_data);
        exit(1);
    }

    // Print the NV index data
    printf("NV index data: ");
    for (int i = 0; i < data_size; i++) {
        printf("%02x", nv_data[i]);
    }
    printf("\n");

    // Clean up resources
    free(nv_data);
    Tss2_Sys_Finalize(sapi_context);
    return 0;
}
