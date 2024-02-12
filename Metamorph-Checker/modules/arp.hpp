
#include "module.h"

#define IP_ADDRESS_FORMAT XOR("%d.%d.%d.%d")
#define MAC_ADDRESS_FORMAT XOR("%02x-%02x-%02x-%02x-%02x-%02x")

DEFINE_MODULE(Arp,
	{
        ULONG nSize = 0;

        PMIB_IPNETTABLE pMib;

        GetIpNetTable(NULL, &nSize, TRUE);

        pMib = (PMIB_IPNETTABLE) new MIB_IPNETTABLE[nSize];

        DWORD dwRet = GetIpNetTable(pMib, &nSize, TRUE);

        for (int i = 0; i < pMib->dwNumEntries; i++)

        {
            auto entry = pMib->table[i];
            auto ip_addr = entry.dwAddr;
            auto mac_addr = entry.bPhysAddr;
            auto type = entry.dwType;

            char ip_addr_s[16];

            sprintf_s(ip_addr_s, IP_ADDRESS_FORMAT,
                (ip_addr & 0x0000ff),
                ((ip_addr & 0xff00) >> 8),
                ((ip_addr & 0xff0000) >> 16),
                (ip_addr >> 24)
            );

            char mac_addr_s[18];

            sprintf_s(mac_addr_s, MAC_ADDRESS_FORMAT,
                mac_addr[0], mac_addr[1],
                mac_addr[2], mac_addr[3],
                mac_addr[4], mac_addr[5]
            );

            const char* type_s = XOR("Unknown");

            if (type == 3) type_s = XOR("Dynamic");
            if (type == 4) type_s = XOR("Static");

            nlohmann::json arp_entry;

            arp_entry["ip"] = ip_addr_s;
            arp_entry["mac"] = mac_addr_s;
            arp_entry["type"] = type_s;

            data.push_back(arp_entry);
        }

        delete[] pMib;
	}
)