int main() {
    int value = 3;

    __asm {
        raw_entry: ; raw_alias must remain unchanged in this comment
        raw_alias:
        DD "raw_alias"
        JMP raw_alias
    }

    if(value > 0) {
        user_entry:
        user_alias:
        goto user_alias;

        while(value > 0) {
            if(value == 2) {
                value--;
                continue;
            } else if(value == 1) {
                break;
            }
            value--;
        }
    } else {
        value = -1;
    }

    return value;
}
