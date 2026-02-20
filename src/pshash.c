#include "algorithm.h"
#include <stdio.h>
#include <stdlib.h>

const char modstr[202] = "100000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000000";
const char mnemostr[201] = "orunelisitusofagumyuinerestireteleralirinesedecoroladicatavehesimepenilomamitocenahogehihapopanocipikemobabesafibosusobituvigirukugakoqulukidofefobudawevafuwafamupugowogudunuhuvoyizeyejujoxikaxejazije";

void print (mpz_t x) {
    char x_str[MAXSIZE_BIG];
    mpz_get_str(x_str, 10, x);
    printf("%s\n", x_str);
}

void parse_key_mnemonic (mpz_t to, char* expr) {
    char buf[128]; int ind = 0;
    while (*expr != '\0' && *(expr + 1) != '\0') {
        if (*expr == ' ' || *(expr + 1) == ' ') {
            expr += 2; continue;
        }
        short found = -1;
        for (int i = 0; i < 100; i++) {
            if (*expr == mnemostr[2*i] && *(expr + 1) == mnemostr[2*i + 1]) {
                found = i;
                break;
            }
        }
        if (found == -1) {
            printf("ERROR: Unknown mnemonic syllable: %c%c\n", *expr, *(expr + 1));
            return;
        }

        if (found < 10) {
            buf[ind] = '0';
            buf[ind + 1] = '0' + found;
        } else {
            buf[ind] = '0' + found/10;
            buf[ind + 1] = '0' + found % 10;
        }

        ind += 2;
        expr += 2;
    }

    buf[ind] = '\0';

    mpz_set_str(to, buf, 10);
}

void parse_key_arithmetic (mpz_t to, mpz_t mod, char* expr, short mode) {
    if (mode == 0) {
        mpz_set_str(to, expr, 10);
        return;
    }

    mpz_t temp; mpz_init(temp);
    char* start = expr;
    char* end = expr;

    if (mode == 3) {
        mpz_set_ui(to, 0);
        while (1) {
            if (*end != '+' && *end != '\0') {
                end += 1;
                continue;
            }
            
            char save = *end;
            *end = '\0';
            parse_key_arithmetic(temp, mod, start, mode - 1);
            mpz_add(to, to, temp);

            if (save == '\0') {
                mpz_clear(temp);
                return;
            }

            start = end + 1;
            end += 1;
        }
    }

    if (mode == 2) {
        mpz_set_ui(to, 1);
        while (1) {
            if (*end != '*' && *end != '\0') {
                end += 1;
                continue;
            }
            
            char save = *end;
            *end = '\0';
            parse_key_arithmetic(temp, mod, start, mode - 1);
            mpz_mul(to, to, temp);

            if (save == '\0') {
                mpz_clear(temp);
                return;
            }

            start = end + 1;
            end += 1;
        }
    }

    if (mode == 1) {
        mpz_set_ui(to, 1);
        while (1) {
            if (*end != '^' && *end != '\0') {
                end += 1;
                continue;
            }
            
            char save = *end;
            *end = '\0';

            if (save == '\0') {
                parse_key_arithmetic(to, mod, start, 0);
            } else {
                mpz_t exp; mpz_init(exp);
                parse_key_arithmetic(exp, mod, end + 1, 1);
                // unsigned long exp_int = atoi()
                parse_key_arithmetic(temp, mod, start, 0);
                mpz_powm(to, temp, exp, mod);
                mpz_clear(exp);
            }

            mpz_clear(temp);
            break;
        }
    }
}

int main (int argc, char** argv) {
    if (argc != 2) {
        printf("ERROR: Expected one argument.\n");
        exit(1);
    }

    mpz_t public; mpz_init(public);
    get_public_key(public, argv[1]);

    char keystr[2][128];
    
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 128; j++) {
            char cur = getc(stdin);
            if (cur == '\n') {
                keystr[i][j] = '\0';
                break;
            }
            keystr[i][j] = cur;
        }
    }

    mpz_t choice; mpz_init(choice);
    mpz_t shuffle; mpz_init(shuffle);
    mpz_t mod; mpz_init(mod); mpz_set_str(mod, modstr, 10);

    if ('a' <= keystr[0][0] && keystr[0][0] <= 'z') {
        parse_key_mnemonic(choice, keystr[0]);
    } else {
        parse_key_arithmetic(choice, mod, keystr[0], 3);
    }

    if ('a' <= keystr[1][0] && keystr[1][0] <= 'z') {
        parse_key_mnemonic(shuffle, keystr[1]);
    } else {
        parse_key_arithmetic(shuffle, mod, keystr[1], 3);
    }

    mpz_clear(mod);

    mpz_add(choice, choice, public);

    char sourceLower[27] = "ckapzfitqdxnwehrolmbyvsujg";
    char sourceUpper[27] = "RQLIANBKJYVWPTEMCZSFDOGUHX";
    char sourceSpecial[13] = "=!*@?$%#&-+^";
    char sourceNumbers[11] = "1952074386";
    struct source srcs[4];
    srcs[0].elts = sourceLower; srcs[0].amount = 8;
    srcs[1].elts = sourceUpper; srcs[1].amount = 8;
    srcs[2].elts = sourceSpecial; srcs[2].amount = 5;
    srcs[3].elts = sourceNumbers; srcs[3].amount = 4;
    struct configuration config;
    config.srcs = srcs;
    config.size = 4;

    char hash[26];
    get_hash(hash, &config, choice, shuffle);

    printf("%s\n", hash);
}
