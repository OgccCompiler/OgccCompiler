#include <stdint.h>

// Global storage arrays for values that do not fit into the 8-byte instruction
uint64_t Global_Constants; // Replaces your 'Num' array
uint64_t Memory_Displacements; // Replaces your 'PDis' array

// The highly efficient 8-byte (64-bit) Instruction Structure
struct X86_Instruction {
    
    // --- 1. PREFIXES (Modify the behavior of the instruction) ---
    uint32_t Prefix_LockRep : 2; // 0 = None, 1 = LOCK (F0), 2 = REPNE (F2), 3 = REP/REPE (F3)
    uint32_t Prefix_Segment : 3; // 0 = None, 1 = CS, 2 = SS, 3 = DS, 4 = ES, 5 = FS, 6 = GS
    uint32_t Prefix_OpSize : 1; // 1 = Operand-Size Override active (0x66) -> e.g., switches between 16/32-bit
    uint32_t Prefix_AddrSize : 1; // 1 = Address-Size Override active (0x67) -> switches addressing mode
    
    // --- 2. COMMAND (What should the CPU do?) ---
    uint32_t Mnemonic_ID : 11; // Instruction ID (e.g., 0 = ADD, 1 = SUB, 2 = MOV...)
    uint32_t Data_Size : 2; // Data type directive: 0 = BYTE, 1 = WORD, 2 = DWORD, 3 = QWORD
    
    // --- 3. OPERAND CONTROL (Who interacts with whom?) ---
    uint32_t Mode_Operand_A : 1; // 0 = Constant (from array), 1 = CPU Register
    uint32_t Mode_Operand_B : 1; // 0 = Memory Address (Pointer active), 1 = CPU Register
    
    // Multi-functional ID: Stores either a Register ID OR an index for the global constants array.
    // Additionally, the 5th bit (value 16) controls the direction when a memory pointer is active:
    // -> Value < 16  : Register is DESTINATION, Memory is SOURCE (Reg reads from memory)
    // -> Value >= 16 : Memory is DESTINATION, Register is SOURCE (Reg writes to memory)
    uint32_t Multi_Operand_ID : 16; 
    
    // --- 4. MEMORY POINTER / S.I.B. (Active if Mode_Operand_B == 0) ---
    uint32_t Pointer_BaseReg : 4; // Base register for addressing (e.g., EBX). Note: If Mode_A=0 & Mode_B=1, Operand A reuses this as its register!
    uint32_t Pointer_IndexReg : 4; // Index register (e.g., ECX). 0 means: No index register active
    uint32_t Pointer_Scale : 2; // Multiplier for the index register: 0 = *1, 1 = *2, 2 = *4, 3 = *8
    uint32_t Pointer_Offset_ID : 16; // Index for the 'Memory_Displacements' array. 0xFFFF means: No offset/displacement
};

// Test array for GPU code generation
X86_Instruction Code_Section;

/*
========================================================================================================
 THE FINAL LOGIC MATRIX (State Evaluation in the Compiler/Interpreter)
========================================================================================================

 1. REG to REG (Copy/calculate register into another register)
    Condition: (Mode_Operand_A == 1 && Mode_Operand_B == 1)
    -> 1st Operand (Dest)   = Register from 'Multi_Operand_ID'
    -> 2nd Operand (Source) = Register from 'Pointer_BaseReg'
    Example: mov eax, ebx

 2. REG to NUM (Load an immediate value/constant into a register)
    Condition: (Mode_Operand_A == 0 && Mode_Operand_B == 1)
    -> 1st Operand (Dest)   = Register from 'Pointer_BaseReg'
    -> 2nd Operand (Source) = Constant from 'Global_Constants[Multi_Operand_ID]'
    Example: mov eax, 500

 3. REG from POINTER (Read data from RAM into a register)
    Condition: (Mode_Operand_A == 1 && Mode_Operand_B == 0 && Multi_Operand_ID < 16)
    -> 1st Operand (Dest)   = Register from 'Multi_Operand_ID'
    -> 2nd Operand (Source) = Memory address calculated from S.I.B. fields (Pointer_...)
    Example: mov eax, [ebx + ecx * 4 + 12]

 4. POINTER from REG (Write data from a register into RAM)
    Condition: (Mode_Operand_A == 1 && Mode_Operand_B == 0 && Multi_Operand_ID >= 16)
    -> 1st Operand (Dest)   = Memory address calculated from S.I.B. fields (Pointer_...)
    -> 2nd Operand (Source) = Register from 'Multi_Operand_ID' (Strip the 5th direction bit with '& 0x0F'!)
    Example: mov [ebx + ecx * 4 + 12], eax

 5. POINTER from NUM (Write an immediate value/constant directly into RAM)
    Condition: (Mode_Operand_A == 0 && Mode_Operand_B == 0)
    -> 1st Operand (Dest)   = Memory address calculated from S.I.B. fields (Pointer_...)
    -> 2nd Operand (Source) = Constant from 'Global_Constants[Multi_Operand_ID]'
    Example: mov DWORD PTR [ebx], 5
*/