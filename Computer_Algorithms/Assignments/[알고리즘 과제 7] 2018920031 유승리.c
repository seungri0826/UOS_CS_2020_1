#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint32_t PlainText[16] = { 0 };
uint32_t Message[16] = { 0 };
uint32_t A, B, C, D, E;
uint32_t h[5] = { 0x67452301,0xefcdab89,0x98badcfe,0x10325476,0xc3d2e1f0 };

uint32_t K0_19 = 0x5a827999;
uint32_t K20_39 = 0x6ed9eba1;
uint32_t K40_59 = 0x8f1bbcdc;
uint32_t K60_79 = 0xca62c1d6;
uint32_t W[80] = { 0 };

uint32_t F0_19(uint32_t B, uint32_t C, uint32_t D)
{
    return ((B & C) | (~B & D));
}

uint32_t F20_39(uint32_t B, uint32_t C, uint32_t D)
{
    return (B ^ C ^ D);
}
uint32_t F40_59(uint32_t B, uint32_t C, uint32_t D)
{
    return ((B & C) | (B & D) | (C & D));
}

uint32_t F60_79(uint32_t B, uint32_t C, uint32_t D)
{
    return (B ^ C ^ D);
}

uint32_t ROTATE_LEFT(uint32_t t, uint32_t n)
{
    for (uint32_t i = 0; i < n; i++)
    {
        if (t & 0x80000000) // 최상위 비트가 1이면 left shift 후 1을 최하위 비트로 설정
        {
            t = t << 1;
            t = t | 1;
        }
        else
            t = t << 1;
    }
    return t;
}

void generate_W(void)
{
    for (int t = 0; t < 80; t++)
    {
        if (t >= 0 && t < 16)
            W[t] = Message[t];
        else
            W[t] = ROTATE_LEFT(W[t - 16] ^ W[t - 14] ^ W[t - 8] ^ W[t - 3], 1);
    }
}

void SHA1(void)
{
    A = h[0];
    B = h[1];
    C = h[2];
    D = h[3];
    E = h[4];

    generate_W();

    uint32_t tmp;
    for (int t = 0; t < 80; t++)
    {
        if (t >= 0 && t < 20)       tmp = ROTATE_LEFT(A, 5) + F0_19(B, C, D) + E + W[t] + K0_19;
        else if (t >= 20 && t < 40) tmp = ROTATE_LEFT(A, 5) + F20_39(B, C, D) + E + W[t] + K20_39;
        else if (t >= 40 && t < 60) tmp = ROTATE_LEFT(A, 5) + F40_59(B, C, D) + E + W[t] + K40_59;
        else if (t >= 60 && t < 80) tmp = ROTATE_LEFT(A, 5) + F60_79(B, C, D) + E + W[t] + K60_79;

        E = D;
        D = C;
        C = ROTATE_LEFT(B, 30);
        B = A;
        A = tmp;
    }

    h[0] += A;
    h[1] += B;
    h[2] += C;
    h[3] += D;
    h[4] += E;
}

void padding(uint32_t PlainText[16], uint32_t bitsize)
{
    int full = (bitsize / 32);
    int remainder = bitsize % 32;
    int t, i;

    for (t = 0; t < 14; ++t)
    {
        if (t < full) Message[t] = PlainText[t];
        else if (t == full)
        {
            // Message[]의 같은 위치로 비트 복사
            for (i = 0; i < remainder; ++i)
                Message[t] |= (((PlainText[t] >> (31 - i)) & 0x1) << (31 - i));
            Message[t] |= (0x1 << (31 - i));  // padding 시 Message 부분이 끝나면 1 넣은 후 나머지에 0 넣기
        }
        else
            Message[t] = 0;
    }
    Message[14] = 0;
    Message[15] = bitsize;
}

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        printf("Usage: %s {PlainText}\n", argv[0]);
        return 0;
    }

    else
    {
        char* input_text = argv[1];

        uint32_t PlainText_len = (uint32_t)strlen(input_text);
        uint32_t PlainText_ASCII[64] = { 0 };

        for (int i = 0; i < PlainText_len; ++i)
            PlainText_ASCII[i] = (uint32_t)input_text[i];

        int full = PlainText_len / 4;
        int remainder = PlainText_len % 4;
        int t = 0;

        // char 4개의 ASCII CODE로 꽉 차게 이루어진 PlainText[t]
        for (t = 0; t < full; ++t)
        {
            PlainText[t] = PlainText_ASCII[4 * t + 0] << 24
                | PlainText_ASCII[4 * t + 1] << 16
                | PlainText_ASCII[4 * t + 2] << 8
                | PlainText_ASCII[4 * t + 3] << 0;
        }

        // char의 개수가 4의 배수가 아닌 경우, 남는 char의 ASCII CODE로 PlainText[t]를 Big Endian으로 구성
        if (remainder != 0)
        {
            switch (remainder) {
            case 3:
                PlainText[t] |= PlainText_ASCII[4 * t + 2] << 8;
            case 2:
                PlainText[t] |= PlainText_ASCII[4 * t + 1] << 16;
            case 1:
                PlainText[t] |= PlainText_ASCII[4 * t + 0] << 24;
                break;
            }
        }

        padding(PlainText, PlainText_len * 8);  // padding 시 PlainText_len의 bit 수를 넘겨줌
        SHA1();
        printf("%08X%08X%08X%08X%08X", h[0], h[1], h[2], h[3], h[4]);

        return 0;
    }
}