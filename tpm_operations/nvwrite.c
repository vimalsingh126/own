#include <tss2/tss2_sys.h>
#include <tss2/tss2_tctildr.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST_NV_INDEX 0x1000001
#define MESSAGE   "THIS IS DEMO FOR NV DEFINE READ WRITE"


int main(void) {
  char passwd[] = { "password" };
  UINT32 rc;
  TSS2L_SYS_AUTH_RESPONSE sessionsDataOut;
  TPM2B_MAX_NV_BUFFER nvData;
  TPM2B_AUTH nvAuth;

  size_t ctx_size = Tss2_Sys_GetContextSize(0);
  TSS2_SYS_CONTEXT *ctx = calloc(1, ctx_size);
  TSS2_TCTI_CONTEXT *tcti_ctx = NULL;
  TSS2_ABI_VERSION abiVersion = TSS2_ABI_VERSION_CURRENT;
  TSS2L_SYS_AUTH_COMMAND sessionsData = { .count = 1, .auths = {{
    .sessionHandle = TPM2_RS_PW,
    .sessionAttributes = 0,
    .nonce = { .size = 0 },
    .hmac = { .size = 0 }
  }}};

  nvAuth.size = strlen(passwd);
  memcpy(&nvAuth.buffer[0], &passwd[0], nvAuth.size);

  nvData.size = sizeof(MESSAGE);  // size of data to write
  memcpy(&nvData.buffer[0], MESSAGE, nvData.size);

  if (!ctx) {
    fprintf(stderr, "Sys Context NULL\n");
    exit(1);
  }

  rc = Tss2_TctiLdr_Initialize(NULL, &tcti_ctx);
  if (rc != TPM2_RC_SUCCESS) {
    fprintf(stderr, "Failed to Tss2_TctiLdr_Initialize: 0x%x\n", rc);
    exit(1);
  }

  rc = Tss2_Sys_Initialize(ctx, ctx_size,
      tcti_ctx, &abiVersion);
  if (rc != TPM2_RC_SUCCESS) {
    fprintf(stderr, "Failed to Tss2_Sys_Initialize: rc = 0x%x\n", rc);
    exit(1);
  }

  rc = Tss2_Sys_NV_Write(ctx, TPM2_RH_OWNER, TEST_NV_INDEX,
      &sessionsData, &nvData, 0, &sessionsDataOut);
  if (rc != TPM2_RC_SUCCESS) {
    fprintf(stderr, "Failed to Tss2_Sys_NV_Write: rc = 0x%x\n", rc);
    goto free_ctx;
  }

  printf("SUCCESS\n");

  Tss2_Sys_Finalize(ctx);
  exit(0);

free_ctx:
  Tss2_Sys_Finalize(ctx);
  exit(1);
}
