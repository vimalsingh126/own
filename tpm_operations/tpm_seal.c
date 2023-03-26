Trusted Platform Module (TPM) is a hardware-based security module that provides a secure storage location for sensitive data. TPM is designed to protect against attacks that attempt to read or modify data stored in memory or on disk, by encrypting the data using a key stored within the TPM.

To hide data in TPM, you can use the TPM's sealing mechanism. Sealing data in TPM means that it can only be decrypted and accessed by the same TPM that sealed the data. This provides a secure way to store data, as even if the storage device or hard disk is compromised, the data remains protected as it can only be decrypted by the original TPM.

The following steps can be used to hide data in TPM:

Create a new TPM object: Use the TPM_Create function to create a new object in the TPM. This object can be a key or a piece of data that you want to seal.

Seal the data: Use the TPM_Seal function to seal the data. This function encrypts the data using a key stored within the TPM and returns a sealed blob that can only be decrypted by the same TPM.

Store the sealed blob: Store the sealed blob in a secure location, such as a file or database.

Retrieve the sealed blob: To retrieve the sealed blob, read it from the secure location where it was stored.

Unseal the data: Use the TPM_Unseal function to decrypt the sealed blob and retrieve the original data. This function can only be called by the same TPM that sealed the data, ensuring that the data remains secure.

It is important to note that TPM is a hardware-based solution, and therefore requires a TPM chip to be present on the system. Additionally, TPM should not be used as the only security mechanism for protecting sensitive data. It should be used in conjunction with other security measures, such as strong authentication, encryption, and access controls.
  
  
  
  #include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <tss2/tss2_sys.h>
#include <tss2/tss2_tctildr.h>

#define PCR_SELECTION_SIZE sizeof(TPMS_PCR_SELECTION)
#define PCR_BANKS_SIZE (PCR_SELECTION_SIZE + 2)

#define MESSAGE "THIS IS A SECRET MESSAGE"
#define PASSWORD "password"

#define NV_INDEX 0x1500000
#define PCR_INDEX 16

static TPM2B_DIGEST getPcrValue(TSS2_SYS_CONTEXT *ctx) {
  TPM2B_DIGEST pcrValue = TPM2B_TYPE_INIT(TPM2B_DIGEST, buffer);
  UINT32 rc;
  TPML_DIGEST_VALUES digests = { 0 };
  TPML_PCR_SELECTION pcrSelectionIn = { 0 };
  TPML_PCR_SELECTION pcrSelectionOut = { 0 };

  pcrSelectionIn.count = 1;
  pcrSelectionIn.pcrSelections[0].hash = TPM2_ALG_SHA256;
  pcrSelectionIn.pcrSelections[0].sizeofSelect = PCR_SELECTION_SIZE;
  pcrSelectionIn.pcrSelections[0].pcrSelect[PCR_INDEX / 8] = 1 << (PCR_INDEX % 8);

  rc = Tss2_Sys_PCR_Read(ctx, 0, &pcrSelectionIn, &digests, &pcrSelectionOut);
  if (rc != TPM2_RC_SUCCESS) {
    printf("Error: TPM2_PCR_Read failed (0x%x)\n", rc);
    exit(1);
  }
  pcrValue = digests.digests[0].digest;

  return pcrValue;
}

