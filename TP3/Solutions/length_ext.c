/* length_ext.c
 * Hash Length Extension Attack
 * Forge MAC for: original_message + padding + &download=secret.txt
 *
 * Compile: gcc length_ext.c -o length_ext -lcrypto
 */

#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>
#include <openssl/sha.h>

int main(void)
{
    int i;
    unsigned char buffer[SHA256_DIGEST_LENGTH];
    SHA256_CTX c;

    SHA256_Init(&c);

    /* Simulate processing of the first 64-byte block (original padded message).
     * We feed 64 dummy bytes so the internal counter (Nl) reaches 512 bits.
     * The actual hash state is then replaced with the known MAC below. */
    for (i = 0; i < 64; i++)
        SHA256_Update(&c, "*", 1);

    /* MAC of the original message (Task 1):
     * SHA256(123456:myname=MohammedTahaHathouti&uid=1001&lstcmd=1)
     * = df1872e3776050e28a57c1eba85902bfce7285806acaabd2cf5f591d90039305
     */
    c.h[0] = htole32(0xdf1872e3);
    c.h[1] = htole32(0x776050e2);
    c.h[2] = htole32(0x8a57c1eb);
    c.h[3] = htole32(0xa85902bf);
    c.h[4] = htole32(0xce728580);
    c.h[5] = htole32(0x6acaabd2);
    c.h[6] = htole32(0xcf5f591d);
    c.h[7] = htole32(0x90039305);

    /* Append the extra command (without knowing the secret key) */
    SHA256_Update(&c, "&download=secret.txt", 20);
    SHA256_Final(buffer, &c);

    printf("Forged MAC: ");
    for (i = 0; i < 32; i++)
        printf("%02x", buffer[i]);
    printf("\n");

    return 0;
}
