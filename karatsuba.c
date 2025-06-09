// karatsuba.c
// Implementasi Karatsuba 256-bit menggunakan helper ASM
#include <stdint.h>
#include <string.h>
#include <stdio.h>

extern void bignum_mul128(uint64_t *a, uint64_t *b, uint64_t *result);

void bignum_add128(uint64_t *a, uint64_t *b, uint64_t *r) {
    uint64_t carry;
    r[0] = a[0] + b[0];
    carry = (r[0] < a[0]);
    r[1] = a[1] + b[1] + carry;
}

void bignum_sub256(uint64_t *a, uint64_t *b, uint64_t *r) {
    uint64_t borrow;
    r[0] = a[0] - b[0];
    borrow = (a[0] < b[0]);
    r[1] = a[1] - b[1] - borrow;
    borrow = (a[1] < b[1] + borrow);
    r[2] = a[2] - b[2] - borrow;
    borrow = (a[2] < b[2] + borrow);
    r[3] = a[3] - b[3] - borrow;
}

void karatsuba_mul256(uint64_t *a, uint64_t *b, uint64_t *res) {
    uint64_t z0[4] = {0}, z1[4] = {0}, z2[4] = {0};
    uint64_t a0[2] = {a[0], a[1]};
    uint64_t a1[2] = {a[2], a[3]};
    uint64_t b0[2] = {b[0], b[1]};
    uint64_t b1[2] = {b[2], b[3]};

    uint64_t a_sum[2], b_sum[2];
    bignum_add128(a0, a1, a_sum);
    bignum_add128(b0, b1, b_sum);

    uint64_t p0[4] = {0}, p1[4] = {0}, p2[4] = {0};
    bignum_mul128(a0, b0, p0);      // z0
    bignum_mul128(a1, b1, p2);      // z2
    bignum_mul128(a_sum, b_sum, p1); // (a0+a1)(b0+b1)

    // z1 = p1 - p0 - p2
    uint64_t tmp[4];
    bignum_sub256(p1, p0, tmp);
    bignum_sub256(tmp, p2, z1);

    memcpy(z0, p0, 4 * sizeof(uint64_t));
    memcpy(z2, p2, 4 * sizeof(uint64_t));

    // Compose result: res = z2<<256 + z1<<128 + z0
    memset(res, 0, 8 * sizeof(uint64_t));
    // z0
    res[0] = z0[0]; res[1] = z0[1]; res[2] = z0[2]; res[3] = z0[3];
    // z1 << 128
    res[2] += z1[0];
    res[3] += z1[1];
    res[4] += z1[2];
    res[5] += z1[3];
    // z2 << 256
    res[4] += z2[0];
    res[5] += z2[1];
    res[6] += z2[2];
    res[7] += z2[3];
}

// Test
int main() {
    uint64_t a[4] = {0xFFFFFFFFFFFFFFFF, 0x1, 0x2, 0x0};
    uint64_t b[4] = {0x1, 0x1, 0x1, 0x1};
    uint64_t res[8] = {0};

    karatsuba_mul256(a, b, res);

    printf("Result (512-bit) =\n");
    for (int i = 7; i >= 0; --i)
        printf("%016lx ", res[i]);
    printf("\n");
    return 0;
}
