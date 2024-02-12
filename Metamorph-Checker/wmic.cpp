#include "wmic.h"

namespace WMIC {
    IWbemLocator* pLoc = nullptr;
    IWbemServices* pSvc = nullptr;
}

void WMIC::Connect() {
    HRESULT hres;

    hres = CoInitializeEx(0, COINIT_MULTITHREADED);
    if (FAILED(hres)) {
        Error(XOR("Failed to initialize COM library. Error code: %d"), hres);
        return;
    }

    hres = CoInitializeSecurity(
        NULL,
        -1,
        NULL,
        NULL,
        RPC_C_AUTHN_LEVEL_DEFAULT,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE,
        NULL
    );

    if (FAILED(hres)) {
        Error(XOR("Failed to initialize security. Error code: %d"), hres);
        CoUninitialize();
        return;
    }

    pLoc = NULL;
    hres = CoCreateInstance(
        CLSID_WbemLocator,
        0,
        CLSCTX_INPROC_SERVER,
        IID_IWbemLocator,
        (LPVOID*)&pLoc
    );

    if (FAILED(hres)) {
        Error(XOR("Failed to create IWbemLocator object. Error code: %d"), hres);
        CoUninitialize();
        return;
    }

    pSvc = NULL;
    hres = pLoc->ConnectServer(
        _bstr_t(L"ROOT\\CIMV2"),
        NULL,
        NULL,
        0,
        NULL,
        0,
        0,
        &pSvc
    );

    if (FAILED(hres)) {
        Error(XOR("Failed to connect to WMI. Error code: %d"), hres);
        pLoc->Release();
        CoUninitialize();
        return;
    }

    hres = CoSetProxyBlanket(
        pSvc,
        RPC_C_AUTHN_WINNT,
        RPC_C_AUTHZ_NONE,
        NULL,
        RPC_C_AUTHN_LEVEL_CALL,
        RPC_C_IMP_LEVEL_IMPERSONATE,
        NULL,
        EOAC_NONE
    );

    if (FAILED(hres)) {
        Error(XOR("Failed to set proxy blanket. Error code: %d"), hres);
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return;
    }
};

void WMIC::DoRequest(const char* req, const char* key, const std::function<void(const char* value)>& callback) {
    HRESULT hres;

    IEnumWbemClassObject* pEnumerator = NULL;
    hres = pSvc->ExecQuery(
        bstr_t("WQL"),
        bstr_t(req),
        WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
        NULL,
        &pEnumerator
    );

    if (FAILED(hres)) {
        Error(XOR("Query failed. Error code: %d"), hres);
        pSvc->Release();
        pLoc->Release();
        CoUninitialize();
        return;
    }

    IWbemClassObject* pclsObj;
    ULONG uReturn = 0;

    while (pEnumerator) {
        hres = pEnumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);
        if (uReturn == 0) {
            break;
        }

        VARIANT vtProp;
        hres = pclsObj->Get(bstr_t(key), 0, &vtProp, 0, 0);

        if (SUCCEEDED(hres)) {

            int length = WideCharToMultiByte(CP_ACP, 0, vtProp.bstrVal, -1, NULL, 0, NULL, NULL);
            char* charStr = new char[length];

            WideCharToMultiByte(CP_ACP, 0, vtProp.bstrVal, -1, charStr, length, NULL, NULL);

            callback(charStr);

            delete[] charStr;

            VariantClear(&vtProp);
        }

        pclsObj->Release();
    }

    pEnumerator->Release();
}

void WMIC::Disconnect() {
    pSvc->Release();
    pLoc->Release();
    CoUninitialize();
};