int main(void) {
  TPM2B_SENSITIVE_CREATE inSensitive = { 0 };
  TPM2B_PUBLIC inPublic = { 0 };
  TPM2B_DATA outsideInfo = { 0 };
  TPML_PCR_SELECTION creationPCR = { 0 };

  TSS2_SYS_CONTEXT *sapi_context;
  TSS2_TCTI_CONTEXT *tcti_context;

  TPM2B_DIGEST pcrValue = { 0 };
  TPM2B_AUTH nv_auth = { 0 };

  TPM2B_MAX_BUFFER out_data = TPM2B_TYPE_INIT(TPM2B_MAX_BUFFER, buffer);

  TSS2_RC rval;

  size_t ctx_size;
  TSS2_ABI_VERSION abi_version = TSS2_ABI_VERSION_CURRENT;

  // set PCR selection
  creationPCR.count = 1;
  creationPCR.pcrSelections[0].hash = TPM2_ALG_SHA256;
  creationPCR.pcrSelections[0].sizeofSelect = PCR_SELECTION_SIZE;
  creationPCR.pcrSelections[0].pcrSelect[PCR_INDEX / 8] = 1 << (PCR_INDEX % 8);

  // Set up the sensitive data structure
  inSensitive.sensitive.userAuth.size = strlen(PASSWORD);
  memcpy(inSensitive.sensitive.userAuth.buffer, PASSWORD, inSensitive.sensitive.userAuth.size);

  // Set up the public data structure
  inPublic.publicArea.nameAlg = TPM2_ALG_SHA256;
  inPublic.publicArea.type = TPM2_ALG_RSA;
  inPublic.publicArea.objectAttributes |= TPMA_OBJECT_FIXEDTPM | TPMA_OBJECT_FIXEDPARENT | TPMA_OBJECT_SENSITIVEDATAORIGIN | TPMA_OBJECT_USERWITHAUTH | TPMA_OBJECT_DECRYPT;
  inPublic.publicArea.authPolicy.size = 0;
  inPublic.publicArea.parameters.rsaDetail
.continued from previous message...

// Set up the RSA parameters
inPublic.publicArea.parameters.rsaDetail.symmetric.algorithm = TPM2_ALG_NULL;
inPublic.publicArea.parameters.rsaDetail.scheme.scheme = TPM2_ALG_NULL;
inPublic.publicArea.parameters.rsaDetail.keyBits = 2048;
inPublic.publicArea.parameters.rsaDetail.exponent = 0;

// Create the TCTI context and SAPI context
tcti_context = tcti_ldr_load_conf(NULL);
if (tcti_context == NULL) {
printf("Failed to load TCTI context\n");
exit(1);
}

rval = Tss2_Sys_Initialize(&sapi_context, &ctx_size, NULL, tcti_context, &abi_version);
if (rval != TPM2_RC_SUCCESS) {
printf("Error: Tss2_Sys_Initialize failed (0x%x)\n", rval);
exit(1);
}

// Get the PCR value
pcrValue = getPcrValue(sapi_context);

// Set the outside info
outsideInfo.size = strlen(MESSAGE);
memcpy(outsideInfo.buffer, MESSAGE, outsideInfo.size);

// Set the auth value
nv_auth.size = strlen(PASSWORD);
memcpy(nv_auth.buffer, PASSWORD, nv_auth.size);

// Define the NV space
rval = Tss2_Sys_NV_DefineSpace(sapi_context, TPM2_RH_OWNER, &nv_auth, &inPublic, NULL);
if (rval != TPM2_RC_SUCCESS) {
printf("Error: Tss2_Sys_NV_DefineSpace failed (0x%x)\n", rval);
exit(1);
}

// Set the attributes
rval = Tss2_Sys_NV_SetAttribute(sapi_context, NV_INDEX, TPM2_NV_PER_AUTHWRITE | TPM2_NV_PER_READSTCLEAR | TPM2_NV_PER_WRITE_STCLEAR, 0);
if (rval != TPM2_RC_SUCCESS) {
printf("Error: Tss2_Sys_NV_SetAttribute failed (0x%x)\n", rval);
exit(1);
}

// Write the data to the NV space
rval = Tss2_Sys_NV_Write(sapi_context, TPM2_RH_OWNER, NV_INDEX, &nv_auth, &out_data, 0);
if (rval != TPM2_RC_SUCCESS) {
printf("Error: Tss2_Sys_NV_Write failed (0x%x)\n", rval);
exit(1);
}

// Seal the data to the PCR value
rval = Tss2_Sys_Create(sapi_context, TPM2_RH_OWNER, &nv_auth, &inSensitive, &inPublic, &outsideInfo, &creationPCR, &out_data, NULL, NULL, NULL);
if (rval != TPM2_RC_SUCCESS) {
printf("Error: Tss2_Sys_Create failed (0x%x)\n", rval);
exit(1);
}

// Output the sealed data
printf("Sealed data: ");
for (UINT16 i = 0; i < out_data.size; i++) {
printf("%02x", out_data.buffer[i]);
}
printf("\n");

// Shut down the TPM
Tss2_Sys_Shutdown(sapi_context, TPM2_SU_CLEAR);

return 0;
}
