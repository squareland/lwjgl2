/*
 * Copyright (c) 2002-2008 LWJGL Project
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * * Neither the name of 'LWJGL' nor the names of
 *   its contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "extal.h"
#include "common_tools.h"

/**
 * $Id$
 *
 * This file contains the AL extension assigning mechanism
 *
 * @author Brian Matzon <brian@matzon.dk>
 * @version $Revision$
 */
/* Handle to OpenAL Library */
static HMODULE handleOAL;

void *extal_NativeGetFunctionPointer(const char *function) {
	return GetProcAddress(handleOAL, function);
}

void extal_LoadLibrary(JNIEnv *env, jstring path) {
	const jchar *path_str = (*env)->GetStringChars(env, path, NULL);
	jsize jlen = (*env)->GetStringLength(env, path);
//	printfDebugJava(env, "Testing '%s'", path_str);
    char *encoded = NULL;
    int len = WideCharToMultiByte(GetACP(), 0, (LPCWSTR)path_str, jlen, NULL, 0, 0, 0);
    if (len > 0)
    {
        encoded = (char*) malloc(len + 1);
        WideCharToMultiByte(GetACP(), 0, (LPCWSTR)path_str, jlen, encoded, len, 0, 0);
        encoded[len] = 0;
	    handleOAL = LoadLibrary(encoded);
    } else {
	    handleOAL = LoadLibraryW(path_str);
    }
	if (handleOAL != NULL) {
		printfDebugJava(env, "Found OpenAL at '%s'", path_str);
	} else {
		throwFormattedException(env, "Could not load OpenAL library (%d)", GetLastError());
	}
	(*env)->ReleaseStringChars(env, path, path_str);
}

/**
 * Unloads the OpenAL Library
 */
void extal_UnloadLibrary() {
	FreeLibrary(handleOAL);
}
