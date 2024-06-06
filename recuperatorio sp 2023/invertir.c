#define PGM_FILE "cat.pgm"
unsigned char imagen[2000*2000];

int main(){
    int in_fd = open(PGM_FILE, O_RDONLY);
	if (in_fd < 0) {
		something_wrong(in_fd, "Error open");
	}
	lseek(in_fd,15,SEEK_CUR);
	
	
	int out_fd=open("outCat.pgm",)
	
    return 0;
}
