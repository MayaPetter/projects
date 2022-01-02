/******************************************
* Author:   Maya Petter                   *
* Reviewer:                               *
* Purpose:                                *
*******************************************/
#include <limits.h> /* CHAR_BIT */
#include <assert.h> /* assert   */
#include <stdlib.h> /* malloc, free */
#include <string.h> /* memset */

#include "dhcp.h"
#include "trie.h"

#define MAX_RESERVED 3
#define MAX_ASCII 255

/*********** Structs Typdef and global array **********************************/
typedef unsigned char u_char;

struct dhcp
{
    u_char net_id[BYTES_IN_IP];
    size_t nbits_in_host;
    trie_t *trie;
};

enum reserved_addr {SERVER, ROUTER, BROADCAST};

u_char RESERVED[MAX_RESERVED][BYTES_IN_IP];

/*********** Static Helper Functions ******************************************/
static void *FreeFunction(void *to_free);
static void CreateAddresses(dhcp_t *dhcp, u_char *dest, u_char num);

/****** initiation ************/
static int DHCPInitManegment(dhcp_t *dhcp, u_char net_id[BYTES_IN_IP], 
                              size_t num_bits_net_id);
static int LeaseReservedAddresses(dhcp_t *dhcp);                              
static void CreateReservedAddresses(dhcp_t *dhcp, u_char *server, 
                                    u_char *router, u_char *broadcast);

/****** find next free IP address ************/                                    
static int AddOneToIP(u_char *req_ip, size_t index);
static u_char *FindNextFreeIP(dhcp_t *dhcp, u_char *req_ip, size_t host_bytes);
static void GetFirstFreeIP(dhcp_t *dhcp, u_char *first_free_ip, size_t index, 
                                                             size_t host_bytes);

/****** compare IP address ************/
static int IsSameIp(u_char *req_ip, u_char *to_compare, size_t index, 
                                                        size_t bytes);
int IsSameNetId(dhcp_t *dhcp, const u_char req_ip[BYTES_IN_IP]);
int IsReservedId(dhcp_t *dhcp, const u_char ip_to_release[BYTES_IN_IP]);

/*********** API Functions ****************************************************/

/*********** Create a new DHCP server *****************************************/
dhcp_t *DHCPCreate(u_char net_id[BYTES_IN_IP], size_t num_bits_net_id)
{
    dhcp_t *new_dhcp = NULL;
    int status = 0;
    
    assert(NULL != net_id);
    assert(BYTES_IN_IP > (num_bits_net_id / CHAR_BIT));    
    
    new_dhcp = (dhcp_t *)malloc(sizeof(struct dhcp)); 
    if (NULL == new_dhcp)
    {
        return NULL;
    }
    
    status = DHCPInitManegment(new_dhcp, net_id, num_bits_net_id);
    if (FAIL == status)
    {
        new_dhcp = FreeFunction(new_dhcp);
        return NULL;
    }
    
    status = LeaseReservedAddresses(new_dhcp);
    if (FAIL == status)
    {
        DHCPDestroy(new_dhcp);
        return NULL;
    }
        
    return new_dhcp;
}

/*********** Destroy given DHCP server ****************************************/
void DHCPDestroy(dhcp_t *dhcp)
{
    assert(NULL != dhcp);
    
    TrieDestroy(dhcp->trie);
    
    dhcp = FreeFunction(dhcp);
}

/*********** Allocate IP address in DHCP server *******************************/

