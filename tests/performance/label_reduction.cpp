int main() {
    int value = 3;

    if(value > 0) {
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
