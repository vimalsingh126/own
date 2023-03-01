#include <tss2/tss2_esys.h>
#include <tss2/tss2_esys.h>
#include <tss2/tss2_mu.h>

#include <tss2/tss2_mu.h>
#include <stdio.h>
#include <string.h>

#include <tss2/tss2_sys.h>
#include <tss2/tss2_tcti.h>
#include <tss2/tss2_mu.h>



#define TPM2B_EMPTY_INIT { { sizeof(TPM2B), } , {0, } }

#define NV_INDEX 1

int main(void) {
  ESYS_CONTEXT *ectx;
  TSS2_RC r;
  TPM2B_AUTH auth = {
  .size = sizeof(TPM2B_AUTH),
  .buffer = { 0 },
};
  TPM2B_NV_PUBLIC public_info = {
    .size = sizeof(TPM2B_NV_PUBLIC),
    .nvPublic = {
      .nameAlg = TPM2_ALG_SHA256,
      .attributes = TPMA_NV_PPWRITE | TPMA_NV_AUTHWRITE | TPMA_NV_OWNERWRITE |
                    TPMA_NV_NO_DA,
      .authPolicy = { .size = 0 },
      .dataSize = 32
    }
  };
  TPM2B_MAX_NV_BUFFER data = { .size = 32 };
  char *my_string = "Hello, TPM!";

  r = Esys_Initialize(&ectx, NULL, NULL);
  if (r != TPM2_RC_SUCCESS) {
    printf("Failed to initialize ESAPI context: 0x%x\n", r);
    return 1;
  }

  r = Esys_Startup(ectx, TPM2_SU_CLEAR);
  if (r != TPM2_RC_SUCCESS) {
    printf("Failed to start TPM: 0x%x\n", r);
    return 1;
  }

  r = Esys_NV_DefineSpace(ectx, ESYS_TR_RH_OWNER, ESYS_TR_PASSWORD, ESYS_TR_NONE,
                          ESYS_TR_NONE, &auth, &public_info, NV_INDEX);
  if (r != TPM2_RC_SUCCESS) {
    printf("Failed to define NV space: 0x%x\n", r);
    return 1;
  }

  strncpy((char*)data.buffer, my_string, data.size);

  r = Esys_NV_Write(ectx, ESYS_TR_RH_OWNER, NV_INDEX, ESYS_TR_PASSWORD, ESYS_TR_NONE,
                    ESYS_TR_NONE, &data, 0);
  if (r != TPM2_RC_SUCCESS) {
    printf("Failed to write NV space: 0x%x\n", r);
    return 1;
  }

  printf("Data successfully written to NV space.\n");

  Esys_Finalize(&ectx);
  return 0;
}

