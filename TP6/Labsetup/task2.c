#include <stdio.h>
#include <string.h>
#include <openssl/bn.h>

void printBN(char *msg, BIGNUM *a) {
	char *number_str = BN_bn2hex(a);
	printf("%s %s\n", msg, number_str);
	OPENSSL_free(number_str);
}

int main() {
	BN_CTX *ctx = BN_CTX_new();

	BIGNUM *n = BN_new();
	BIGNUM *e = BN_new();
	BIGNUM *d = BN_new();
	BIGNUM *M = BN_new();
	BIGNUM *C = BN_new();
	BIGNUM *verif = BN_new();

	// Clé publique (e, n) et clé privée d (pour vérifier le résultat)
	BN_hex2bn(&n, "DCBFFE3E51F62E09CE7032E2677A78946A849DC4CDDE3A4D0CB81629242FB1A5");
	BN_hex2bn(&e, "010001");
	BN_hex2bn(&d, "74D806F9F3A62BAE331FFE3F0A68AFE35B3D2E4794148AACBC26AA381CD7D30D");

	// Conversion du message ASCII en BIGNUM (équivalent de ASCII -> hex -> BN_hex2bn)
	char *message = "A top secret!";
	BN_bin2bn((unsigned char *)message, strlen(message), M);
	printBN("M (hex) =", M);

	// Chiffrement : C = M^e mod n
	BN_mod_exp(C, M, e, n, ctx);
	printBN("C = M^e mod n =", C);

	// Vérification : déchiffrer C avec d doit redonner M
	BN_mod_exp(verif, C, d, n, ctx);
	printBN("C^d mod n =", verif);
	printf("Vérification : %s\n", BN_cmp(verif, M) == 0 ? "OK (on retrouve M)" : "ERREUR");

	BN_free(n); BN_free(e); BN_free(d);
	BN_free(M); BN_free(C); BN_free(verif);
	BN_CTX_free(ctx);
	return 0;
}
