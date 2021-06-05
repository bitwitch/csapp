word f_pc = [
    M_icode == IJXX && M_Cnd : M_valA;
    W_icode == IRET : W_valM;
    1: F_predPC;
];

word f_predpc = [
    f_icode in { IJXX, ICALL } : f_valC;
    1: f_valP;
];

# 4.30
word f_stat = [
    f_icode == IHALT : SHLT; 
    !instr_valid     : SINS;
    imem_error       : SADR;
    1                : SAOK;
];

# 4.31
word d_dstE = [
    D_icode in { IIRMOVQ, IOPQ } : D_rB;
    D_icode in { IPUSHQ, IPOPQ, ICALL, IRET } : RRSP;
    1 : RNONE;
];

word d_valA = [
    D_icode in { ICALL, IJXX } : D_valP;
    d_srcA == e_dstE : e_valE;
    d_srcA == M_dstM : m_valM;
    d_srcA == M_dstE : M_valE;
    d_srcA == W_dstM : W_valM;
    d_srcA == W_dstE : W_valE;
    1 : d_rvalA;
];

# 4.34
word d_valB = [
    d_srcB == e_dstE : e_valE;
    d_srcB == M_dstM : m_valM;
    d_srcB == M_dstE : M_valE;
    d_srcB == W_dstM : W_valM;
    d_srcB == W_dstE : W_valE;
    1 : d_rvalB;
];

word Stat = [
    W_stat == SBUB : SAOK;
    1 : W_stat;
];

# 4.36
word m_stat = [
    dmem_error : SADR;
    1 : M_stat;
];

# 4.37
# proc:
#     xorq %rax, %rax
#     jne done                F  D  E  M  W
#     irmovq $69, %rax          
#     halt
# done:
#     ret                        F  D  E  M  W


# 4.38
# main:
#     irmovq stack, %rsp
#     call proc
#     halt
# proc:
#     ret
# stack:

bool F_stall =
    # load/use hazard
    E_icode in { IMRMOVQ, IPOPQ } && E_dstM in { d_srcA, d_srcB } || 
    # processing ret
    IRET in { D_icode, E_icode, M_icode };

# 4.39
bool D_stall = 
    # load/use hazard
    E_icode in { IMRMOVQ, IPOPQ } && E_dstM in { d_srcA, d_srcB };
    
# 4.40
bool E_bubble = 
    # load/use hazard
    E_icode in { IMRMOVQ, IPOPQ } && E_dstM in { d_srcA, d_srcB } ||
    # mispredicted branch
    E_icode == IJXX && !e_Cnd;

# 4.41
bool set_cc = E_icode == IOPQ && E_stat == SAOK;

# 4.42
bool M_bubble = 
    m_stat in { SADR, SINS, SHLT }  || 
    W_stat in { SADR, SINS, SHLT }

bool W_stall = W_stat in { SADR, SINS, SHLT };
    





