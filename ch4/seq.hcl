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








