dhcp_status_t DHCPLease(dhcp_t *dhcp, const u_char req_ip[BYTES_IN_IP],
						u_char alloc_ip[BYTES_IN_IP])
{
    int status = FAIL;
    u_char *found_ip = NULL;
    size_t host_bytes = 0;    
    size_t net_bytes = 0; 
            
    assert(NULL != dhcp);
    assert(NULL != req_ip);
    assert(NULL != alloc_ip);
    
    if (0 == DHCPCountFree(dhcp))
    {
        return FAIL;
    }
    
    if (!IsSameNetId(dhcp, req_ip))
    {
        return INVALID_REQUEST;
    }
    
    memcpy(alloc_ip, req_ip, BYTES_IN_IP);
    
    host_bytes = dhcp->nbits_in_host / CHAR_BIT;
    net_bytes = BYTES_IN_IP - host_bytes;
    
    if (!TrieFind(dhcp->trie, (alloc_ip + net_bytes)))
    {
        status = TrieInsert(dhcp->trie, (alloc_ip + net_bytes));
        if (FAIL == status)
        {
            return FAIL;
        }
        
        return SUCCESS;
    }
   
    found_ip = FindNextFreeIP(dhcp, alloc_ip, host_bytes); 
    
    status = TrieInsert(dhcp->trie, found_ip);
    if (FAIL == status)
    {
        return FAIL;
    }
        
    return SUCCESS;
}
/*********** Release previously allocated IP address in DHCP server ***********/
dhcp_status_t DHCPRelease(dhcp_t *dhcp, 
						  const u_char ip_to_release[BYTES_IN_IP])
{
    size_t host_index = 0;
	
	assert(NULL != dhcp);
	assert(NULL != ip_to_release);
	
	host_index = BYTES_IN_IP - (dhcp->nbits_in_host / CHAR_BIT);
	
    if (!IsSameNetId(dhcp, ip_to_release))
    {
        return INVALID_REQUEST;
    }
    
    if (IsReservedId(dhcp, ip_to_release))
    {
        return INVALID_REQUEST; 
    }
    
    if (!TrieFind(dhcp->trie, (u_char *)ip_to_release + host_index))
    {
    	return INVALID_REQUEST;
    }
    
    TrieRemove(dhcp->trie, (u_char *)ip_to_release + host_index);

    return SUCCESS;
}

/*********** Gets number of available addresses to lease **********************/
size_t DHCPCountFree(const dhcp_t *dhcp)
{
    assert(NULL != dhcp);    
    
    return (TrieCapacity(dhcp->trie) - TrieCount(dhcp->trie));
}

/*********** Static Helper Functions ******************************************/

/*********** return the next free IP ******************************************/
static u_char *FindNextFreeIP(dhcp_t *dhcp, u_char *req_ip, size_t host_bytes)
{
    u_char last_ip[BYTES_IN_IP] = "";
    u_char first_free_ip[BYTES_IN_IP] = "";
    size_t index = BYTES_IN_IP - 1;
    
    assert(NULL != dhcp);
    assert(NULL != req_ip);
    
    CreateAddresses(dhcp, last_ip, MAX_ASCII);
    GetFirstFreeIP(dhcp, first_free_ip, index, host_bytes);
    
    if (0 == TrieFind(dhcp->trie, req_ip))
    {   
        return req_ip;  
    }

    if (IsSameIp(req_ip, last_ip, index, host_bytes))
    {   
        return memcpy((req_ip + index), (first_free_ip + index), host_bytes);
    }

    
    if(FAIL == AddOneToIP(req_ip, index))
    {
        return NULL;
    }
    
    return FindNextFreeIP(dhcp, req_ip, host_bytes); 
}

/*********** return the first free IP *****************************************/
static void GetFirstFreeIP(dhcp_t *dhcp, u_char *first_free_ip, size_t index, 
                                                              size_t host_bytes)
{
    u_char first_ip[BYTES_IN_IP] = "";
    
    assert(NULL != dhcp);
    assert(NULL != first_free_ip);
    
    CreateAddresses(dhcp, first_ip, 0);
    
    if (!TrieFind(dhcp->trie, first_ip))
    {   
        memcpy((first_free_ip + index), (first_ip + index), host_bytes);
        return;
    }
    
    AddOneToIP(first_ip, index);
        
    GetFirstFreeIP(dhcp, first_ip, index, host_bytes);
}

/****************** compare IP address ****************************************/
static int IsSameIp(u_char *req_ip, u_char *to_compare, size_t index, 
                                                        size_t bytes)
{
    assert(NULL != req_ip);
    assert(NULL != to_compare);
    
    return (0 == (memcmp((req_ip + index), (to_compare + index), bytes)));
}

