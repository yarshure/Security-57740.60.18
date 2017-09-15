//
//  so_01_ServerEncryption.c
//  sec
//
//


#include <stdio.h>

#include "Security_regressions.h"

#include <Security/SecServerEncryptionSupport.h>
#include <Security/SecCertificatePriv.h>
#include <Security/SecTrust.h>
#include <utilities/SecCFWrappers.h>
#include <Security/SecPolicy.h>
#include <Security/SecKey.h>
#include <Security/SecECKey.h>

unsigned char private_key_der[] = {
    0x30, 0x77, 0x02, 0x01, 0x01, 0x04, 0x20, 0x6c, 0x30, 0x0c, 0xac, 0x33,
    0x9b, 0x4c, 0x70, 0x84, 0xe3, 0x41, 0x75, 0xe2, 0xe6, 0xf5, 0xe1, 0xd1,
    0xb1, 0x35, 0xd1, 0x58, 0xbd, 0x57, 0x8c, 0x2b, 0x1a, 0xf8, 0x70, 0xfa,
    0xca, 0xef, 0x17, 0xa0, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d,
    0x03, 0x01, 0x07, 0xa1, 0x44, 0x03, 0x42, 0x00, 0x04, 0xad, 0x2d, 0x12,
    0x6c, 0x3f, 0x8f, 0x85, 0xbf, 0x57, 0x96, 0xf6, 0xab, 0x84, 0x9b, 0x57,
    0xa3, 0x51, 0x33, 0xfe, 0xd4, 0x91, 0xec, 0xed, 0x02, 0x54, 0xed, 0x6a,
    0x11, 0x69, 0xd9, 0x28, 0x1f, 0x98, 0x01, 0x8f, 0x1a, 0xa7, 0x1d, 0x22,
    0x28, 0x74, 0xd7, 0x2f, 0x47, 0xb0, 0xb2, 0x8d, 0x84, 0xda, 0xcb, 0x88,
    0x01, 0xb9, 0x6e, 0x81, 0x5c, 0x06, 0xf1, 0x15, 0x12, 0x10, 0xcc, 0x70,
    0x90
};

unsigned char public_cert_der[] = {
    0x30, 0x82, 0x02, 0x41, 0x30, 0x82, 0x01, 0xe7, 0xa0, 0x03, 0x02, 0x01,
    0x02, 0x02, 0x01, 0x01, 0x30, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce,
    0x3d, 0x04, 0x03, 0x02, 0x30, 0x81, 0x85, 0x31, 0x15, 0x30, 0x13, 0x06,
    0x03, 0x55, 0x04, 0x03, 0x0c, 0x0c, 0x54, 0x65, 0x73, 0x74, 0x20, 0x45,
    0x43, 0x20, 0x43, 0x65, 0x72, 0x74, 0x31, 0x0e, 0x30, 0x0c, 0x06, 0x03,
    0x55, 0x04, 0x0a, 0x0c, 0x05, 0x41, 0x70, 0x70, 0x6c, 0x65, 0x31, 0x10,
    0x30, 0x0e, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x0c, 0x07, 0x43, 0x6f, 0x72,
    0x65, 0x20, 0x4f, 0x53, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04,
    0x08, 0x0c, 0x02, 0x43, 0x41, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55,
    0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03,
    0x55, 0x04, 0x07, 0x0c, 0x09, 0x43, 0x75, 0x70, 0x65, 0x72, 0x74, 0x69,
    0x6f, 0x6e, 0x31, 0x1c, 0x30, 0x1a, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
    0xf7, 0x0d, 0x01, 0x09, 0x01, 0x16, 0x0d, 0x74, 0x65, 0x73, 0x74, 0x40,
    0x74, 0x65, 0x73, 0x74, 0x2c, 0x6f, 0x72, 0x67, 0x30, 0x1e, 0x17, 0x0d,
    0x31, 0x34, 0x30, 0x35, 0x30, 0x32, 0x32, 0x33, 0x33, 0x31, 0x31, 0x33,
    0x5a, 0x17, 0x0d, 0x31, 0x37, 0x30, 0x31, 0x32, 0x36, 0x32, 0x33, 0x33,
    0x31, 0x31, 0x33, 0x5a, 0x30, 0x81, 0x85, 0x31, 0x15, 0x30, 0x13, 0x06,
    0x03, 0x55, 0x04, 0x03, 0x0c, 0x0c, 0x54, 0x65, 0x73, 0x74, 0x20, 0x45,
    0x43, 0x20, 0x43, 0x65, 0x72, 0x74, 0x31, 0x0e, 0x30, 0x0c, 0x06, 0x03,
    0x55, 0x04, 0x0a, 0x0c, 0x05, 0x41, 0x70, 0x70, 0x6c, 0x65, 0x31, 0x10,
    0x30, 0x0e, 0x06, 0x03, 0x55, 0x04, 0x0b, 0x0c, 0x07, 0x43, 0x6f, 0x72,
    0x65, 0x20, 0x4f, 0x53, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55, 0x04,
    0x08, 0x0c, 0x02, 0x43, 0x41, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55,
    0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x12, 0x30, 0x10, 0x06, 0x03,
    0x55, 0x04, 0x07, 0x0c, 0x09, 0x43, 0x75, 0x70, 0x65, 0x72, 0x74, 0x69,
    0x6f, 0x6e, 0x31, 0x1c, 0x30, 0x1a, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
    0xf7, 0x0d, 0x01, 0x09, 0x01, 0x16, 0x0d, 0x74, 0x65, 0x73, 0x74, 0x40,
    0x74, 0x65, 0x73, 0x74, 0x2c, 0x6f, 0x72, 0x67, 0x30, 0x59, 0x30, 0x13,
    0x06, 0x07, 0x2a, 0x86, 0x48, 0xce, 0x3d, 0x02, 0x01, 0x06, 0x08, 0x2a,
    0x86, 0x48, 0xce, 0x3d, 0x03, 0x01, 0x07, 0x03, 0x42, 0x00, 0x04, 0xad,
    0x2d, 0x12, 0x6c, 0x3f, 0x8f, 0x85, 0xbf, 0x57, 0x96, 0xf6, 0xab, 0x84,
    0x9b, 0x57, 0xa3, 0x51, 0x33, 0xfe, 0xd4, 0x91, 0xec, 0xed, 0x02, 0x54,
    0xed, 0x6a, 0x11, 0x69, 0xd9, 0x28, 0x1f, 0x98, 0x01, 0x8f, 0x1a, 0xa7,
    0x1d, 0x22, 0x28, 0x74, 0xd7, 0x2f, 0x47, 0xb0, 0xb2, 0x8d, 0x84, 0xda,
    0xcb, 0x88, 0x01, 0xb9, 0x6e, 0x81, 0x5c, 0x06, 0xf1, 0x15, 0x12, 0x10,
    0xcc, 0x70, 0x90, 0xa3, 0x46, 0x30, 0x44, 0x30, 0x0e, 0x06, 0x03, 0x55,
    0x1d, 0x0f, 0x01, 0x01, 0xff, 0x04, 0x04, 0x03, 0x02, 0x03, 0xb8, 0x30,
    0x12, 0x06, 0x03, 0x55, 0x1d, 0x25, 0x01, 0x01, 0xff, 0x04, 0x08, 0x30,
    0x06, 0x06, 0x04, 0x55, 0x1d, 0x25, 0x00, 0x30, 0x1e, 0x06, 0x03, 0x55,
    0x1d, 0x11, 0x04, 0x17, 0x30, 0x15, 0x82, 0x13, 0x64, 0x6f, 0x6e, 0x74,
    0x2e, 0x74, 0x72, 0x75, 0x73, 0x74, 0x2e, 0x6d, 0x65, 0x2e, 0x63, 0x72,
    0x61, 0x7a, 0x79, 0x30, 0x0a, 0x06, 0x08, 0x2a, 0x86, 0x48, 0xce, 0x3d,
    0x04, 0x03, 0x02, 0x03, 0x48, 0x00, 0x30, 0x45, 0x02, 0x21, 0x00, 0xb6,
    0x07, 0x44, 0xa0, 0xad, 0xb2, 0x68, 0x85, 0x5e, 0x40, 0x9e, 0x28, 0x7a,
    0xb6, 0x5a, 0x4d, 0x63, 0x2a, 0xb4, 0x3d, 0x51, 0x56, 0xe9, 0x00, 0x86,
    0xa9, 0xf3, 0x37, 0x82, 0x0d, 0x34, 0x5c, 0x02, 0x20, 0x1e, 0x6e, 0xb6,
    0x24, 0x55, 0xe4, 0xf2, 0x38, 0xc7, 0xf6, 0xbd, 0x78, 0x35, 0xdd, 0xdc,
    0x89, 0x88, 0xd9, 0x97, 0xb4, 0x47, 0x3a, 0xae, 0x02, 0x91, 0x7b, 0xf2,
    0xbf, 0x86, 0x65, 0xd5, 0x1f
};

