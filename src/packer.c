#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define PACKER_MAX_ARRAY_LINE_LEN 70
#define PACKER_TAB 2

typedef struct buffer
{
	unsigned char *data;
	size_t size;
} buffer;

char *str_to_upper(char *s)
{
	char *tmp = s;

	for (; *tmp; ++tmp)
	{
		*tmp = toupper((unsigned char)*tmp);
	}

	return s;
}

struct buffer loadFile(const char *_path, const char *_mode)
{
	struct buffer buffer;
	FILE *f;

	f = fopen(_path, _mode);
	memset(&buffer, 0, sizeof(buffer));

	if (f == 0)
		return buffer;

	fseek(f, 0, SEEK_END);
	buffer.size = (size_t)ftell(f);
	buffer.data = (unsigned char *)malloc(buffer.size);
	if (buffer.data == 0)
		return buffer;
	fseek(f, 0, SEEK_SET);
	fread(buffer.data, sizeof(*buffer.data), buffer.size, f);
	fclose(f);

	return buffer;
}

const char *getFileName(const char *_path)
{
	const char *path_delims = "\\/";
	const char *name = NULL;
	const char *path_iter = _path;

	if (!_path)
		return _path;

	while ((path_iter = strpbrk(path_iter, path_delims)))
	{
		name = path_iter++;
	}

	if (!name)
		return _path;

	return name + 1;
}

/* Stole this part from RAYLIB code.*/
const char *getFileExt(const char *_path)
{
	const char *dot = strrchr(_path, '.');

	if (!dot || dot == _path)
		return _path;

	return dot;
}

void stringReplace(
	char *_string,
	const size_t _string_size,
	const char _find,
	const char _replace_with)
{
	char *str_iter = _string;
	while ((size_t)(str_iter - _string) < _string_size &&
		   *str_iter != 0)
	{
		if (*str_iter == _find)
			*str_iter = _replace_with;
		str_iter++;
	}
}

int main(int _argc, const char **_argv)
{
	const char *mode, *input_path, *output_path;
	size_t i = 0, j, temp_line_len = 0;
	FILE *output_file;
	struct buffer input_data;
	char output_file_prefix[256];
	const size_t hex_code_len = strlen("0xff, ");

	if (_argc <= 3)
	{
		printf("[ ERROR ]:\t too few arguments!\n");
		return EXIT_FAILURE;
	}

	mode = _argv[1];
	input_path = _argv[2];
	output_path = _argv[3];

	printf(
		"Input: %s; Output: %s;\n",
		input_path,
		output_path);

	input_data = loadFile(input_path, mode);

	if (!input_data.data || !input_data.size)
	{
		printf(
			"[ ERROR ]:\t cannot open input file! %s\n",
			input_path);
		return EXIT_FAILURE;
	}

	output_file = fopen(output_path, "w");
	if (!output_file)
	{
		printf(
			"[ ERROR ]:\t cannot save output file! %s\n",
			output_path);
		return EXIT_FAILURE;
	}

	memset(output_file_prefix, 0, sizeof(char) * 256);
	strcpy(output_file_prefix, input_path);
	str_to_upper(output_file_prefix);
	stringReplace(output_file_prefix,
				  strlen(output_file_prefix),
				  '.',
				  '_');
	stringReplace(output_file_prefix,
				  strlen(output_file_prefix),
				  '\\',
				  '_');
	stringReplace(output_file_prefix,
				  strlen(output_file_prefix),
				  '/',
				  '_');

	fprintf(
		output_file,
		"%s",
		"/* Simple packer. 1.0.0 */\n\n");

	fprintf(
		output_file,
		"#define %s_SIZE %u\n"
		"#define %s_EXT \"%s\"\n\n\n"
		"static const unsigned char %s[] = {\n",
		output_file_prefix,
		(unsigned int)input_data.size,
		output_file_prefix,
		getFileExt(input_path),
		output_file_prefix);

	while (i < input_data.size)
	{
		j = 0;
		if (!temp_line_len)
		{
			while (j < PACKER_TAB)
			{
				fputc(' ', output_file);
				j++;
			}
			temp_line_len += PACKER_TAB;
		}

		fprintf(
			output_file,
			"0x%.2x, ",
			input_data.data[i]);
		temp_line_len += hex_code_len;

		if (temp_line_len + hex_code_len >=
			PACKER_MAX_ARRAY_LINE_LEN)
		{
			fputc('\n', output_file);
			temp_line_len = 0;
		}

		i++;
	}

	fputc('\n', output_file);
	j = 0;
	while (j < PACKER_TAB)
	{
		fputc(' ', output_file);
		j++;
	}
	fprintf(
		output_file,
		"0x00");

	fprintf(
		output_file,
		"\n};\n");

	fclose(output_file);
	return EXIT_SUCCESS;
}