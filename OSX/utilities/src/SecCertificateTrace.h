/*
 * Copyright (c) 2013-2014 Apple Inc. All Rights Reserved.
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

/*
 * SecCertificateTrace.h - log statistics for Apple certificate usage
 */

#ifndef _UTILITIES_SECCERTIFICATETRACE_H_
#define _UTILITIES_SECCERTIFICATETRACE_H_

#include <CoreFoundation/CoreFoundation.h>

__BEGIN_DECLS

/* top-level prefix string */
extern const CFStringRef kCertStatsPrefix;

/* certificate key */
extern const CFStringRef kCertStatsCert;

/* keys for certificate field strings */
extern const CFStringRef kCertStatsPolicy;
extern const CFStringRef kCertStatsNotBefore;
extern const CFStringRef kCertStatsNotAfter;
extern const CFStringRef kCertStatsSerialNumber;
extern const CFStringRef kCertStatsSubjectSummary;
extern const CFStringRef kCertStatsIssuerSummary;

bool SecCertificateTraceAddRecord(CFDictionaryRef certRecord);

__END_DECLS

#endif /* _UTILITIES_SECCERTIFICATETRACE_H_ */
