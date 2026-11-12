#include <stdio.h>
#include <string.h>
#include <openssl/bn.h>

void printBN(char *msg, BIGNUM *a) {
	char *number_str = BN_bn2hex(a);
	printf("%s %s\n", msg, number_str);
	OPENSSL_free(number_str);
}

// Signe directement le message (sans hash) : S = M^d mod n
void signer(char *message, BIGNUM *d, BIGNUM *n, BN_CTX *ctx) {
	BIGNUM *M = BN_new();
	BIGNUM *S = BN_new();

	BN_bin2bn((unsigned char *)message, strlen(message), M);
	BN_mod_exp(S, M, d, n, ctx);

	printf("Message : %s\n", message);
	printBN("M (hex) =", M);
	printBN("Signature S =", S);
	printf("\n");

	BN_free(M);
	BN_free(S);
}

int main() {
	BN_CTX *ctx = BN_CTX_new();

	BIGNUM *n = BN_new();
	BIGNUM *d = BN_new();

	// Clés de la Tâche 2
	BN_hex2bn(&n, "DCBFFE3E51F62E09CE7032E2677A78946A849DC4CDDE3A4D0CB81629242FB1A5");
	BN_hex2bn(&d, "74D806F9F3A62BAE331FFE3F0A68AFE35B3D2E4794148AACBC26AA381CD7D30D");

	signer("I owe you $2000.", d, n, ctx);
	signer("I owe you $3000.", d, n, ctx);

	BN_free(n); BN_free(d);
	BN_CTX_free(ctx);
	return 0;
}
