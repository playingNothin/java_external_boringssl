/*
 * Copyright (C) 2019 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <openssl/crypto.h>

#include <stdio.h>

// Moto G20 (java) 2026-09-24: report the outcome to stderr (stdio_to_kmsg
// forwards it to the kernel log) instead of failing silently. On non-FIPS
// platform builds FIPS_mode() is always 0 — the power-on self-test this
// binary validates lives in libcrypto's FIPS-only constructor and never
// runs — so the failure here is the build mode, not broken crypto.
int main(int argc, char** argv) {
    int fips = FIPS_mode();
    fprintf(stderr, "boringssl_self_test: %s: FIPS_mode()=%d -> %s\n",
            argc > 0 ? argv[0] : "boringssl_self_test", fips,
            fips ? "pass" : "fail (non-FIPS platform build)");
    // If we get here, then libcrypto is either in FIPS mode (in which case
    // it doesn't run the self test), or the self test has passed. If the
    // self test ran and failed, then libcrypto will already have abort()ed.
    if (!fips) {
        return 1;  // failure
    }
    return 0;  // success
}
