#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pffr.h"

int main(int argc, char *argv[]) {
    Pffr pffr;
    FILE *pf = NULL;

 // ファイルのパスを設定
    if(argc == 2) {
        if(strlen(argv[1]) > 254) {
            fputs("error: ファイル名が長すぎます\n", stderr);
            exit(1);
        }
        strcpy(pffr.path, argv[1]);
    } else if(argc < 2) {
        int canSetPath = 0;
        for(int i = 0; i < 256; i ++) {
            sprintf(pffr.path, "名称未設定%d.pffr", i);
            pf = fopen(pffr.path, "r");
            if(!pf) {
                fclose(pf);
                canSetPath = 1;
                break;
            }
        }
        if(!canSetPath) {
            fputs("error: 名称未設定のファイルが多すぎます\n", stderr);
            exit(1);
        }
    } else {
        fputs("error: 不要な引数があります\n", stderr);
        exit(1);
    }

 // ファイルの内容を取得
    pf = fopen(pffr.path, "r");
    if(pf) {
        getFileContent(&pffr);
    } else {
        createDefaultFile(&pffr);
    }
    fclose(pf);

    free(pffr.page);
    exit(0);
}
