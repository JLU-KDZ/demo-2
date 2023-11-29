#include "getip_kdz.h"

GetIP_KDZ::GetIP_KDZ()
{

}

void GetIP_KDZ::FreeIpForwardTable(PMIB_IPFORWARDTABLE pIpRouteTab)
{
    if (pIpRouteTab != NULL)
    {
        ::GlobalFree(pIpRouteTab);
        pIpRouteTab = NULL;
    }
}

PMIB_IPFORWARDTABLE GetIP_KDZ::GetIpForwardTable(WINBOOL bOrder)
{
    PMIB_IPFORWARDTABLE pIpRouteTab = NULL;
    DWORD dwActualSize = 0;
    //查询所需缓冲区的大小
    if (::GetIpForwardTable(pIpRouteTab, &dwActualSize, bOrder) == ERROR_INSUFFICIENT_BUFFER)
    {
        //为MIB_IPFORWARDTABLE结构申请内存
        pIpRouteTab = (PMIB_IPFORWARDTABLE)::GlobalAlloc(GPTR, dwActualSize);
        //获取路由表
        if (::GetIpForwardTable(pIpRouteTab, &dwActualSize, bOrder) == NO_ERROR)
            return pIpRouteTab;
        ::GlobalFree(pIpRouteTab);
    }
    return NULL;
}

string GetIP_KDZ::GetSystemIpAddress()
{
    string strLoalhostIp;
    PMIB_IPFORWARDTABLE pIpRouteTable = GetIpForwardTable(TRUE);
    if (pIpRouteTable != NULL)
    {
        //DWORD  dwCurrIndex;
        struct in_addr inadDest;
        struct in_addr inadMask;
        struct in_addr inadGateway;
        char szDestIp[128] = { 0 };
        char szMaskIp[128] = { 0 };
        char szGatewayIp[128] = { 0 };
        DWORD IfIndex = 0;
        DWORD ForwardMetric1 = 0;
        if (pIpRouteTable->dwNumEntries > 0)
        {
            int i = 0;
            for (i = 0; i < (int)pIpRouteTable->dwNumEntries; i++)
            {
                //dwCurrIndex = pIpRouteTable->table[i].dwForwardIfIndex;
                // 目的地址
                inadDest.s_addr = pIpRouteTable->table[i].dwForwardDest;
                strcpy_s(szDestIp, sizeof(szDestIp), inet_ntoa(inadDest));
                // 子网掩码
                inadMask.s_addr = pIpRouteTable->table[i].dwForwardMask;
                strcpy_s(szMaskIp, sizeof(szDestIp), inet_ntoa(inadMask));
                // 网关地址
                inadGateway.s_addr = pIpRouteTable->table[i].dwForwardNextHop;
                strcpy_s(szGatewayIp, sizeof(szDestIp), inet_ntoa(inadGateway));
                if ((strcmp(szDestIp, "0.0.0.0") == 0) && (strcmp(szMaskIp, "0.0.0.0") == 0))
                {
                    if (i == 0)
                    {
                        ForwardMetric1 = pIpRouteTable->table[i].dwForwardMetric1;
                        IfIndex = pIpRouteTable->table[i].dwForwardIfIndex;
                        //struct in_addr inadDest;
                        //inadDest.s_addr = pIpRouteTable->table[i].dwForwardDest;
                    }
                    else if (ForwardMetric1 > pIpRouteTable->table[i].dwForwardMetric1)
                    {
                        ForwardMetric1 = pIpRouteTable->table[i].dwForwardMetric1;
                        IfIndex = pIpRouteTable->table[i].dwForwardIfIndex;
                        //struct in_addr inadDest;
                        //inadDest.s_addr = pIpRouteTable->table[i].dwForwardDest;
                    }
                }
            }
        }
        else
        {
            FreeIpForwardTable(pIpRouteTable);
            return "";
        }
        FreeIpForwardTable(pIpRouteTable);

        if (IfIndex > 0)
        {
            //DWORD ipdwSize = 0;
            PBYTE m_pBuffer = new BYTE[MAX_PATH];
            ULONG m_ulSize = MAX_PATH;
            DWORD m_dwResult;
            PMIB_IPADDRTABLE pAddrTable;
            PMIB_IPADDRROW pAddrRow;
            in_addr ia;
            GetIpAddrTable((PMIB_IPADDRTABLE)m_pBuffer, &m_ulSize, TRUE);
            delete[] m_pBuffer;
            m_pBuffer = new BYTE[m_ulSize];
            if (NULL != m_pBuffer)
            {
                m_dwResult = GetIpAddrTable((PMIB_IPADDRTABLE)m_pBuffer, &m_ulSize, TRUE);
                if (m_dwResult == NO_ERROR)
                {
                    pAddrTable = (PMIB_IPADDRTABLE)m_pBuffer;

                    for (int x = 0; x <(int) pAddrTable->dwNumEntries; x++)
                    {
                        pAddrRow = (PMIB_IPADDRROW) & (pAddrTable->table[x]);
                        ia.S_un.S_addr = pAddrRow->dwAddr;
                        //char IPMsg[100] = { 0 };
                        if (IfIndex == pAddrRow->dwIndex)
                        {
                            LPCSTR psz = inet_ntoa(ia);
                            if (psz)
                            {
                                strLoalhostIp = psz;
                            }
                            delete[] m_pBuffer;
                            return strLoalhostIp;
                        }
                    }
                }
                delete[] m_pBuffer;
            }
        }
    }
    else
    {
        FreeIpForwardTable(pIpRouteTable);
        return "";
    }
    return "";
}
