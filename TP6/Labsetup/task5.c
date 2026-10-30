#include <stdio.h>
#include <string.h>
#include <openssl/bn.h>

void printBN(char *msg, BIGNUM *a) {
	char *number_str = BN_bn2hex(a);
	printf("%s %s\n", msg, number_str);
	OPENSSL_free(number_str);
}

// Vérifie une signature : on calcule S^e mod n et on compare avec M
void verifier(char *message, char *signature_hex, BIGNUM *e, BIGNUM *n, BN_CTX *ctx) {
	BIGNUM *M = BN_new();
	BIGNUM *S = BN_new();
	BIGNUM *res = BN_new();

	BN_bin2bn((unsigned char *)message, strlen(message), M);
	BN_hex2bn(&S, signature_hex);

	BN_mod_exp(res, S, e, n, ctx);

	printf("Message : %s\n", message);
	printBN("S =", S);
	printBN("S^e mod n =", res);
	printBN("M          =", M);
	printf("Résultat : signature %s\n\n", BN_cmp(res, M) == 0 ? "VALIDE" : "INVALIDE");

	BN_free(M); BN_free(S); BN_free(res);
}

int main() {
	BN_CTX *ctx = BN_CTX_new();

	BIGNUM *n = BN_new();
	BIGNUM *e = BN_new();

	// Clé publique d'Alice
	BN_hex2bn(&n, "AE1CD4DC432798D933779FBD46C6E1247F0CF1233595113AA51B450F18116115");
	BN_hex2bn(&e, "010001");

	char *message = "Launch a missile.";

	// Signature d'origine
	printf("=== Signature d'origine ===\n");
	verifier(message, "643D6F34902D9C7EC90CB0B2BCA36C47FA37165C0005CAB026C0542CBDB6802F", e, n, ctx);

	// Signature corrompue (dernier octet 2F -> 3F)
	printf("=== Signature corrompue (2F -> 3F) ===\n");
	verifier(message, "643D6F34902D9C7EC90CB0B2BCA36C47FA37165C0005CAB026C0542CBDB6803F", e, n, ctx);

	BN_free(n); BN_free(e);
	BN_CTX_free(ctx);
	return 0;
}
