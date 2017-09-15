/*
 * Copyright (c) 2007-2010,2012-2014 Apple Inc. All Rights Reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */


#include <CoreFoundation/CoreFoundation.h>
#include <Security/SecCertificate.h>
#include <Security/SecCertificatePriv.h>
#include <Security/SecCertificateInternal.h>
#include <Security/SecKey.h>
#include <Security/SecRSAKey.h>
#include <Security/SecItem.h>
#include <Security/SecItemPriv.h>
#include <Security/SecIdentityPriv.h>
#include <Security/SecIdentity.h>
#include <utilities/array_size.h>
#include <stdlib.h>
#include <unistd.h>

#include "Security_regressions.h"

#define CFReleaseNull(CF) { CFTypeRef _cf = (CF); if (_cf) {  (CF) = NULL; CFRelease(_cf); } }

static const uint8_t _c0[] = {
    0x30, 0x82, 0x04, 0xbb, 0x30, 0x82, 0x03, 0xa3,
    0xa0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x01, 0x02,
    0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
    0xf7, 0x0d, 0x01, 0x01, 0x05, 0x05, 0x00, 0x30,
    0x62, 0x31, 0x0b, 0x30, 0x09, 0x06, 0x03, 0x55,
    0x04, 0x06, 0x13, 0x02, 0x55, 0x53, 0x31, 0x13,
    0x30, 0x11, 0x06, 0x03, 0x55, 0x04, 0x0a, 0x13,
    0x0a, 0x41, 0x70, 0x70, 0x6c, 0x65, 0x20, 0x49,
    0x6e, 0x63, 0x2e, 0x31, 0x26, 0x30, 0x24, 0x06,
    0x03, 0x55, 0x04, 0x0b, 0x13, 0x1d, 0x41, 0x70,
    0x70, 0x6c, 0x65, 0x20, 0x43, 0x65, 0x72, 0x74,
    0x69, 0x66, 0x69, 0x63, 0x61, 0x74, 0x69, 0x6f,
    0x6e, 0x20, 0x41, 0x75, 0x74, 0x68, 0x6f, 0x72,
    0x69, 0x74, 0x79, 0x31, 0x16, 0x30, 0x14, 0x06,
    0x03, 0x55, 0x04, 0x03, 0x13, 0x0d, 0x41, 0x70,
    0x70, 0x6c, 0x65, 0x20, 0x52, 0x6f, 0x6f, 0x74,
    0x20, 0x43, 0x41, 0x30, 0x1e, 0x17, 0x0d, 0x30,
    0x36, 0x30, 0x34, 0x32, 0x35, 0x32, 0x31, 0x34,
    0x30, 0x33, 0x36, 0x5a, 0x17, 0x0d, 0x33, 0x35,
    0x30, 0x32, 0x30, 0x39, 0x32, 0x31, 0x34, 0x30,
    0x33, 0x36, 0x5a, 0x30, 0x62, 0x31, 0x0b, 0x30,
    0x09, 0x06, 0x03, 0x55, 0x04, 0x06, 0x13, 0x02,
    0x55, 0x53, 0x31, 0x13, 0x30, 0x11, 0x06, 0x03,
    0x55, 0x04, 0x0a, 0x13, 0x0a, 0x41, 0x70, 0x70,
    0x6c, 0x65, 0x20, 0x49, 0x6e, 0x63, 0x2e, 0x31,
    0x26, 0x30, 0x24, 0x06, 0x03, 0x55, 0x04, 0x0b,
    0x13, 0x1d, 0x41, 0x70, 0x70, 0x6c, 0x65, 0x20,
    0x43, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69, 0x63,
    0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x41, 0x75,
    0x74, 0x68, 0x6f, 0x72, 0x69, 0x74, 0x79, 0x31,
    0x16, 0x30, 0x14, 0x06, 0x03, 0x55, 0x04, 0x03,
    0x13, 0x0d, 0x41, 0x70, 0x70, 0x6c, 0x65, 0x20,
    0x52, 0x6f, 0x6f, 0x74, 0x20, 0x43, 0x41, 0x30,
    0x82, 0x01, 0x22, 0x30, 0x0d, 0x06, 0x09, 0x2a,
    0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x01,
    0x05, 0x00, 0x03, 0x82, 0x01, 0x0f, 0x00, 0x30,
    0x82, 0x01, 0x0a, 0x02, 0x82, 0x01, 0x01, 0x00,
    0xe4, 0x91, 0xa9, 0x09, 0x1f, 0x91, 0xdb, 0x1e,
    0x47, 0x50, 0xeb, 0x05, 0xed, 0x5e, 0x79, 0x84,
    0x2d, 0xeb, 0x36, 0xa2, 0x57, 0x4c, 0x55, 0xec,
    0x8b, 0x19, 0x89, 0xde, 0xf9, 0x4b, 0x6c, 0xf5,
    0x07, 0xab, 0x22, 0x30, 0x02, 0xe8, 0x18, 0x3e,
    0xf8, 0x50, 0x09, 0xd3, 0x7f, 0x41, 0xa8, 0x98,
    0xf9, 0xd1, 0xca, 0x66, 0x9c, 0x24, 0x6b, 0x11,
    0xd0, 0xa3, 0xbb, 0xe4, 0x1b, 0x2a, 0xc3, 0x1f,
    0x95, 0x9e, 0x7a, 0x0c, 0xa4, 0x47, 0x8b, 0x5b,
    0xd4, 0x16, 0x37, 0x33, 0xcb, 0xc4, 0x0f, 0x4d,
    0xce, 0x14, 0x69, 0xd1, 0xc9, 0x19, 0x72, 0xf5,
    0x5d, 0x0e, 0xd5, 0x7f, 0x5f, 0x9b, 0xf2, 0x25,
    0x03, 0xba, 0x55, 0x8f, 0x4d, 0x5d, 0x0d, 0xf1,
    0x64, 0x35, 0x23, 0x15, 0x4b, 0x15, 0x59, 0x1d,
    0xb3, 0x94, 0xf7, 0xf6, 0x9c, 0x9e, 0xcf, 0x50,
    0xba, 0xc1, 0x58, 0x50, 0x67, 0x8f, 0x08, 0xb4,
    0x20, 0xf7, 0xcb, 0xac, 0x2c, 0x20, 0x6f, 0x70,
    0xb6, 0x3f, 0x01, 0x30, 0x8c, 0xb7, 0x43, 0xcf,
    0x0f, 0x9d, 0x3d, 0xf3, 0x2b, 0x49, 0x28, 0x1a,
    0xc8, 0xfe, 0xce, 0xb5, 0xb9, 0x0e, 0xd9, 0x5e,
    0x1c, 0xd6, 0xcb, 0x3d, 0xb5, 0x3a, 0xad, 0xf4,
    0x0f, 0x0e, 0x00, 0x92, 0x0b, 0xb1, 0x21, 0x16,
    0x2e, 0x74, 0xd5, 0x3c, 0x0d, 0xdb, 0x62, 0x16,
    0xab, 0xa3, 0x71, 0x92, 0x47, 0x53, 0x55, 0xc1,
    0xaf, 0x2f, 0x41, 0xb3, 0xf8, 0xfb, 0xe3, 0x70,
    0xcd, 0xe6, 0xa3, 0x4c, 0x45, 0x7e, 0x1f, 0x4c,
    0x6b, 0x50, 0x96, 0x41, 0x89, 0xc4, 0x74, 0x62,
    0x0b, 0x10, 0x83, 0x41, 0x87, 0x33, 0x8a, 0x81,
    0xb1, 0x30, 0x58, 0xec, 0x5a, 0x04, 0x32, 0x8c,
    0x68, 0xb3, 0x8f, 0x1d, 0xde, 0x65, 0x73, 0xff,
    0x67, 0x5e, 0x65, 0xbc, 0x49, 0xd8, 0x76, 0x9f,
    0x33, 0x14, 0x65, 0xa1, 0x77, 0x94, 0xc9, 0x2d,
    0x02, 0x03, 0x01, 0x00, 0x01, 0xa3, 0x82, 0x01,
    0x7a, 0x30, 0x82, 0x01, 0x76, 0x30, 0x0e, 0x06,
    0x03, 0x55, 0x1d, 0x0f, 0x01, 0x01, 0xff, 0x04,
    0x04, 0x03, 0x02, 0x01, 0x06, 0x30, 0x0f, 0x06,
    0x03, 0x55, 0x1d, 0x13, 0x01, 0x01, 0xff, 0x04,
    0x05, 0x30, 0x03, 0x01, 0x01, 0xff, 0x30, 0x1d,
    0x06, 0x03, 0x55, 0x1d, 0x0e, 0x04, 0x16, 0x04,
    0x14, 0x2b, 0xd0, 0x69, 0x47, 0x94, 0x76, 0x09,
    0xfe, 0xf4, 0x6b, 0x8d, 0x2e, 0x40, 0xa6, 0xf7,
    0x47, 0x4d, 0x7f, 0x08, 0x5e, 0x30, 0x1f, 0x06,
    0x03, 0x55, 0x1d, 0x23, 0x04, 0x18, 0x30, 0x16,
    0x80, 0x14, 0x2b, 0xd0, 0x69, 0x47, 0x94, 0x76,
    0x09, 0xfe, 0xf4, 0x6b, 0x8d, 0x2e, 0x40, 0xa6,
    0xf7, 0x47, 0x4d, 0x7f, 0x08, 0x5e, 0x30, 0x82,
    0x01, 0x11, 0x06, 0x03, 0x55, 0x1d, 0x20, 0x04,
    0x82, 0x01, 0x08, 0x30, 0x82, 0x01, 0x04, 0x30,
    0x82, 0x01, 0x00, 0x06, 0x09, 0x2a, 0x86, 0x48,
    0x86, 0xf7, 0x63, 0x64, 0x05, 0x01, 0x30, 0x81,
    0xf2, 0x30, 0x2a, 0x06, 0x08, 0x2b, 0x06, 0x01,
    0x05, 0x05, 0x07, 0x02, 0x01, 0x16, 0x1e, 0x68,
    0x74, 0x74, 0x70, 0x73, 0x3a, 0x2f, 0x2f, 0x77,
    0x77, 0x77, 0x2e, 0x61, 0x70, 0x70, 0x6c, 0x65,
    0x2e, 0x63, 0x6f, 0x6d, 0x2f, 0x61, 0x70, 0x70,
    0x6c, 0x65, 0x63, 0x61, 0x2f, 0x30, 0x81, 0xc3,
    0x06, 0x08, 0x2b, 0x06, 0x01, 0x05, 0x05, 0x07,
    0x02, 0x02, 0x30, 0x81, 0xb6, 0x1a, 0x81, 0xb3,
    0x52, 0x65, 0x6c, 0x69, 0x61, 0x6e, 0x63, 0x65,
    0x20, 0x6f, 0x6e, 0x20, 0x74, 0x68, 0x69, 0x73,
    0x20, 0x63, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69,
    0x63, 0x61, 0x74, 0x65, 0x20, 0x62, 0x79, 0x20,
    0x61, 0x6e, 0x79, 0x20, 0x70, 0x61, 0x72, 0x74,
    0x79, 0x20, 0x61, 0x73, 0x73, 0x75, 0x6d, 0x65,
    0x73, 0x20, 0x61, 0x63, 0x63, 0x65, 0x70, 0x74,
    0x61, 0x6e, 0x63, 0x65, 0x20, 0x6f, 0x66, 0x20,
    0x74, 0x68, 0x65, 0x20, 0x74, 0x68, 0x65, 0x6e,
    0x20, 0x61, 0x70, 0x70, 0x6c, 0x69, 0x63, 0x61,
    0x62, 0x6c, 0x65, 0x20, 0x73, 0x74, 0x61, 0x6e,
    0x64, 0x61, 0x72, 0x64, 0x20, 0x74, 0x65, 0x72,
    0x6d, 0x73, 0x20, 0x61, 0x6e, 0x64, 0x20, 0x63,
    0x6f, 0x6e, 0x64, 0x69, 0x74, 0x69, 0x6f, 0x6e,
    0x73, 0x20, 0x6f, 0x66, 0x20, 0x75, 0x73, 0x65,
    0x2c, 0x20, 0x63, 0x65, 0x72, 0x74, 0x69, 0x66,
    0x69, 0x63, 0x61, 0x74, 0x65, 0x20, 0x70, 0x6f,
    0x6c, 0x69, 0x63, 0x79, 0x20, 0x61, 0x6e, 0x64,
    0x20, 0x63, 0x65, 0x72, 0x74, 0x69, 0x66, 0x69,
    0x63, 0x61, 0x74, 0x69, 0x6f, 0x6e, 0x20, 0x70,
    0x72, 0x61, 0x63, 0x74, 0x69, 0x63, 0x65, 0x20,
    0x73, 0x74, 0x61, 0x74, 0x65, 0x6d, 0x65, 0x6e,
    0x74, 0x73, 0x2e, 0x30, 0x0d, 0x06, 0x09, 0x2a,
    0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x01, 0x05,
    0x05, 0x00, 0x03, 0x82, 0x01, 0x01, 0x00, 0x5c,
    0x36, 0x99, 0x4c, 0x2d, 0x78, 0xb7, 0xed, 0x8c,
    0x9b, 0xdc, 0xf3, 0x77, 0x9b, 0xf2, 0x76, 0xd2,
    0x77, 0x30, 0x4f, 0xc1, 0x1f, 0x85, 0x83, 0x85,
    0x1b, 0x99, 0x3d, 0x47, 0x37, 0xf2, 0xa9, 0x9b,
    0x40, 0x8e, 0x2c, 0xd4, 0xb1, 0x90, 0x12, 0xd8,
    0xbe, 0xf4, 0x73, 0x9b, 0xee, 0xd2, 0x64, 0x0f,
    0xcb, 0x79, 0x4f, 0x34, 0xd8, 0xa2, 0x3e, 0xf9,
    0x78, 0xff, 0x6b, 0xc8, 0x07, 0xec, 0x7d, 0x39,
    0x83, 0x8b, 0x53, 0x20, 0xd3, 0x38, 0xc4, 0xb1,
    0xbf, 0x9a, 0x4f, 0x0a, 0x6b, 0xff, 0x2b, 0xfc,
    0x59, 0xa7, 0x05, 0x09, 0x7c, 0x17, 0x40, 0x56,
    0x11, 0x1e, 0x74, 0xd3, 0xb7, 0x8b, 0x23, 0x3b,
    0x47, 0xa3, 0xd5, 0x6f, 0x24, 0xe2, 0xeb, 0xd1,
    0xb7, 0x70, 0xdf, 0x0f, 0x45, 0xe1, 0x27, 0xca,
    0xf1, 0x6d, 0x78, 0xed, 0xe7, 0xb5, 0x17, 0x17,
    0xa8, 0xdc, 0x7e, 0x22, 0x35, 0xca, 0x25, 0xd5,
    0xd9, 0x0f, 0xd6, 0x6b, 0xd4, 0xa2, 0x24, 0x23,
    0x11, 0xf7, 0xa1, 0xac, 0x8f, 0x73, 0x81, 0x60,
    0xc6, 0x1b, 0x5b, 0x09, 0x2f, 0x92, 0xb2, 0xf8,
    0x44, 0x48, 0xf0, 0x60, 0x38, 0x9e, 0x15, 0xf5,
    0x3d, 0x26, 0x67, 0x20, 0x8a, 0x33, 0x6a, 0xf7,
    0x0d, 0x82, 0xcf, 0xde, 0xeb, 0xa3, 0x2f, 0xf9,
    0x53, 0x6a, 0x5b, 0x64, 0xc0, 0x63, 0x33, 0x77,
    0xf7, 0x3a, 0x07, 0x2c, 0x56, 0xeb, 0xda, 0x0f,
    0x21, 0x0e, 0xda, 0xba, 0x73, 0x19, 0x4f, 0xb5,
    0xd9, 0x36, 0x7f, 0xc1, 0x87, 0x55, 0xd9, 0xa7,
    0x99, 0xb9, 0x32, 0x42, 0xfb, 0xd8, 0xd5, 0x71,
    0x9e, 0x7e, 0xa1, 0x52, 0xb7, 0x1b, 0xbd, 0x93,
    0x42, 0x24, 0x12, 0x2a, 0xc7, 0x0f, 0x1d, 0xb6,
    0x4d, 0x9c, 0x5e, 0x63, 0xc8, 0x4b, 0x80, 0x17,
    0x50, 0xaa, 0x8a, 0xd5, 0xda, 0xe4, 0xfc, 0xd0,
    0x09, 0x07, 0x37, 0xb0, 0x75, 0x75, 0x21,
};