int IsSameNetId(dhcp_t *dhcp, const u_char req_ip[BYTES_IN_IP])
{
    size_t net_id_bytes = 0;
    
    assert(NULL != dhcp);
    assert(NULL != req_ip);
    
    net_id_bytes = BYTES_IN_IP - (dhcp->nbits_in_host / CHAR_BIT);

    return (0 == memcmp(dhcp->net_id, req_ip, net_id_bytes));
}

static int AddOneToIP(u_char *req_ip, size_t index)
{
    assert(NULL != req_ip);
    
    if (MAX_ASCII == req_ip[index])
    {
        req_ip[index] = 0;
        
        if (0 == index)
        {
            return FAIL;
        }
        
        ++req_ip[index - 1];
    }
    else
    {
        ++req_ip[index];
    }
    
    return SUCCESS;
}

int IsReservedId(dhcp_t *dhcp, const u_char ip_to_release[BYTES_IN_IP])
{
    size_t i = 0;
    
    assert(NULL!= dhcp);
    assert(NULL!= ip_to_release);
    
    for (i = 0; i < MAX_RESERVED; ++i)
    {
        if (0 == memcmp(RESERVED[i], ip_to_release, BYTES_IN_IP))
        {
            return 1;
        }
    }
    
    return 0;
}

/****** initiation ************/
static int DHCPInitManegment(dhcp_t *dhcp, u_char net_id[BYTES_IN_IP], 
                              size_t num_bits_net_id)
{
    assert(NULL != net_id);
    assert(NULL != dhcp);
    
    memcpy(dhcp->net_id, net_id, (num_bits_net_id / CHAR_BIT));
    dhcp->nbits_in_host = (BYTES_IN_IP * CHAR_BIT) - num_bits_net_id;
    
    dhcp->trie = TrieCreate(dhcp->nbits_in_host); 
    if (NULL == dhcp->trie)
    {
        return FAIL;
    }
    
    return SUCCESS;
}

static void *FreeFunction(void *to_free)
{
    assert(NULL != to_free);
    
    free(to_free);
    to_free = NULL;
    
    return NULL;
}

/*********** Create and lease reserved addresses ******************************/
static int LeaseReservedAddresses(dhcp_t *dhcp)
{
    u_char server[BYTES_IN_IP] = "";
    u_char router[BYTES_IN_IP] = "";
    u_char broadcast[BYTES_IN_IP] = "";
    
    assert(NULL != dhcp);
    
    CreateReservedAddresses(dhcp, server, router, broadcast);

    if (FAIL == DHCPLease(dhcp, server, RESERVED[SERVER]))
    {
        return FAIL;
    }
    
    if (FAIL == DHCPLease(dhcp, router, RESERVED[ROUTER]))
    {
        return FAIL;
    }
     
    if (FAIL == DHCPLease(dhcp, broadcast, RESERVED[BROADCAST])) 
    {
        return FAIL;
    }                                

    return SUCCESS;
}

static void CreateReservedAddresses(dhcp_t *dhcp, u_char *server, 
                                    u_char *router, u_char *broadcast)
{
    assert(NULL != dhcp);
    assert(NULL != server);
    assert(NULL != router);
    assert(NULL != broadcast);
    
    CreateAddresses(dhcp, server, 0);
    CreateAddresses(dhcp, router, MAX_ASCII);
    CreateAddresses(dhcp, broadcast, MAX_ASCII);
    
    router[BYTES_IN_IP - 1] = 254;
}

static void CreateAddresses(dhcp_t *dhcp, u_char *dest, u_char num)
{
    size_t host_bytes = 0;
    size_t host_index = 0;
    
    assert(NULL != dhcp);
    assert(NULL != dest);
    
    host_bytes = (dhcp->nbits_in_host) / CHAR_BIT;
    host_index = BYTES_IN_IP - host_bytes;
    
    memcpy(dest, dhcp->net_id, host_index);
    memset((dest + host_index), num, host_bytes);
}
