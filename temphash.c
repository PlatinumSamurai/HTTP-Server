#include <stdio.h>
#include <string.h>
#include <stdlib.h>

size_t Hash(char *name, char *id) {
    char *res = NULL;
    int sum = 0;
    size_t result = 533436788;
    size_t size = strlen(name) + strlen(id) + 1;
    
    res = (char *)malloc(size);
    memset(res, 0, size);
    strcpy(res, name);
    //strcat(res, id);
    for(int i = 0; i < strlen(res); ++i) {
        sum += res[i] * (i + 1);
    }

    srand(sum);

    for(int i = 0; i < strlen(res); ++i) {
        if(rand() % 2) {
            char ch = res[i];
            int rnd = rand() % strlen(res);
            res[i] = res[strlen(res) - 1 - rnd];
            res[strlen(res) - 1 - rnd] = ch;
        }
    }
    printf("permutated string is \"%s\"\n\n", res);
   
    for(int i = 0; i < strlen(res); ++i) {
        size_t temp;

        goto label4;
label0:
        temp = (temp << 2); // step 1
        goto label6;
label1:
        result = temp ^ result ; // step 4
        goto label7;
label2:
        result = (result >> 1) - res[i] + '0'; // step 3
        goto label1;
label3:
        temp = temp * res[i]; // step 2
        goto label2;
label4:
        temp = ~result;
        goto label0;
label5:
        temp = temp ^ 3;
        goto label3;
label6:
        temp = ~temp;
        goto label5;
label7:
//        result = ((result << 2) * res[i]) ^ ((result >> 1) - res[i] + '0');
    }

    free(res);

    return result;
}

int main() {
    printf("hash = %zu\n", Hash("Alexander", ""));

    return 0;
}