/*
   Bag Attributes
friendlyName: uranusLeaf
localKeyID: 46 E0 8A 05 63 4D 17 3F CA A4 AA B6 5A DA CF BA 84 22 7C 23
subject=/CN=uranusLeaf/emailAddress=uranus@uranus.com
issuer=/CN=plutoCA/emailAddress=pluto@pluto.com
 */
static const uint8_t _c1[] = {
    0x30, 0x82, 0x02, 0xe0, 0x30, 0x82, 0x01, 0xc8,
    0xa0, 0x03, 0x02, 0x01, 0x02, 0x02, 0x01, 0x02,
    0x30, 0x0b, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
    0xf7, 0x0d, 0x01, 0x01, 0x05, 0x30, 0x32, 0x31,
    0x10, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x04, 0x03,
    0x0c, 0x07, 0x70, 0x6c, 0x75, 0x74, 0x6f, 0x43,
    0x41, 0x31, 0x1e, 0x30, 0x1c, 0x06, 0x09, 0x2a,
    0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x01,
    0x0c, 0x0f, 0x70, 0x6c, 0x75, 0x74, 0x6f, 0x40,
    0x70, 0x6c, 0x75, 0x74, 0x6f, 0x2e, 0x63, 0x6f,
    0x6d, 0x30, 0x1e, 0x17, 0x0d, 0x30, 0x35, 0x31,
    0x32, 0x31, 0x37, 0x30, 0x30, 0x30, 0x34, 0x32,
    0x35, 0x5a, 0x17, 0x0d, 0x30, 0x36, 0x31, 0x32,
    0x31, 0x37, 0x30, 0x30, 0x30, 0x34, 0x32, 0x35,
    0x5a, 0x30, 0x37, 0x31, 0x13, 0x30, 0x11, 0x06,
    0x03, 0x55, 0x04, 0x03, 0x0c, 0x0a, 0x75, 0x72,
    0x61, 0x6e, 0x75, 0x73, 0x4c, 0x65, 0x61, 0x66,
    0x31, 0x20, 0x30, 0x1e, 0x06, 0x09, 0x2a, 0x86,
    0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x01, 0x0c,
    0x11, 0x75, 0x72, 0x61, 0x6e, 0x75, 0x73, 0x40,
    0x75, 0x72, 0x61, 0x6e, 0x75, 0x73, 0x2e, 0x63,
    0x6f, 0x6d, 0x30, 0x82, 0x01, 0x22, 0x30, 0x0d,
    0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d,
    0x01, 0x01, 0x01, 0x05, 0x00, 0x03, 0x82, 0x01,
    0x0f, 0x00, 0x30, 0x82, 0x01, 0x0a, 0x02, 0x82,
    0x01, 0x01, 0x00, 0xa6, 0x82, 0x8e, 0xc6, 0x7e,
    0xc9, 0x8c, 0x99, 0x6f, 0xb0, 0x62, 0x32, 0x35,
    0xe7, 0xdb, 0xff, 0x34, 0x84, 0xdc, 0x72, 0xa8,
    0xef, 0x22, 0x6f, 0x93, 0x63, 0x64, 0x80, 0x80,
    0x5d, 0x50, 0x7e, 0xb4, 0x2e, 0x1b, 0x93, 0x93,
    0x49, 0xca, 0xae, 0xcd, 0x34, 0x44, 0x4b, 0xd7,
    0xfa, 0x9f, 0x3c, 0xfc, 0x9e, 0x65, 0xa9, 0xfb,
    0x5e, 0x5d, 0x18, 0xa3, 0xf8, 0xb0, 0x08, 0xac,
    0x8f, 0xfd, 0x03, 0xcb, 0xbd, 0x7f, 0xa0, 0x2a,
    0xa6, 0xea, 0xca, 0xa3, 0x24, 0xef, 0x7c, 0xc3,
    0xeb, 0x95, 0xcb, 0x90, 0x3f, 0x5e, 0xde, 0x78,
    0xf2, 0x3d, 0x32, 0x72, 0xdb, 0x33, 0x6e, 0x9b,
    0x52, 0x9f, 0x0c, 0x60, 0x4a, 0x24, 0xa1, 0xf6,
    0x3b, 0x80, 0xbd, 0xa1, 0xdc, 0x40, 0x03, 0xe7,
    0xa0, 0x59, 0x1f, 0xdb, 0xb4, 0xed, 0x57, 0xdc,
    0x74, 0x0d, 0x99, 0x5a, 0x12, 0x74, 0x64, 0xaa,
    0xb6, 0xa5, 0x96, 0x75, 0xf9, 0x42, 0x43, 0xe2,
    0x52, 0xc2, 0x57, 0x23, 0x75, 0xd7, 0xa9, 0x4f,
    0x07, 0x32, 0x99, 0xbd, 0x3d, 0x44, 0xbd, 0x04,
    0x62, 0xe5, 0xb7, 0x2c, 0x0c, 0x11, 0xc5, 0xb2,
    0x2e, 0xc4, 0x12, 0x1d, 0x7f, 0x42, 0x1e, 0x71,
    0xaf, 0x39, 0x2b, 0x78, 0x47, 0x92, 0x23, 0x44,
    0xef, 0xe3, 0xc1, 0x47, 0x69, 0x5a, 0xf1, 0x48,
    0xaa, 0x37, 0xa4, 0x94, 0x6b, 0x96, 0xe5, 0x4b,
    0xfd, 0x05, 0xc7, 0x9c, 0xcc, 0x38, 0xd1, 0x47,
    0x85, 0x60, 0x7f, 0xef, 0xe9, 0x2e, 0x25, 0x08,
    0xf8, 0x7d, 0x98, 0xdd, 0x6c, 0xeb, 0x4a, 0x32,
    0x33, 0x44, 0x0b, 0x61, 0xb3, 0xf9, 0xae, 0x26,
    0x41, 0xb5, 0x38, 0xdb, 0xcf, 0x13, 0x72, 0x23,
    0x5b, 0x66, 0x20, 0x86, 0x4d, 0x24, 0xc2, 0xd4,
    0x94, 0xde, 0xe3, 0x24, 0xb7, 0xcd, 0x75, 0x9e,
    0x1d, 0x9f, 0xbc, 0xd0, 0x60, 0x34, 0x7d, 0xf8,
    0xcb, 0x41, 0x39, 0x02, 0x03, 0x01, 0x00, 0x01,
    0x30, 0x0d, 0x06, 0x09, 0x2a, 0x86, 0x48, 0x86,
    0xf7, 0x0d, 0x01, 0x01, 0x05, 0x05, 0x00, 0x03,
    0x82, 0x01, 0x01, 0x00, 0x17, 0xa5, 0x22, 0xed,
    0xb8, 0x3e, 0x1f, 0x11, 0x99, 0xc5, 0xba, 0x28,
    0x3e, 0x7e, 0xa6, 0xeb, 0x02, 0x81, 0x06, 0xa1,
    0xc6, 0x80, 0xb9, 0x7e, 0x5c, 0x5a, 0x63, 0xe0,
    0x8d, 0xeb, 0xd0, 0xec, 0x9c, 0x3a, 0x94, 0x64,
    0x7c, 0x13, 0x54, 0x0d, 0xd6, 0xe3, 0x27, 0x88,
    0xa6, 0xd2, 0x4b, 0x36, 0xdd, 0x2e, 0xfa, 0x94,
    0xe5, 0x03, 0x27, 0xc9, 0xa6, 0x31, 0x02, 0xea,
    0x40, 0x77, 0x2e, 0x93, 0xc4, 0x4d, 0xe2, 0x70,
    0xe2, 0x67, 0x1c, 0xa8, 0x0d, 0xcd, 0x1a, 0x72,
    0x86, 0x2c, 0xea, 0xdc, 0x7f, 0x8c, 0x49, 0x2c,
    0xe7, 0x99, 0x13, 0xda, 0x3f, 0x58, 0x9e, 0xf5,
    0x4d, 0x3c, 0x8c, 0x1c, 0xed, 0x85, 0xa7, 0xe2,
    0xae, 0xda, 0x5f, 0xbe, 0x36, 0x1c, 0x9f, 0x5a,
    0xa0, 0xdc, 0x2a, 0xc0, 0xee, 0x71, 0x07, 0x26,
    0x8b, 0xe8, 0x8a, 0xf8, 0x2d, 0x36, 0x78, 0xc9,
    0x79, 0xfa, 0xbe, 0x98, 0x59, 0x95, 0x12, 0x24,
    0xf1, 0xda, 0x20, 0xc7, 0x78, 0xf9, 0x7c, 0x6a,
    0x24, 0x43, 0x82, 0xa8, 0x0f, 0xb1, 0x7d, 0x94,
    0xaa, 0x30, 0x35, 0xe5, 0x69, 0xdc, 0x0a, 0x0e,
    0xaf, 0x10, 0x5e, 0x1a, 0x81, 0x50, 0x5c, 0x7e,
    0x24, 0xb3, 0x07, 0x65, 0x4b, 0xc1, 0x7e, 0xc6,
    0x38, 0xdb, 0xd3, 0x6a, 0xf0, 0xd8, 0x85, 0x61,
    0x9a, 0x9f, 0xfe, 0x02, 0x46, 0x29, 0xb2, 0x9a,
    0xe2, 0x04, 0xe7, 0x72, 0xcc, 0x87, 0x46, 0xba,
    0x7d, 0xa8, 0xf9, 0xd0, 0x0f, 0x29, 0xfc, 0xfd,
    0xd1, 0xd0, 0x7f, 0x36, 0xc1, 0xd8, 0x7d, 0x88,
    0x03, 0x62, 0xf5, 0x8c, 0x00, 0xb5, 0xc2, 0x81,
    0x44, 0x67, 0x58, 0x11, 0xb4, 0x3a, 0xbb, 0xd1,
    0x8c, 0x94, 0x20, 0x60, 0xea, 0xa0, 0xac, 0xc1,
    0xf1, 0x08, 0x54, 0xb8, 0xf6, 0x5e, 0xac, 0xf1,
    0xec, 0x78, 0x69, 0x9d, 0x7e, 0x4d, 0x06, 0x3b,
    0x9b, 0x78, 0x78, 0x10
};

