// The followings are SetByte_v2 and SetShort_v2.
// Efficiency improvement is doubtful.
//    A more detailed analysis is required, so I don't think the following will be used. Just a try.
void function SetByte_v2(address, value) {
    var dist;
    __asm {
        PUSHARG address
        PUSH INTV_ARRAY_BASE
        SUB
        POPN dist

        PUSHARG dist
        PUSH 2
        SHR
        PUSHINVR 0

        // 0xFF << ((dist % 4) * 8) occupies the bits on the target.
        // get the dword, but left the byte to write zero.
        // top_stack & ~(0xFF << ((dist % 4) * 8))
        PUSH 0xFF
        PUSHARG dist
        PUSH 0x03
        AND
        PUSH 3
        SHL
        SHL
        NOT
        AND

        // OR the byte to write:
        // top_stack | (value << ((dist % 4) * 8))
        PUSH value
        PUSHARG dist
        PUSH 0x03
        AND
        PUSH 3
        SHL
        SHL
        OR

        PUSHARG dist
        PUSH 2
        SHR
        SETINVR 0
    }
}
