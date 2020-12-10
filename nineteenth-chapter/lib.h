//
// Created by 王勇椿 on 2020/12/9.
//

#ifndef UNIX_NETWORK_PROGRAMING_LIN_H
#define UNIX_NETWORK_PROGRAMING_LIN_H

#include "../lib/unp.h"
#include <net/pfkeyv2.h>

struct idlist {
    int val;
    const char *name;
};

static struct idlist satype[] = {
        { SADB_SATYPE_UNSPEC,	"unspec" },
        { SADB_SATYPE_AH,		"ah" },
        { SADB_SATYPE_ESP,		"esp" },
        { SADB_SATYPE_RSVP,		"rsvp" },
        { SADB_SATYPE_OSPFV2,	"ospfv2" },
        { SADB_SATYPE_RIPV2,	"ripv2" },
        { SADB_SATYPE_MIP,		"mip" },
        { 0,					NULL } };

static struct idlist ahalg[] = {
        { SADB_AALG_NONE,		"none" },
        { SADB_AALG_MD5HMAC,	"HMAC-MD5-96" },
        { SADB_AALG_SHA1HMAC,	"HMAC-SHA-1-96" },
        { 0,					NULL } };

static struct idlist espalg[] = {
        { SADB_EALG_NONE,		"none" },
        { SADB_EALG_DESCBC,		"DES-CBC" },
        { SADB_EALG_3DESCBC,	"3DES-CBC" },
        { SADB_EALG_NULL,		"NULL" },
        { 0,					NULL } };


const char *get_sadb_msg_type(int type);
const char *get_sadb_satype(int type);
const char *get_auth_alg(int alg);
const char *get_encrypt_alg(int alg);
const char *get_sa_state(int state);
const char *get_sadb_alg_type(int alg, int authenc);
void sa_print(struct sadb_ext *ext);
void supported_print(struct sadb_ext *ext);
void lifetime_print(struct sadb_ext *ext);
void address_print(struct sadb_ext *ext);
void key_print(struct sadb_ext *ext);
void print_sadb_msg(struct sadb_msg *msg, int msglen);
int idlistlookup(char *name, struct idlist *il);
int getsatypebyname(char *name);
int getsaalgbyname(int type, char *name);
void sadb_add(struct sockaddr *, struct sockaddr *, int, int, int, int, unsigned char *);
int salen(struct sockaddr *);
int prefix_all(struct sockaddr *);

#endif //UNIX_NETWORK_PROGRAMING_LIN_H