/*
   Bag Attributes
friendlyName: uranusLeaf
localKeyID: 46 E0 8A 05 63 4D 17 3F CA A4 AA B6 5A DA CF BA 84 22 7C 23
Key Attributes: <No Attributes>
 */
static const uint8_t _k1[] = {
    0x30, 0x82, 0x04, 0xa4, 0x02, 0x01, 0x00, 0x02,
    0x82, 0x01, 0x01, 0x00, 0xa6, 0x82, 0x8e, 0xc6,
    0x7e, 0xc9, 0x8c, 0x99, 0x6f, 0xb0, 0x62, 0x32,
    0x35, 0xe7, 0xdb, 0xff, 0x34, 0x84, 0xdc, 0x72,
    0xa8, 0xef, 0x22, 0x6f, 0x93, 0x63, 0x64, 0x80,
    0x80, 0x5d, 0x50, 0x7e, 0xb4, 0x2e, 0x1b, 0x93,
    0x93, 0x49, 0xca, 0xae, 0xcd, 0x34, 0x44, 0x4b,
    0xd7, 0xfa, 0x9f, 0x3c, 0xfc, 0x9e, 0x65, 0xa9,
    0xfb, 0x5e, 0x5d, 0x18, 0xa3, 0xf8, 0xb0, 0x08,
    0xac, 0x8f, 0xfd, 0x03, 0xcb, 0xbd, 0x7f, 0xa0,
    0x2a, 0xa6, 0xea, 0xca, 0xa3, 0x24, 0xef, 0x7c,
    0xc3, 0xeb, 0x95, 0xcb, 0x90, 0x3f, 0x5e, 0xde,
    0x78, 0xf2, 0x3d, 0x32, 0x72, 0xdb, 0x33, 0x6e,
    0x9b, 0x52, 0x9f, 0x0c, 0x60, 0x4a, 0x24, 0xa1,
    0xf6, 0x3b, 0x80, 0xbd, 0xa1, 0xdc, 0x40, 0x03,
    0xe7, 0xa0, 0x59, 0x1f, 0xdb, 0xb4, 0xed, 0x57,
    0xdc, 0x74, 0x0d, 0x99, 0x5a, 0x12, 0x74, 0x64,
    0xaa, 0xb6, 0xa5, 0x96, 0x75, 0xf9, 0x42, 0x43,
    0xe2, 0x52, 0xc2, 0x57, 0x23, 0x75, 0xd7, 0xa9,
    0x4f, 0x07, 0x32, 0x99, 0xbd, 0x3d, 0x44, 0xbd,
    0x04, 0x62, 0xe5, 0xb7, 0x2c, 0x0c, 0x11, 0xc5,
    0xb2, 0x2e, 0xc4, 0x12, 0x1d, 0x7f, 0x42, 0x1e,
    0x71, 0xaf, 0x39, 0x2b, 0x78, 0x47, 0x92, 0x23,
    0x44, 0xef, 0xe3, 0xc1, 0x47, 0x69, 0x5a, 0xf1,
    0x48, 0xaa, 0x37, 0xa4, 0x94, 0x6b, 0x96, 0xe5,
    0x4b, 0xfd, 0x05, 0xc7, 0x9c, 0xcc, 0x38, 0xd1,
    0x47, 0x85, 0x60, 0x7f, 0xef, 0xe9, 0x2e, 0x25,
    0x08, 0xf8, 0x7d, 0x98, 0xdd, 0x6c, 0xeb, 0x4a,
    0x32, 0x33, 0x44, 0x0b, 0x61, 0xb3, 0xf9, 0xae,
    0x26, 0x41, 0xb5, 0x38, 0xdb, 0xcf, 0x13, 0x72,
    0x23, 0x5b, 0x66, 0x20, 0x86, 0x4d, 0x24, 0xc2,
    0xd4, 0x94, 0xde, 0xe3, 0x24, 0xb7, 0xcd, 0x75,
    0x9e, 0x1d, 0x9f, 0xbc, 0xd0, 0x60, 0x34, 0x7d,
    0xf8, 0xcb, 0x41, 0x39, 0x02, 0x03, 0x01, 0x00,
    0x01, 0x02, 0x82, 0x01, 0x00, 0x4d, 0x27, 0xf2,
    0x40, 0xc8, 0x3f, 0x5c, 0x87, 0x3c, 0xd9, 0xde,
    0xa6, 0xa5, 0x93, 0xea, 0xbd, 0x36, 0xf8, 0xd9,
    0xad, 0xc7, 0xda, 0x07, 0x7a, 0xec, 0x31, 0x02,
    0x41, 0x09, 0x3a, 0x34, 0x32, 0x82, 0x0b, 0x5b,
    0x7b, 0xe6, 0xa4, 0x2a, 0xe7, 0x14, 0xef, 0x43,
    0x36, 0x61, 0xbe, 0x20, 0x4b, 0x82, 0x43, 0x63,
    0x98, 0x80, 0x82, 0x19, 0x61, 0x71, 0x99, 0xaa,
    0xf8, 0x59, 0xfd, 0xde, 0xa0, 0x03, 0xa8, 0xab,
    0x9a, 0xec, 0x28, 0xac, 0x63, 0x79, 0x75, 0x84,
    0x03, 0xac, 0x45, 0x5e, 0x04, 0x15, 0xb3, 0x47,
    0xa2, 0x8f, 0x28, 0xb0, 0x72, 0xd0, 0x06, 0x02,
    0xaf, 0x1e, 0x0a, 0x0a, 0xe9, 0x11, 0x35, 0x4a,
    0x04, 0x42, 0xb5, 0x0f, 0xd2, 0xcf, 0x4d, 0xdf,
    0xdb, 0xef, 0x58, 0xbd, 0xf3, 0xa5, 0x3b, 0x11,
    0x3f, 0xc5, 0x47, 0x81, 0x85, 0xad, 0xd7, 0x1f,
    0x58, 0x06, 0x42, 0xdc, 0x37, 0x3c, 0xdb, 0x98,
    0x33, 0xa1, 0xc6, 0x80, 0x07, 0xe0, 0x2b, 0xc5,
    0xf5, 0x60, 0x35, 0x6a, 0xa2, 0x06, 0x40, 0x4a,
    0xac, 0x64, 0x02, 0x58, 0x4d, 0x07, 0xe3, 0x69,
    0xd7, 0xe0, 0x8f, 0xb5, 0xf4, 0xbc, 0xfa, 0xab,
    0x1a, 0xb0, 0xfa, 0x29, 0xf8, 0xca, 0xde, 0x78,
    0xf0, 0x89, 0xe2, 0xf9, 0xb7, 0x68, 0x5b, 0x0e,
    0xdc, 0x4e, 0x8a, 0x56, 0x8d, 0x33, 0x20, 0x2e,
    0xed, 0x2e, 0xab, 0x6f, 0xba, 0x77, 0xef, 0xe6,
    0x12, 0x62, 0x49, 0x9e, 0x87, 0x76, 0x1c, 0x1e,
    0xf4, 0x0e, 0x9e, 0x78, 0x98, 0x91, 0x1a, 0xe3,
    0xb4, 0x51, 0x4b, 0x8c, 0x2f, 0x08, 0x97, 0x8f,
    0xf9, 0x68, 0x61, 0x40, 0xcd, 0xb6, 0x10, 0xb4,
    0xfb, 0x75, 0xb4, 0x20, 0xc1, 0x5a, 0xda, 0x64,
    0xfd, 0x51, 0x06, 0x85, 0x9a, 0x9e, 0x5d, 0x82,
    0x14, 0xd4, 0x41, 0x4e, 0x75, 0x10, 0xb5, 0x7b,
    0xd0, 0x4c, 0xd1, 0x00, 0x01, 0x02, 0x81, 0x81,
    0x00, 0xcf, 0x8e, 0x68, 0x04, 0x67, 0x09, 0xa9,
    0x6e, 0xff, 0x11, 0x8c, 0xe5, 0xe4, 0x16, 0xdd,
    0xb6, 0xa6, 0x55, 0xca, 0x4b, 0x0b, 0xbb, 0xb7,
    0xf5, 0xe5, 0x73, 0xf3, 0x24, 0x84, 0x29, 0xb2,
    0xc3, 0xbc, 0x7f, 0x2b, 0x4a, 0xc7, 0xdf, 0x46,
    0x8e, 0xe1, 0x35, 0x69, 0x1b, 0x8e, 0x9f, 0x6b,
    0x4d, 0xf3, 0x65, 0xae, 0x3d, 0x87, 0x2b, 0xc9,
    0xf0, 0x8c, 0xf2, 0x88, 0x2f, 0x1b, 0x79, 0x80,
    0xd2, 0xb2, 0x64, 0x0a, 0xcc, 0x66, 0x69, 0x4c,
    0xa1, 0x85, 0xc4, 0x6a, 0x94, 0x46, 0x70, 0x69,
    0xbc, 0x8c, 0x1c, 0x62, 0x65, 0x4d, 0x68, 0xcc,
    0xe3, 0x3c, 0x6c, 0xe7, 0xd1, 0x09, 0xed, 0xdd,
    0x42, 0x10, 0x11, 0x6b, 0xdd, 0x7c, 0xe3, 0xe1,
    0x3b, 0x3b, 0x0d, 0x01, 0x6d, 0xca, 0x2f, 0x4b,
    0x45, 0x5e, 0x76, 0x5d, 0x5c, 0x6f, 0x53, 0xa4,
    0x38, 0x74, 0x75, 0x94, 0x2c, 0xda, 0xf8, 0xa6,
    0x01, 0x02, 0x81, 0x81, 0x00, 0xcd, 0x5f, 0x9d,
    0x6c, 0x94, 0xf6, 0x44, 0x37, 0x72, 0xfe, 0xcf,
    0xbe, 0x82, 0x96, 0x24, 0x22, 0x12, 0x07, 0x6f,
    0xd1, 0x57, 0x7b, 0xc7, 0x63, 0x20, 0xf5, 0x93,
    0x79, 0x70, 0x0b, 0xe4, 0x38, 0x19, 0x62, 0x7b,
    0x89, 0x3e, 0x45, 0xdf, 0xd6, 0xae, 0x9d, 0x0d,
    0xa8, 0x76, 0xc1, 0xbd, 0x04, 0x2b, 0xaa, 0x30,
    0x6a, 0xac, 0x65, 0x91, 0x61, 0xf0, 0xf8, 0x5d,
    0xa3, 0x53, 0xa4, 0xfb, 0x99, 0xac, 0x46, 0x7a,
    0x12, 0x4b, 0xf7, 0xa7, 0x48, 0x41, 0x61, 0x48,
    0x26, 0x5c, 0x68, 0x2f, 0x73, 0x91, 0xe4, 0x74,
    0xcd, 0xc9, 0x8b, 0xe7, 0x26, 0xe4, 0x35, 0xde,
    0x32, 0x6b, 0x24, 0x49, 0xf2, 0x04, 0x67, 0x3d,
    0x31, 0x8f, 0x22, 0xe5, 0x49, 0xae, 0x49, 0x94,
    0xb3, 0x45, 0x2b, 0xed, 0x6f, 0x9c, 0xc7, 0x80,
    0xf0, 0x42, 0xd5, 0x8f, 0x27, 0xd6, 0xd6, 0x49,
    0xf2, 0x16, 0xcc, 0x4b, 0x39, 0x02, 0x81, 0x81,
    0x00, 0xbb, 0xb7, 0xd7, 0x59, 0xcb, 0xfb, 0x10,
    0x13, 0xc4, 0x7b, 0x92, 0x0c, 0x45, 0xcb, 0x6c,
    0x81, 0x0a, 0x55, 0x63, 0x1d, 0x96, 0xa2, 0x13,
    0xd2, 0x40, 0xd1, 0x2a, 0xa1, 0xe7, 0x2a, 0x73,
    0x74, 0xd6, 0x61, 0xc9, 0xbc, 0xdb, 0xa2, 0x93,
    0x85, 0x1c, 0x28, 0x9b, 0x44, 0x82, 0x2c, 0xaa,
    0xf7, 0x18, 0x60, 0xe9, 0x42, 0xda, 0xa2, 0xff,
    0x04, 0x21, 0xe6, 0x24, 0xc7, 0x3e, 0x39, 0x19,
    0x0a, 0xf6, 0xae, 0xc6, 0x99, 0x71, 0x32, 0x61,
    0x4d, 0x60, 0xd7, 0x71, 0x71, 0x63, 0x77, 0xbe,
    0x19, 0xfa, 0x3a, 0x9d, 0xbf, 0x73, 0x50, 0x8a,
    0xa6, 0x26, 0x7b, 0x74, 0xfa, 0x39, 0xd9, 0xb9,
    0x18, 0x4b, 0xc2, 0x05, 0xe5, 0x8f, 0x53, 0xe6,
    0xdc, 0x14, 0x1f, 0x42, 0x20, 0x93, 0x11, 0x4d,
    0x29, 0x93, 0x32, 0xc8, 0x63, 0x96, 0x88, 0x76,
    0x69, 0x5c, 0xe3, 0x0e, 0xbd, 0xb6, 0xd9, 0xd6,
    0x01, 0x02, 0x81, 0x80, 0x62, 0xa2, 0xed, 0x84,
    0xdc, 0xf6, 0x7a, 0x44, 0xf7, 0x62, 0x12, 0x7c,
    0xb9, 0x53, 0x4a, 0xff, 0x62, 0x11, 0x58, 0x4e,
    0xfe, 0xe9, 0x60, 0x15, 0xe8, 0x1a, 0x8a, 0x3d,
    0xe4, 0xe6, 0x91, 0x31, 0xb0, 0x5f, 0x70, 0x5d,
    0xb6, 0x1e, 0xf1, 0x26, 0xb6, 0xae, 0x8f, 0x84,
    0xbd, 0xa4, 0xc7, 0x17, 0x5d, 0xb1, 0x5b, 0x97,
    0xa0, 0x3d, 0x17, 0xda, 0x26, 0x55, 0xe3, 0x03,
    0x32, 0x85, 0x26, 0xa1, 0xe3, 0xef, 0xe5, 0x69,
    0x2c, 0x3b, 0x41, 0x88, 0x9e, 0x7e, 0x0e, 0x9c,
    0xfd, 0xfc, 0xbb, 0xed, 0x91, 0xc0, 0x5b, 0xa9,
    0x0a, 0x87, 0xba, 0xf9, 0x1e, 0xda, 0x10, 0x61,
    0xbe, 0xbb, 0xab, 0x18, 0x25, 0xad, 0x3f, 0xe2,
    0xb1, 0x90, 0x5c, 0xf7, 0x4a, 0x51, 0xe4, 0xad,
    0x45, 0x27, 0x97, 0xdd, 0xe7, 0x3a, 0x9a, 0x5e,
    0xca, 0x7a, 0xaf, 0x4a, 0xbf, 0x10, 0x24, 0x6b,
    0xb5, 0x2f, 0x61, 0x61, 0x02, 0x81, 0x81, 0x00,
    0x85, 0x7c, 0x78, 0xa5, 0x11, 0xdf, 0xc3, 0x6a,
    0x38, 0x48, 0xfa, 0x7e, 0x48, 0xf0, 0x5a, 0x58,
    0xe2, 0xc5, 0x83, 0x4e, 0x38, 0x3f, 0x4a, 0x2b,
    0x07, 0x57, 0x31, 0xe7, 0xbe, 0x50, 0xb1, 0xbb,
    0x24, 0xf3, 0x3d, 0x8b, 0x53, 0xb7, 0xd1, 0x47,
    0x72, 0x5e, 0xd5, 0xd6, 0x4c, 0xce, 0x2c, 0x46,
    0x61, 0x9a, 0xaa, 0xc3, 0x0e, 0xd4, 0x23, 0x2c,
    0xdd, 0xf5, 0xb7, 0xad, 0x38, 0x52, 0x17, 0xc4,
    0x16, 0xbb, 0xda, 0x1c, 0x61, 0xb1, 0xca, 0x8d,
    0xb2, 0xa0, 0xbe, 0x4f, 0x3d, 0x19, 0x0e, 0xe0,
    0x0e, 0x52, 0xad, 0xf3, 0xaf, 0xd9, 0xcc, 0x78,
    0xc2, 0xb1, 0x5e, 0x05, 0x5e, 0xf2, 0x27, 0x84,
    0x15, 0xe4, 0x8f, 0xca, 0xc5, 0x92, 0x43, 0xe0,
    0x24, 0x8d, 0xf2, 0x5d, 0x55, 0xcc, 0x9d, 0x2f,
    0xa9, 0xf6, 0x9b, 0x67, 0x6a, 0x87, 0x74, 0x36,
    0x34, 0x7c, 0xd4, 0x9d, 0xff, 0xad, 0xee, 0x69
};

