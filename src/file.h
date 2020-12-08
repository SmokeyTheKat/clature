#ifndef __ddScript_file_h__
#define __ddScript_file_h__

void write_file(const char* path, ddString content)
{
	FILE* fp = fopen(path, "w");
	if (fp == null) return;
	fwrite(content.cstr, sizeof(char), content.length, fp);
	fclose(fp);
}

ddString read_file(const char* path)
{
	FILE* fp = fopen(path, "r");
	if (fp == null) return make_constant_ddString("ERROR");
	fseek(fp, 0L, SEEK_END);
	long nb = ftell(fp) - 1;
	fseek(fp, 0L, SEEK_SET);
	char* buffer = (char*)calloc(nb+1, sizeof(char));
	fread(buffer, sizeof(char), nb, fp);
	fclose(fp);
	buffer[nb] = '\0';
	return make_ddString(buffer);
}

#endif
