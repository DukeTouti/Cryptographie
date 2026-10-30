#include <stdio.h>
#include <openssl/bn.h>

#define NBITS 256

void printBN(char *msg, BIGNUM * a) {
	/* Use BN_bn2hex(a) for hex string
	 * Use BN_bn2dec(a) for decimal string */
	
	// Convertie le BIGNUM en chaîne de nombre hexadecimal
	char * number_str = BN_bn2hex(a);
	
	// Affiche la chaine
	printf("%s %s\n", msg, number_str);
	
	// Libère la mémoire allouée dynamiquement
	OPENSSL_free(number_str);
}

int main () {
	// On crée la variable temporaire ctx qui servira de variable intermédiaire pour les calculs comme le modulo ou la multiplication
	BN_CTX *ctx = BN_CTX_new();

	BIGNUM *a = BN_new();
	BIGNUM *b = BN_new();
	BIGNUM *n = BN_new();
	BIGNUM *res = BN_new();


	// Initialize a, b, n
	BN_generate_prime_ex(a, NBITS, 1, NULL, NULL, NULL);
	BN_dec2bn(&b, "273489463796838501848592769467194369268");
	BN_rand(n, NBITS, 0, 0);

	// res = a*b
	BN_mul(res, a, b, ctx);
	printBN("a * b = ", res);

	// res = a^b mod n
	BN_mod_exp(res, a, b, n, ctx);
	printBN("a^b mod n = ", res);

	return 0;
}