static const uint8_t _k1_digest[] = {
    0x46, 0xE0, 0x8A, 0x05, 0x63, 0x4D, 0x17, 0x3F,
    0xCA, 0xA4, 0xAA, 0xB6, 0x5A, 0xDA, 0xCF, 0xBA,
    0x84, 0x22, 0x7C, 0x23
};

/* Create and identity and try to retrieve it. */
static void tests(void)
{
    SecCertificateRef cert = NULL;
    SecKeyRef privKey = NULL;
    SecIdentityRef identity = NULL;

    isnt(cert = SecCertificateCreateWithBytes(NULL, _c1, sizeof(_c1)),
            NULL, "create certificate");
    isnt(privKey = SecKeyCreateRSAPrivateKey(NULL, _k1, sizeof(_k1),
                kSecKeyEncodingPkcs1), NULL, "create private key");

    const void *certkeys[] = {
        kSecValueRef
    };
    const void *certvalues[] = {
        cert
    };
    CFDictionaryRef certDict = CFDictionaryCreate(NULL, certkeys, certvalues,
            array_size(certkeys), NULL, NULL);
    ok_status(SecItemAdd(certDict, NULL), "add certificate");
    CFReleaseNull(certDict);

    const void *privkeys[] = {
        kSecValueRef
    };
    const void *privvalues[] = {
        privKey
    };
    CFDictionaryRef privDict = CFDictionaryCreate(NULL, privkeys, privvalues,
            array_size(privkeys), NULL, NULL);
    ok_status(SecItemAdd(privDict, NULL), "add private key");
    CFReleaseNull(privDict);

    isnt(identity = SecIdentityCreate(NULL, cert, privKey), NULL, "create identity");

    /* Lookup the key and certificate using SecItemCopyMatching(). */
    CFDataRef pk_digest = CFDataCreate(NULL, _k1_digest, sizeof(_k1_digest));
    const void *q_keys[] = {
        kSecClass,
        kSecAttrApplicationLabel,
        kSecReturnRef
    };
    const void *q_values[] = {
        kSecClassKey,
        pk_digest,
        kCFBooleanTrue
    };
    CFDictionaryRef query = CFDictionaryCreate(NULL, q_keys, q_values,
            array_size(q_keys), NULL, NULL);
    CFTypeRef result_key;
    ok_status(SecItemCopyMatching(query, &result_key), "lookup key");

    isnt(CFEqual(privKey, result_key), 0, "keys match");
    CFReleaseNull(query);

    q_keys[1] = kSecAttrPublicKeyHash;
    q_values[0] = kSecClassCertificate;
    query = CFDictionaryCreate(NULL, q_keys, q_values,
            array_size(q_keys), NULL, NULL);
    CFTypeRef result_cert;
    ok_status(SecItemCopyMatching(query, &result_cert), "lookup certificate");
    isnt(CFEqual(cert, result_cert), 0, "certificates match");
    CFReleaseNull(query);

    /* Cleanup. */
    CFReleaseNull(result_key);
    CFReleaseNull(result_cert);

    /* identity lookup */
    const void *idnt_keys[] = {
        kSecClass,
        kSecAttrApplicationLabel,
        kSecReturnRef
    };
    const void *idnt_values[] = {
        kSecClassIdentity,
        pk_digest,
        kCFBooleanTrue
    };
    CFTypeRef result_idnt;
    SecCertificateRef result_cert2;
    query = CFDictionaryCreate(NULL, idnt_keys, idnt_values,
            array_size(idnt_keys), NULL, NULL);
    ok_status(SecItemCopyMatching(query, &result_idnt), "lookup identity");
    isnt(result_idnt, NULL, "found identity?");
    is(CFGetRetainCount(result_idnt), 1, "result_idnt rc = 1");
    isnt(CFEqual(identity, result_idnt), 0, "identities match");
    CFReleaseNull(identity);

    ok_status(SecIdentityCopyCertificate((SecIdentityRef)result_idnt, &result_cert2), "get cert from identity");
    isnt(CFEqual(cert, result_cert2), 0, "certificates match");
    CFRelease(query);
    CFRelease(pk_digest);
    CFReleaseNull(result_cert2);

    certDict = CFDictionaryCreate(NULL, certkeys, certvalues,
            array_size(certkeys), NULL, NULL);
    ok_status(SecItemDelete(certDict), "delete certificate via ref");
    is_status(errSecItemNotFound, SecItemCopyMatching(certDict, NULL), "verify certificate is gone");

    CFReleaseNull(certDict);

    privDict = CFDictionaryCreate(NULL, privkeys, privvalues,
            array_size(privkeys), NULL, NULL);
    ok_status(SecItemDelete(privDict), "delete key via ref");
    is_status(errSecItemNotFound, SecItemCopyMatching(privDict, NULL), "verify key is gone");
    CFReleaseNull(privDict);

    /* add certificate to offset cert row id from key row id */
    SecCertificateRef apple_ca_cert = NULL;
    isnt(apple_ca_cert = SecCertificateCreateWithBytes(NULL, _c0, sizeof(_c0)),
            NULL, "create apple ca certificate");
    CFDictionaryRef appleCertDict = CFDictionaryCreate(NULL, (const void **)&kSecValueRef, (const void **)&apple_ca_cert, 1, NULL, NULL);
    ok_status(SecItemAdd(appleCertDict, NULL), "add apple ca certificate to offset key and cert rowid");

    /* add identity, get persistent ref */
    const void *keys_identity[] = {     kSecValueRef,   kSecReturnPersistentRef };
    const void *values_identity[] = {   result_idnt,    kCFBooleanTrue };
    CFDictionaryRef identity_add = CFDictionaryCreate(NULL, keys_identity, values_identity,
        array_size(keys_identity), NULL, NULL);
    CFTypeRef persist = NULL;
    ok_status(SecItemAdd(identity_add, &persist), "add identity ref");
    ok(persist, "got back persistent ref");
    /* <rdar://problem/6537195> SecItemAdd returns success when it shouldn't */
    CFTypeRef persist_again = NULL;
    is_status(errSecDuplicateItem, SecItemAdd(identity_add, &persist_again),
        "fail to add identity ref again");
    ok(!persist_again, "no persistent ref this time");

    /* find by persistent ref */
    const void *keys_persist[] = { kSecReturnRef, kSecValuePersistentRef };
    const void *values_persist[] = { kCFBooleanTrue, persist };
    CFDictionaryRef persist_find = CFDictionaryCreate(NULL, keys_persist, values_persist,
	(array_size(keys_persist)), NULL, NULL);
    CFTypeRef results2 = NULL;
    ok_status(SecItemCopyMatching(persist_find, &results2), "find identity by persistent ref");
    is(CFGetRetainCount(results2), 1, "results2 rc = 1");
    // not implemented ok(CFEqual(result_idnt, results2), "same item (attributes)");
    CFReleaseNull(results2);

    /* find identity, key and cert by ref and return persistent ref */
    const void *keys_ref_to_persist[] = { kSecReturnPersistentRef, kSecValueRef };
    const void *values_ref_to_persist[] = { kCFBooleanTrue, NULL };
    CFTypeRef items[] = { result_idnt, privKey, cert, NULL };
    CFTypeRef *item = items;
    while (*item) {
        values_ref_to_persist[1] = *item;
        CFDictionaryRef ref_to_persist_find = CFDictionaryCreate(NULL, keys_ref_to_persist, values_ref_to_persist,
            (array_size(keys_ref_to_persist)), NULL, NULL);
        results2 = NULL;
        ok_status(SecItemCopyMatching(ref_to_persist_find, &results2), "find persistent ref for identity ref");
        ok(NULL != results2, "good persistent ref");
        is(CFGetRetainCount(results2), 1, "results2 rc = 1");
        CFReleaseNull(results2);
        CFReleaseNull(ref_to_persist_find);
        item++;
    }

    /* delete identity by identity ref */
    ok_status(SecItemDelete(identity_add), "delete identity by identity ref");
    is(SecItemCopyMatching(persist_find, &results2), errSecItemNotFound,
        "make sure identity by persistent ref is no longer there");
    CFRelease(persist_find);
    CFReleaseNull(persist);
    ok_status(SecItemAdd(identity_add, &persist), "add identity ref back");
    CFRelease(identity_add);

    /* delete identity by persistent ref */
    CFDictionaryRef persist_delete = CFDictionaryCreate(NULL,
        (const void **)&kSecValuePersistentRef, &persist, 1, NULL, NULL);
    ok_status(SecItemDelete(persist_delete),
        "delete identity by persistent ref");
    is(SecItemCopyMatching(persist_delete, &results2), errSecItemNotFound,
        "make sure identity by persistent ref is no longer there");
    CFRelease(persist_delete);
    CFReleaseNull(persist);

	/* add identity with a label set */
	CFStringRef zomg_label = CFSTR("zomg");
	CFMutableDictionaryRef lbl_idnt_query =
		CFDictionaryCreateMutable(kCFAllocatorDefault, 0,
		&kCFTypeDictionaryKeyCallBacks, &kCFTypeDictionaryValueCallBacks);
	CFDictionarySetValue(lbl_idnt_query, kSecValueRef, result_idnt);
	CFDictionarySetValue(lbl_idnt_query, kSecAttrLabel, zomg_label);
    ok_status(SecItemAdd(lbl_idnt_query, NULL), "add identity ref");

	/* find identity with label*/
	CFDictionaryRemoveAllValues(lbl_idnt_query);
	CFDictionarySetValue(lbl_idnt_query, kSecClass, kSecClassIdentity);
	CFDictionarySetValue(lbl_idnt_query, kSecAttrLabel, zomg_label);
	ok_status(SecItemCopyMatching(lbl_idnt_query, NULL), "find identity by label");

	/* find certs with label */
	CFTypeRef zomg_cert;
	CFDictionaryRemoveAllValues(lbl_idnt_query);
	CFDictionarySetValue(lbl_idnt_query, kSecClass, kSecClassCertificate);
	CFDictionarySetValue(lbl_idnt_query, kSecAttrLabel, zomg_label);
	CFDictionarySetValue(lbl_idnt_query, kSecReturnRef, kCFBooleanTrue);
	ok_status(SecItemCopyMatching(lbl_idnt_query, &zomg_cert), "find cert by label");

	/* find keys with label */
	CFTypeRef zomg_key;
	CFDictionaryRemoveAllValues(lbl_idnt_query);
	CFDictionarySetValue(lbl_idnt_query, kSecClass, kSecClassKey);
	CFDictionarySetValue(lbl_idnt_query, kSecAttrLabel, zomg_label);
	CFDictionarySetValue(lbl_idnt_query, kSecReturnRef, kCFBooleanTrue);
	ok_status(SecItemCopyMatching(lbl_idnt_query, &zomg_key), "find key by label");

	/* update label on key */
	CFStringRef new_label_value = CFSTR("zzzomg");
	CFDictionaryRef new_label = CFDictionaryCreate(kCFAllocatorDefault, (const void **)&kSecAttrLabel, (const void **)&new_label_value, 1, NULL, NULL);
	CFDictionaryRemoveAllValues(lbl_idnt_query);
	CFDictionarySetValue(lbl_idnt_query, kSecValueRef, zomg_key);
	ok_status(SecItemUpdate(lbl_idnt_query, new_label), "update label to zzzomg for key");

	CFTypeRef zomg_idnt = NULL;
	CFDictionaryRemoveAllValues(lbl_idnt_query);
	CFDictionarySetValue(lbl_idnt_query, kSecReturnRef, kCFBooleanTrue);
	CFDictionarySetValue(lbl_idnt_query, kSecAttrLabel, zomg_label);
	CFDictionarySetValue(lbl_idnt_query, kSecClass, kSecClassIdentity);
	ok_status(SecItemCopyMatching(lbl_idnt_query, &zomg_idnt), "still finding zomg ident");
	CFReleaseNull(zomg_idnt);


	CFDictionaryRemoveAllValues(lbl_idnt_query);
	CFDictionarySetValue(lbl_idnt_query, kSecValueRef, zomg_cert);
	ok_status(SecItemUpdate(lbl_idnt_query, new_label), "update label to zzzomg for cert");
	CFReleaseNull(new_label);

	CFDictionaryRemoveAllValues(lbl_idnt_query);
	CFDictionarySetValue(lbl_idnt_query, kSecReturnRef, kCFBooleanTrue);
	CFDictionarySetValue(lbl_idnt_query, kSecAttrLabel, zomg_label);
	CFDictionarySetValue(lbl_idnt_query, kSecClass, kSecClassIdentity);
	is_status(errSecItemNotFound, SecItemCopyMatching(lbl_idnt_query, &zomg_idnt), "no longer find identity by label");

	CFDictionaryRemoveAllValues(lbl_idnt_query);
	CFDictionarySetValue(lbl_idnt_query, kSecReturnRef, kCFBooleanTrue);
	CFDictionarySetValue(lbl_idnt_query, kSecAttrLabel, new_label_value);
	CFDictionarySetValue(lbl_idnt_query, kSecClass, kSecClassIdentity);
	ok_status(SecItemCopyMatching(lbl_idnt_query, &zomg_idnt), "finding ident with zzzomg label");

    /* Find zomg identity with canonical issuer */
    {
        unsigned char DN[] = {
            0x30, 0x32, 0x31, 0x10, 0x30, 0x0e, 0x06, 0x03, 0x55, 0x04, 0x03, 0x0c,
            0x07, 0x70, 0x6c, 0x75, 0x74, 0x6f, 0x43, 0x41, 0x31, 0x1e, 0x30, 0x1c,
            0x06, 0x09, 0x2a, 0x86, 0x48, 0x86, 0xf7, 0x0d, 0x01, 0x09, 0x01, 0x0c,
            0x0f, 0x70, 0x6c, 0x75, 0x74, 0x6f, 0x40, 0x70, 0x6c, 0x75, 0x74, 0x6f,
            0x2e, 0x63, 0x6f, 0x6d
        };
        unsigned int DN_len = 52;
        CFMutableDictionaryRef find_by_issuer = CFDictionaryCreateMutable(NULL, 0, NULL, NULL);
        CFDataRef issuer = SecCertificateGetNormalizedIssuerContent(cert);
        CFTypeRef found_by_issuer = NULL;
        CFDictionarySetValue(find_by_issuer, kSecAttrIssuer, issuer);
        CFDictionarySetValue(find_by_issuer, kSecClass, kSecClassIdentity);
        CFDictionarySetValue(find_by_issuer, kSecReturnRef, kCFBooleanTrue);
        ok_status(SecItemCopyMatching(find_by_issuer, &found_by_issuer), "find identity by cert issuer");
        ok(CFEqual(found_by_issuer, zomg_idnt), "should be same as zomg_idnt");
        CFReleaseNull(found_by_issuer);
        issuer = CFDataCreate(kCFAllocatorDefault, DN, DN_len);
        CFDictionarySetValue(find_by_issuer, kSecAttrIssuer, issuer);
        ok_status(SecItemCopyMatching(find_by_issuer, &found_by_issuer), "find identity by cert issuer");
        CFReleaseNull(issuer);
        ok(CFEqual(found_by_issuer, zomg_idnt), "should be same as zomg_idnt");
        CFReleaseNull(found_by_issuer);
        CFReleaseNull(find_by_issuer);
    }

    ok_status(SecItemDelete(lbl_idnt_query), "delete ident with zzzomg label");

    /* Delete the apple cert last */
    ok_status(SecItemDelete(appleCertDict), "delete apple ca certificate");
    CFReleaseNull(appleCertDict);
    CFReleaseNull(apple_ca_cert);

	CFRelease(zomg_key);
	CFRelease(zomg_cert);
	CFRelease(zomg_idnt);
	CFRelease(zomg_label);
	CFRelease(new_label_value);
	CFRelease(lbl_idnt_query);

    CFReleaseNull(result_idnt);
    CFReleaseNull(privKey);
    CFReleaseNull(cert);
}

int si_42_identity(int argc, char *const *argv)
{
    plan_tests(56);


    tests();

    return 0;
}
