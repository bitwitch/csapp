# Name          Value(hex)
# --------      ----------
# IHALT       :     0
# INOP        :     1
# IRRMOVQ     :     2
# IIRMOVQ     :     3
# IRMMOVQ     :     4
# IMRMOVQ     :     5
# IOPL        :     6
# IJXX        :     7
# ICALL       :     8
# IRET        :     9
# IPUSHQ      :     A
# IPOPQ       :     B
# FNONE       :     0
# RESP        :     4
# RNONE       :     F
# ALUADD      :     0
# SAOK        :     1
# SADR        :     2
# SINS        :     3
# SHLT        :     4


bool need_regids = icode in { IRRMOVQ, IOPQ, IPUSHQ, IPOPQ, IIRMOVQ, IRMMOVQ, IMRMOVQ };

# 4.19
bool need_valC = icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ, IJXX, ICALL }; 

word srcA = [
    icode in { IRRMOVQ, IRMMOVQ, IOPQ, IPUSHQ } : rA;
    icode in { IPOPQ, IRET } : RRSP;
    1 : RNONE;
];

# 4.20
word srcB = [
    icode in { IRMMOVQ, IMRMOVQ, IOPQ } : rB;
    icode in { IPUSHQ, IPOPQ, ICALL, IRET } : RRSP;
    1 : RNONE;
];

# 4.21
word dstM = [
    icode in { IMRMOVQ, IPOPQ } : rA;
    1 : RNONE;
];

word aluA = [
    icode in { IRRMOVQ, IOPQ } : valA;
    icode in { IIRMOVQ, IRMMOVQ, IMRMOVQ } : valC;
    icode in { ICALL, IPUSHQ } : -8;
    icode in { IRET, IPOPQ } : 8;
]

# 4.23
word aluB = [
    icode in { IOPQ, IRMMOVQ, IMRMOVQ, IPUSHQ, IPOPQ } : valB;    
    icode in { IRRMOVQ, IIRMOVQ } : 0;
];

word alufun = [
    icode == IOPQ : ifun;
    1 : ALUADD;
];

bool set_cc = icoode == IOPQ;

# 4.24
word dstE = [
    icode in { IRRMOVQ } && Cnd : rB;
    icode in { IIRMOVQ, IOPQ } : rB;
    icode in { IPUSHQ, IPOPQ, ICALL, IRET } : RRSP;
    1 : RNONE;
];

word mem_addr = [
    icode in { IRMMOVQ, IPUSHQ, ICALL, IMRMOVQ } : valE;
    icode in { IPOPQ, IRET } : valA;
];

# 4.25
word mem_data = [
    icode in { IRMMOVQ, IPUSHQ } : valA;
    icode == ICALL : valP;
];

bool mem_read = icode in { IMRMOVQ, IPOPQ, IRET };

# 4.26
bool mem_write = icode in {  IRMMOVQ, IPUSHQ, ICALL };

# 4.27
word stat = [
    icode == IHALT : 4;              # SHLT
    !instr_valid : 3;                # SINS
    imem_error || dmem_error : 2;    # SADR
    1 : 1;                           # SAOK
];


word new_pc = [
    icode == ICALL : valC;
    icode == IJXX && Cnd : valC;
    icode == IRET : valM;
    1 : valP;
];









































