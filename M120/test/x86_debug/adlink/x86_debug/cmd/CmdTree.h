/*
 * CmdTree.h
 */

#ifndef _CMD_TREE_H
#define _CMD_TREE_H

#ifdef __cplusplus
extern "C" {
#endif

#define TOKEN        0x1
#define T_LEAF       0x2
#define VALUE        0x4
#define V_LEAF       0x8
#define D_TOKEN      0x10
#define D_VALUE      0x20
#define DT_LEAF      0x40

#define CMD_STR      0x0
#define CMD_INT      0x1

#define END_TREE     0x00ABCDEF

#define GENE_MODE1   0x1  /* > */
#define PRIV_MODE1   0x2  /* # */
#define CONF_MODE1   0x4  /* # */
#define DBUG_MODE1   0x8  /* # */
#define GROP_MODE1   0x80  /* # */

#define NONE_MODE2   0x1   /* NULL */
#define IF_MODE2     0x2   /* interface */
#define RIP_MODE2    0x4   /* rip */
#define OSPF_MODE2   0x8   /* ospf */

#define NONE_MODE3   0x1   /* NULL */
#define ETHE_MODE3   0x2   /* ethernet */
#define GIGA_MODE3   0x4   /* giga-ethe */
#define VLAN_MODE3   0x8   /* vlan */
#define MANA_MODE3   0x10  /* vlan */

#define MASK_VALUE1  0xFF000000
#define MASK_VALUE2  0x00FFFF00
#define MASK_VALUE3  0x000000FF

#define GLOB_MODE    0xFFFFFFFF

#define INIT_MODE    ((GENE_MODE1) << 24)
#define PRIV_MODE    ((PRIV_MODE1) << 24)
#define DBUG_MODE    ((DBUG_MODE1) << 24)
#define GROP_MODE    ((GROP_MODE1) << 24)

#define CNNN_MODE    (((CONF_MODE1) << 24) | (NONE_MODE2 << 8) | NONE_MODE3)
#define CINN_MODE    (((CONF_MODE1) << 24) | (IF_MODE2 << 8) | NONE_MODE3)
#define CIEP_MODE    (((CONF_MODE1) << 24) | (IF_MODE2 << 8) | ETHE_MODE3)
#define CIGP_MODE    (((CONF_MODE1) << 24) | (IF_MODE2 << 8) | GIGA_MODE3)
#define CIVP_MODE    (((CONF_MODE1) << 24) | (IF_MODE2 << 8) | VLAN_MODE3)
#define CIE0_MODE    (((CONF_MODE1) << 24) | (IF_MODE2 << 8) | MANA_MODE3)
#define CRNN_MODE    (((CONF_MODE1) << 24) | (RIP_MODE2 << 8) | NONE_MODE3)
#define CIEG_MODE    (CIGP_MODE | CIEP_MODE)

typedef struct cmd_node
{
    struct cmd_node *pstNext;
    struct cmd_node *pstPrev;
} CMD_Node;

typedef struct list_stru
{
    CMD_Node stNode;
    ULONG ulCount;
} CMD_List;

typedef struct key
{
    VOID *pValue;
} KEY_STRU;

#define NODE_HEAD        stNode.pstNext

#define LIST_NEXT(pNode) ((pNode)->pstNext)

#define LIST_PREV(pNode) ((pNode)->pstPrev)

#define NODE_INIT(pNode) {(pNode)->pstNext = NULL;   (pNode)->pstPrev = NULL;}

typedef void (*CMD_FuncPtr)(char *, int, CHAR **, ULONG);

typedef struct cmd_value
{
    UINT16 usType;
    UINT16 usResever;
    ULONG ulDLimit;
    ULONG ulSLimit;
} CMD_Value;

typedef struct cmd_command
{
    CMD_Node stNode;
    USHORT usIType;
    USHORT usPType;
    USHORT usCType;
    USHORT usEType;
    USHORT usGType;
    USHORT usVType;
    USHORT usOther;
    USHORT usGroup;
    ULONG TotleType;
    ULONG ulLevel;
    ULONG ulMode;
    CHAR *pcName;
    CHAR *pcTip;
    CHAR *pcDomain;
    CMD_Value stVarInfo;
    void (*pFunc)(char *, int, CHAR **, ULONG);
    CMD_List stList;
} CMD_CmdInfo;

typedef struct cmd_record
{
    ULONG ulType;
    ULONG ulLevel;
    ULONG ulMode;
    UINT16 usVarType;
    UINT16 usReserve;
    CHAR *pcVarLimit;
    CHAR *pcName;
    CHAR *pcTip;
    CHAR *pcDomain;
    void (*pFunc)();
} CMD_TreeRecd;

#define END_RECORD {0, UPOWER, 0, NULL_WORD, NULL_WORD, NULL, NULL, NULL, NULL, NULL}

#define CMD_TOKEN(LEVEL, MODE, NAME, TIP) {TOKEN, LEVEL, MODE, NULL_WORD, NULL_WORD, NULL, NAME, TIP, NULL, (VOID *)NULL}

#define CMD_TLEAF(LEVEL, MODE, NAME, FUNC, DOMAIN, TIP) {T_LEAF, LEVEL, MODE, NULL_WORD, NULL_WORD, NULL, NAME, TIP, DOMAIN, (VOID *)FUNC}

#define CMD_TLEAFG(LEVEL, MODE, NAME, FUNC, DOMAIN, TIP) {T_LEAF, LEVEL, GROP_MODE | MODE, NULL_WORD, NULL_WORD, NULL, NAME, TIP, DOMAIN, (VOID *)FUNC}

#define CMD_VALUE(TYPE, LIMIT, NAME, TIP) {VALUE, UPOWER, NULL_LONG, TYPE, NULL_WORD, LIMIT, NAME, TIP, NULL, (VOID *)NULL}

#define CMD_VLEAF(TYPE, LIMIT, NAME, FUNC, DOMAIN, TIP) {V_LEAF, UPOWER, NULL_LONG, TYPE, NULL_WORD, LIMIT, NAME, TIP, DOMAIN, (VOID *)FUNC}

#define CMD_VLEAFG(TYPE, LIMIT, NAME, FUNC, DOMAIN, TIP) {V_LEAF, UPOWER, GROP_MODE, TYPE, NULL_WORD, LIMIT, NAME, TIP, DOMAIN, (VOID *)FUNC}

#define CMD_TDEF(LEVEL, NAME, TIP) {D_TOKEN, LEVEL, NULL_LONG, NULL_WORD, NULL_WORD, NULL, NAME, TIP, NULL, (VOID *)NULL}

#define CMD_VDEF(TYPE, LIMIT, NAME, TIP) {D_VALUE, UPOWER, NULL_LONG, TYPE, NULL_WORD, LIMIT, NAME, TIP, NULL, (VOID *)NULL}

#define CMD_TLDEF(LEVEL, MODE, NAME, FUNC, DOMAIN, TIP) {DT_LEAF, LEVEL, MODE, NULL_WORD, NULL_WORD, NULL, NAME, TIP, DOMAIN, (VOID *)FUNC}

extern CMD_CmdInfo CmdRoot;

extern ULONG CMD_TreeInit(VOID);
extern ULONG CMD_BuildTree(CMD_TreeRecd *m_pastCmdTable);

extern VOID  CMD_SetUserMode(ULONG ulSession, ULONG ulMode);
extern VOID  CMD_InitUserMode(CMD_Ctrl *CmdCtrl);
extern VOID  CMD_DisplayPrompt(ULONG ulSession);
extern ULONG CMD_IsCommandCanUse(ULONG ulSession, ULONG ulMode, ULONG ulLevel);
extern ULONG CMD_IsCommandCanDisplay(ULONG ulSession, ULONG ulMode, ULONG ulLevel);

extern CMD_Node *LIST_First(CMD_List *pList);

extern ULONG CMD_GetNodeType(ULONG ulSes, CMD_CmdInfo *pstCmd);
extern ULONG CMD_GetGroupType(ULONG ulSes, CMD_CmdInfo *pstCmd);
extern ULONG CMD_GetUserPWord(ULONG ulSes, CHAR *pcBuf, ULONG ulLen);
extern ULONG CMD_GetSuperUserPWord(ULONG ulSes, CHAR *pcBuf, ULONG ulLen);

#ifdef __cplusplus
}
#endif

#endif

