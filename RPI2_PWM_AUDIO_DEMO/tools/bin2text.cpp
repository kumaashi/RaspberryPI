#include <stdio.h>

int main(int argc, char *argv[]) {
	if(argv[1]) {
		FILE *fp = fopen(argv[1], "rb");
		if(fp) {
			unsigned char c;
			int count = 0;
			printf("unsigned char data[] = { \n");
			while( fread(&c, 1, 1, fp) > 0) {
				if( (count % 16) == 0) printf("\n");
				printf("0x%02X, ", c);
				count++;
			}
			printf("};\n");
			fclose(fp);
		}
	}
}
