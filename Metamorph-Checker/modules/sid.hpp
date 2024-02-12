

#include "module.h"

DEFINE_MODULE(UserSID,
    {
        HANDLE token;
        if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &token)) {
            Error(XOR("OpenProcessToken failed. Error code: %d"), GetLastError());
            return STATUS_UNSUCCESSFUL;
        }

        DWORD tokenInfoSize = 0;
        if (!GetTokenInformation(token, TokenUser, NULL, 0, &tokenInfoSize) && GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
            Error(XOR("GetTokenInformation failed. Error code: %d"), GetLastError());
            CloseHandle(token);
            return STATUS_UNSUCCESSFUL;
        }

        PTOKEN_USER tokenUser = (PTOKEN_USER)malloc(tokenInfoSize);
        if (!tokenUser) {
            Error(XOR("Memory allocation failed. Error code: %d"), GetLastError());
            CloseHandle(token);
            return STATUS_UNSUCCESSFUL;
        }

        if (!GetTokenInformation(token, TokenUser, tokenUser, tokenInfoSize, &tokenInfoSize)) {
            Error(XOR("GetTokenInformation failed. Error code: %d"), GetLastError());
            CloseHandle(token);
            free(tokenUser);
            return STATUS_UNSUCCESSFUL;
        }

        CloseHandle(token);

        if (tokenUser) {
            LPSTR sidString = NULL;
            if (ConvertSidToStringSidA(tokenUser->User.Sid, &sidString)) {

                data = sidString;

                LocalFree(sidString);
            }
            else {
                Error(XOR("ConvertSidToStringSidA failed. Error code: "), GetLastError());

                return STATUS_UNSUCCESSFUL;
            }

            free(tokenUser);
        }
    }
)