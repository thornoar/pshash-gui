#include "algorithm.h"
#include <stdio.h>
#include <stdlib.h>

const int EXP_MOD = 250;

const char mnemostr[201] = "orunelisitusofagumyuinerestireteleralirinesedecoroladicatavehesimepenilomamitocenahogehihapopanocipikemobabesafibosusobituvigirukugakoqulukidofefobudawevafuwafamupugowogudunuhuvoyizeyejujoxikaxejazije";

void print (mpz_t x) {
    char x_str[MAXSIZE_BIG];
    mpz_get_str(x_str, 10, x);
    printf("%s\n", x_str);
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
        printf("ERROR: Expected only one argument.\n");
        exit(1);
    }

    char* pubkey = argv[1];

    char keystr[2][128];
    
    for (int i = 0; i < 2; i++) {
        fprintf(stderr, "KEY %i: ", i+1);
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
    mpz_t mod; mpz_init(mod);
    char mod_str[EXP_MOD + 1];
    mod_str[0] = '1';
    for (int i = 1; i < EXP_MOD; i++) {
        mod_str[i] = '0';
    }
    mod_str[EXP_MOD] = '\0';
    mpz_set_str(mod, mod_str, 10);

    parse_key_arithmetic(choice, mod, keystr[0], 3);
    parse_key_arithmetic(shuffle, mod, keystr[1], 3);

}
