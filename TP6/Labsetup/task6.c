#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <openssl/bn.h>

/*
 * Vérification manuelle d'une signature de certificat X.509 (RSA + SHA-256).
 *
 * Usage : ./task6 <n_hex> <e_hex> <signature_hex> <hash_hex>
 *   n_hex         : modulus de la clé publique de l'AC (c1.pem)
 *   e_hex         : exposant public de l'AC (ex: 010001)
 *   signature_hex : signature du certificat serveur (c0.pem), sans ':' ni espaces
 *   hash_hex      : sha256sum de c0_body.bin
 */

void printBN(char *msg, BIGNUM *a) {
	char *number_str = BN_bn2hex(a);
	printf("%s %s\n", msg, number_str);
	OPENSSL_free(number_str);
}

int main(int argc, char *argv[]) {
	if (argc != 5) {
		printf("Usage : %s <n_hex> <e_hex> <signature_hex> <hash_hex>\n", argv[0]);
		return 1;
	}

	BN_CTX *ctx = BN_CTX_new();
	BIGNUM *n = BN_new();
	BIGNUM *e = BN_new();
	BIGNUM *S = BN_new();
	BIGNUM *res = BN_new();

	BN_hex2bn(&n, argv[1]);
	BN_hex2bn(&e, argv[2]);
	BN_hex2bn(&S, argv[3]);
	char *hash = argv[4];

	// On "déchiffre" la signature avec la clé publique : res = S^e mod n
	BN_mod_exp(res, S, e, n, ctx);
	printBN("S^e mod n =", res);
	printf("Hash attendu     = %s\n", hash);

	// Le résultat est au format PKCS#1 v1.5 :
	// 0001 FF..FF 00 <DigestInfo SHA-256> <hash>   -> le hash est à la fin
	char *res_hex = BN_bn2hex(res);
	size_t lr = strlen(res_hex), lh = strlen(hash);

	if (lr >= lh && strcasecmp(res_hex + lr - lh, hash) == 0) {
		printf("Résultat : signature VALIDE (le hash correspond)\n");
	} else {
		printf("Résultat : signature INVALIDE\n");
	}
	
	OPENSSL_free(res_hex);
	BN_free(n); BN_free(e); BN_free(S); BN_free(res);
	BN_CTX_free(ctx);
	return 0;
}