const char *testbytes = "funky";
CFIndex testbytes_size = 5;

static int kTestTestCount = 8;
static void tests(void)
{
    CFErrorRef error = NULL;
    CFDataRef testData = CFDataCreateWithBytesNoCopy(kCFAllocatorDefault, (const UInt8 *)testbytes, testbytes_size, kCFAllocatorNull);

    SecCertificateRef cert = SecCertificateCreateWithBytes(kCFAllocatorDefault, public_cert_der, sizeof(public_cert_der));
    CFArrayRef certInArray = CFArrayCreateForCFTypes(kCFAllocatorDefault, cert, NULL);

    SecKeyRef full_key = SecKeyCreateECPrivateKey(kCFAllocatorDefault, private_key_der, sizeof(private_key_der), kSecKeyEncodingPkcs1);

    SecTrustRef trust = NULL;
    SecPolicyRef basic = SecPolicyCreateBasicX509();
    OSStatus status = SecTrustCreateWithCertificates(cert, basic, &trust);
    ok_status(status, "created");
    CFReleaseNull(basic);
    status = SecTrustSetAnchorCertificates(trust, certInArray);
    ok_status(status, "Anchors");

    SecTrustResultType result;
    status = SecTrustEvaluate(trust, &result);
    ok_status(status, "Trust evaluation");
    is(result, (SecTrustResultType)kSecTrustResultUnspecified, "Trust result");

    CFDataRef encrypted = SecCopyEncryptedToServer(trust, testData, &error);
    ok(encrypted != NULL, "Encrypt to server (%@)", error);
    CFReleaseNull(error);

    ok(!CFEqualSafe(testData, encrypted), "encrypted different");

    CFDataRef decrypted = SecCopyDecryptedForServer(full_key, encrypted, &error);
    ok(decrypted != NULL, "Decrypt from server (%@)", error);
    ok(CFEqualSafe(testData, decrypted), "round trip");


    CFReleaseNull(cert);
    CFReleaseNull(certInArray);
    CFReleaseNull(trust);
    CFReleaseNull(testData);
    CFReleaseNull(encrypted);
    CFReleaseNull(decrypted);
}

int so_01_serverencryption(int argc, char *const *argv)
{
    plan_tests(kTestTestCount);
    
    tests();
    
    return 0;    
}
