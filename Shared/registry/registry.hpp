#pragma once

#define SetRegValue(type, length, hkey, path, key, value, callback) \
{ \
    HKEY hKey; \
    if (RegOpenKeyEx(hkey, path, 0, KEY_WRITE, &hKey) == ERROR_SUCCESS) { \
        if (RegSetValueEx(hKey, key, 0, type, (LPBYTE)(value), length) == ERROR_SUCCESS) { \
            callback \
        } else { \
            Error(XOR("Error setting Value: %s %s"), path, key, GetLastError()); \
        } \
        RegCloseKey(hKey); \
    } else { \
        Error(XOR("Can't open key: %s (Code: %d)"), path, GetLastError()); \
    } \
}

#define GetRegValue(type_, alloc, val, del, hkey, path, key, callback) \
{ \
    HKEY hKey; \
    if (RegOpenKeyEx(hkey, path, 0, KEY_READ, &hKey) == ERROR_SUCCESS) { \
        type_ value; \
        DWORD type; \
        DWORD length = 0; \
        if (RegQueryValueEx(hKey, key, nullptr, &type, nullptr, &length) == ERROR_SUCCESS) { \
            value = alloc; \
            if (RegQueryValueEx(hKey, key, nullptr, &type, reinterpret_cast<BYTE*>(val), &length) == ERROR_SUCCESS) { \
                callback \
            } else { \
                Error(XOR("Error getting value: %s/%s (Code: %d)"), path, key, GetLastError()); \
                del; \
            } \
        } else { \
            Error(XOR("Can't open key: %s (Code: %d)"), path, GetLastError()); \
        } \
        RegCloseKey(hKey); \
    } \
}

#define SetRegString(hkey, path, key, value, callback) SetRegValue(REG_SZ, strlen(value) + 1, hkey, path, key, value, callback);
#define SetRegDWORD(hkey, path, key, value, callback) SetRegValue(REG_DWORD, sizeof(DWORD), hkey, path, key, value, callback);
#define SetRegQWORD(hkey, path, key, value, callback) SetRegValue(REG_QWORD, sizeof(DWORDLONG), hkey, path, key, value, callback);
#define SetRegBinary(hkey, path, key, value, length, callback)  SetRegValue(REG_BINARY, length, hkey, path, key, value, callback);

#define GetRegBinary(hkey, path, key, callback) GetRegValue(BYTE*, new BYTE[length], value, delete[] value, hkey, path, key, callback)
#define GetRegString(hkey, path, key, callback) GetRegValue(char*, new char[length], value, delete[] value, hkey, path, key, callback)
#define GetRegDWORD(hkey, path, key, callback) GetRegValue(DWORD, 0, &value, , hkey, path, key, callback)
#define GetRegQWORD(hkey, path, key, callback) GetRegValue(DWORDLONG, 0, &value, , hkey, path, key, callback)