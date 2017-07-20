#include <cstdio>
#include <cstring>
#include <string>
#include <map>
#include <vector>
#include <regex>

struct Function {
	char name[64];
	int arg_cnt;
	int callsign;
};
std::map<std::string, Function> magic_table;

std::vector<std::string> string_table;

//returns hasnext
bool nextline_cstr(char* target, int len, char*& src) {
	for (int i = 0; i < len; i++) {
		if (*src == '\n' || *src == '\r') {
			*target = '\0';
			while(*src != '\n') src++; //skip \r
			src++; //skip \n
			return true;
		} else if (*src == '\0') {
			*target = '\0';
			return false;
		}

		*target = *src;
		target++;
		src++;

	}
	return true;
}

void load_magic_table(char* magic_table_area) {
	char line[1024];
	char *p;
	while(nextline_cstr(line, 1024, magic_table_area)) {
		//skip spaces
		p = line;
		while(*p == ' ' || *p == '\t')
			p++;

		Function func;
		int read = sscanf(p, "MAGIC_%*d : procedure %s %d STRING_%*d %d",
			func.name, &func.arg_cnt, &func.callsign);
		if(read == 0 || read == EOF)
			break;

		std::string name (func.name);
		magic_table[name] = func;
	}
}

void load_string_decl(char* string_decl_area) {
	char line[1024];
	char *p;
	while(nextline_cstr(line, 1024, string_decl_area)) {
		//skip spaces
		p = line;
		while(*p == ' ' || *p == '\t')
			p++;

		int idx;
		char str[1024];
		int read = sscanf(p, "STRING_%d: DB \"%s", &idx, str);

		if(read == 0 || read == EOF)
			break;

		//get rid of '"' at the end of line
		for(char *q = str; *q != '\0'; q++) {
			if(*q == '"') {
				*q = '\0';
				break;
			}
		}

		std::string stlstr (str);
		string_table.push_back(stlstr);
	}
}

int main(int argc, char** argv) {
    if(argc != 3) {
        printf("Usage: AsmToCpp <asmfile> <cppfile>");
        return 1;
    }
    FILE* fin = fopen(argv[1], "r");
    FILE* fout = fopen(argv[2], "w");

	char* input_src = new char[1048576];
	char* input = input_src;
	while((*input = (char) fgetc(fin)) != EOF)
		input++;

	char* line = new char[1024];

	//init code blocks, and load.
	//code area
	char* code_area = strstr(input_src, "org 40h");
	if(code_area != NULL) {
		code_area += 7;
		while(*code_area != '\n')
			code_area ++;
		code_area ++;
	} else
		code_area = input_src;

	//magic table
	char* magic_table_area = strstr(input_src, "magic_table_addr:");
	if(magic_table_area != NULL) {
		nextline_cstr(line, 1024, magic_table_area); //skip line "magic_table_addr:"
		load_magic_table(magic_table_area);
	}

	//string decls
	char* string_decl_area = strstr(input_src, "STRING_0:");
	if(string_decl_area != NULL)
		load_string_decl(string_decl_area);

	char* end_of_code_area = strstr(code_area, "include");
	if(end_of_code_area == NULL) {
		if(string_decl_area != NULL)
			end_of_code_area = string_decl_area;
		else
			end_of_code_area = magic_table_area;
	}


	bool first_func = true;

	//Go go go!
	while(nextline_cstr(line, 1024, code_area) &&
			(end_of_code_area == NULL || code_area < end_of_code_area)) {
		//check if this line is a func decl (a label and does not starts with LOC_)
		if(strstr(line, ":") != NULL && strncmp(line, "LOC_", 4) != 0) {
			//get rid of ':'
			for(char* p = line; *p != '\0'; p++) {
				if(*p == ':') {
					*p = '\0';
					break;
				}
			}

			if(!first_func)
				fprintf(fout, "}}\n\n");

			first_func = false;

			//in magic table?
			std::string name (line);
			if(magic_table.find(name) != magic_table.end()) {
				Function func = magic_table[name];
				fprintf(fout, "void function %s (", line);
				for(int i = 0; i < func.arg_cnt; i++) {
					fprintf(fout, "arg_%d", i);
					if(i != func.arg_cnt - 1)
						fprintf(fout, ", ");
				}
				fprintf(fout, ") ");
				if(func.callsign != 0)
					fprintf(fout, "callsign %d ", func.callsign);
				fprintf(fout, "{\n");

			//nope: then let's pretend it is a function with no args and return val.
			//asm code will handle all this.
			//(Parser will produce a redunant RETN but won't cause trouble.)
			} else {
				fprintf(fout, "void function %s () {\n", line);
			}
			fprintf(fout, "__asm{\n");

		//else it is a normal asm code.
		} else {
			//a special deal with strings.
			int string_no;

			//skip spaces
			char *p = line;
			while(*p == ' ' || *p == '\t')
				p++;

			int bytesread = sscanf(p, "PUSHSTR %d", &string_no);
			if(bytesread == 0 || bytesread == EOF)
				fprintf(fout, "%s\n", line);
			else
				fprintf(fout, "\tPUSHSTR \"%s\"\n", string_table[string_no].c_str());

		}
	}

	//last end-of-func
	fprintf(fout, "}}\n");

	//end
	fclose(fin);
	fclose(fout);
	delete[] input_src;
	delete[] line;
	return 0;

}
