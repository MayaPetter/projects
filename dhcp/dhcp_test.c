#include <stdio.h> /* printf */
#include <math.h>  /* pow */
#include <limits.h> /*CHAR_BIT */

#include "dhcp.h"
#include "trie.h"

#define Expect(x) \
;{\
	if (!(x))\
	{\
		printf("test failed in line %d\n", __LINE__);\
		return 0;\
	}\
} /* end of Expect */

static void PrintIP(const unsigned char ip_to_print[BYTES_IN_IP]);
static int CreateDestroyTest();
static int DHCPLeaseTest();
static int DHCPReleaseTest();

int main()
{
    CreateDestroyTest();
    DHCPLeaseTest();
    DHCPReleaseTest();    
    
    return 0;
}

static int CreateDestroyTest()
{
    unsigned char net_id[BYTES_IN_IP] = {196, 10, 0, 0};
    size_t num_bits_net_id = 16;
    size_t host_bits = (CHAR_BIT * BYTES_IN_IP) - num_bits_net_id;
    size_t capacity = pow(2, host_bits);
    dhcp_t *dhcp = NULL;

    dhcp = DHCPCreate(net_id, num_bits_net_id);
    
    Expect(NULL != dhcp)
    Expect((size_t)(capacity - 3) == DHCPCountFree(dhcp))
    DHCPDestroy(dhcp);

    return 1;
}

static int DHCPLeaseTest()
{
    unsigned char net_id[BYTES_IN_IP] = {196, 10, 0, 0};
    size_t num_bits_net_id = 16;
  
    unsigned char alloc_ip[BYTES_IN_IP] = "";
    unsigned char valid_req_ip[] = {196, 10, 0, 6};
    unsigned char invalid_req_ip[] = {120, 10, 0, 6};
    unsigned char unavailable_req_ip[] = {196, 10, 255, 254};
    
    dhcp_t *dhcp = NULL;

    dhcp = DHCPCreate(net_id, num_bits_net_id);
    
    Expect(NULL != dhcp)
            
    Expect(SUCCESS == DHCPLease(dhcp, valid_req_ip, alloc_ip))
    PrintIP(alloc_ip);

    Expect(INVALID_REQUEST == DHCPLease(dhcp, invalid_req_ip, alloc_ip))
    PrintIP(alloc_ip);
    
    Expect(SUCCESS == DHCPLease(dhcp, unavailable_req_ip, alloc_ip))
    PrintIP(alloc_ip);

    DHCPDestroy(dhcp);

    return 1;

}

static void PrintIP(const unsigned char ip_to_print[BYTES_IN_IP])
{
    size_t i = 0;
    
    for (i = 0; i < BYTES_IN_IP ; ++i)
    {
        printf("%d. ", ip_to_print[i]);
    }
    puts("\n");
}

static int DHCPReleaseTest()
{
    unsigned char net_id[BYTES_IN_IP] = {196, 10, 0, 0};
    size_t num_bits_net_id = 16;
  
    unsigned char alloc_ip[BYTES_IN_IP] = "";
    unsigned char valid_req_ip[] = {196, 10, 0, 6};
    unsigned char invalid_req_ip[] = {120, 10, 0, 6};
    unsigned char unavailable_req_ip[] = {196, 10, 255, 254};
    
    dhcp_t *dhcp = NULL;

    dhcp = DHCPCreate(net_id, num_bits_net_id);
    
    Expect(NULL != dhcp)
            
    Expect(SUCCESS == DHCPLease(dhcp, valid_req_ip, alloc_ip))
    Expect(SUCCESS == DHCPRelease(dhcp, valid_req_ip))

    Expect(INVALID_REQUEST == DHCPRelease(dhcp, invalid_req_ip))
    
    Expect(INVALID_REQUEST == DHCPRelease(dhcp, unavailable_req_ip))

    DHCPDestroy(dhcp);

    return 1;

}
