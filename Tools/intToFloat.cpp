#include <cstdio>

int main() {
    int source;
    scanf("%d", &source);
    int* ptr = &source;
    float* ptr2 = (float *) ptr;
    printf("%f", *ptr2);
    return 0;
}