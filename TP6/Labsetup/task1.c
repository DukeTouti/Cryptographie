#include <stdio.h>
#include <openssl/bn.h>

#define NBITS 128

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

	BIGNUM *p = BN_new();
	BIGNUM *q = BN_new();
	BIGNUM *n = BN_new();
	BIGNUM *p1 = BN_new();
	BIGNUM *q1 = BN_new();
	BIGNUM *phi = BN_new();
	BIGNUM *premier = BN_new();
	BIGNUM *pgcd = BN_new();
	BIGNUM *e = BN_new();
	BIGNUM *d = BN_new();


	// Initialisation p, q, e et 1
	BN_hex2bn(&p, "F7E75FDC469067FFDC4E847C51F452DF");
	BN_hex2bn(&q, "E85CED54AF57E53E092113E62F436F4F");
	BN_hex2bn(&e, "0D88C3");
	BN_dec2bn(&premier, "1");
	
	// n = p * q
	BN_mul(n, p, q, ctx);
	printBN("n = p * q = ", n);

	// phi(n) = (p - 1) (q - 1)
	BN_sub(p1, p, premier);
	BN_sub(q1, q, premier);
	BN_mul(phi, p1, q1, ctx);
	printBN("phi = (p - 1) (q - 1) = ", phi);
	
	// Vérification : pgcd(e, phi(n)) == 1
	BN_gcd(pgcd, e, phi, ctx);
	printBN("pgcd(e, phi(n)) = ", pgcd);
	
	// e*d ≡ 1 mod phi(n)
	BN_mod_inverse(d, e, phi, ctx);
	
	printBN("n = ", n);
	printBN("phi = ", phi);
	printBN("d = ", d);
	
	BN_free(p);
	BN_free(q);
	BN_free(n);
	BN_free(phi);
	BN_free(premier);
	BN_free(pgcd);
	BN_free(e);
	BN_free(d);
	BN_CTX_free(ctx);
	

	return 0;
}

