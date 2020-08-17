#ifndef UNICODE
#define UNICODE
#endif
#pragma comment(lib, "netapi32.lib")

#include <napi.h>
#include <stdio.h>
#include <windows.h>
#include <lm.h>


DWORD dwlevel = 0;
DWORD dwfilter = 0;
LPUSER_INFO_0 pBuf = NULL;
LPUSER_INFO_0 pTmpBuf; 
DWORD dwprefmaxlen = MAX_PREFERRED_LENGTH;
DWORD dwentriesread;
DWORD dwtotalentries;


Napi::Boolean Method(const Napi::CallbackInfo& info) {

    Napi::Env env = info.Env();

    if (info.Length() != 1) {
        Napi::Error::New(env, "Wrong amount of aruments").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }
    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "Wrong type of argument").ThrowAsJavaScriptException();
        return Napi::Boolean::New(env, false);
    }

    std::u16string js_string = info[0].ToString().Utf16Value();

    NetUserEnum(NULL, dwlevel, dwfilter, (LPBYTE*)&pBuf, dwprefmaxlen, &dwentriesread,
        &dwtotalentries, NULL);

    if ((pTmpBuf = pBuf) != NULL) {

        for (int i = 0; (i < dwentriesread); i++)
        {
            std::wstring fromLPWSTR = pTmpBuf->usri0_name;
            std::u16string u16account_name(fromLPWSTR.begin(), fromLPWSTR.end());
            
            if (u16account_name == js_string)
            {
                if (pBuf != NULL)
                {
                    NetApiBufferFree(pBuf);
                    pBuf = NULL;
                }

                return Napi::Boolean::New(env, true);
            }

            pTmpBuf++;
        }
    }
   
    if (pBuf != NULL)
    {
        NetApiBufferFree(pBuf);
        pBuf = NULL;
    }

    return Napi::Boolean::New(env, false);
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
    exports.Set(Napi::String::New(env, "IsRegisteredAsUser"),
        Napi::Function::New(env, Method));
    return exports;
}

NODE_API_MODULE(addon, Init)